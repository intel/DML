/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include "dml_dif.h"

#include <dml/dmldefs.h>
#include <dml_kernels.h>
#include <stddef.h>

#define OWN_DIF_CRC_POLYNOMIAL 0x8BB7u /**< CRC16 T10 polynomial */

#if defined(__GNUC__)
/**
 * @brief Packs a structure with byte by byte
 */
#define OWN_BYTE_PACKED_STRUCTURE_BEGIN typedef struct __attribute__((__packed__))
#elif (_MSC_VER)
/**
     * @brief Packs a structure with byte by byte
     */
#define OWN_BYTE_PACKED_STRUCTURE_BEGIN __pragma(pack(push, 1)) typedef struct
#else
#error Compiler not supported
#endif

#if defined(__GNUC__)
/**
 * @brief Pops a previous structure pack property
 */
#define OWN_BYTE_PACKED_STRUCTURE_END
#elif (_MSC_VER)
/**
    * @brief Pops a previous structure pack property
    */
#define OWN_BYTE_PACKED_STRUCTURE_END __pragma(pack(pop))
#else
#error Compiler not supported
#endif

typedef enum
{
    OWN_FALSE = 0u, /**< Bool True  */
    OWN_TRUE  = 1u  /**< Bool False */
} dml_bool_t;

/**
 * @brief Description of Data Integrity Field (DIF)
 */
OWN_BYTE_PACKED_STRUCTURE_BEGIN
{
    uint16_t guard_tag;       /**< CRC16 checksum                              */
    uint16_t application_tag; /**< Teg used to set compliance with application */
    uint32_t reference_tag;   /**< Teg used to set compliance with data group  */
}
own_dif_t;
OWN_BYTE_PACKED_STRUCTURE_END

static const uint32_t own_dif_block_sizes[4] = { 512u, 520u, 4096u, 4104u };

/**
 * @brief Determines if an F condition is met for all tags
 */
#define OWN_F_TAGS_DETECTED(dif_ptr, flags) \
    (DML_DIF_FLAG_SRC_F_DETECT_TAGS & (flags) && ((dif_ptr)->application_tag == DML_MAX_16U) && ((dif_ptr)->reference_tag == DML_MAX_32U))

/**
 * @brief Determines if an F condition is met for application tag
 */
#define OWN_F_APPLICATION_TAG_DETECTED(dif_ptr, flags) \
    (DML_DIF_FLAG_SRC_F_DETECT_APP_TAG & dif_flags && ((dif_ptr)->application_tag == DML_MAX_16U))

static inline uint16_t reverse_bytes_16u(uint16_t value)
{
    union
    {
        uint16_t value;
        uint8_t  bytes[2];
    } received_value, reversed_value;

    received_value.value = value;

    reversed_value.bytes[0] = received_value.bytes[1];
    reversed_value.bytes[1] = received_value.bytes[0];

    return reversed_value.value;
}

static inline uint32_t reverse_bytes_32u(uint32_t value)
{
    union
    {
        uint32_t value;
        uint8_t  bytes[4];
    } received_value, reversed_value;

    received_value.value = value;

    reversed_value.bytes[0] = received_value.bytes[3];
    reversed_value.bytes[1] = received_value.bytes[2];
    reversed_value.bytes[2] = received_value.bytes[1];
    reversed_value.bytes[3] = received_value.bytes[0];

    return reversed_value.value;
}

static inline uint16_t calculate_crc_16u(uint16_t crc_value, const uint8_t data, const uint16_t polynomial)
{
    const size_t   byte_width     = 8;
    const size_t   crc_bit_count  = sizeof(crc_value) * byte_width;
    const size_t   crc_byte_shift = crc_bit_count - byte_width;
    const uint16_t high_bit_mask  = 1 << (crc_bit_count - 1);

    crc_value ^= (data << crc_byte_shift);

    for (size_t bit = 0u; bit < byte_width; ++bit)
    {
        crc_value = (crc_value & high_bit_mask) ? ((crc_value << 1) ^ polynomial) : (crc_value << 1);
    }

    return crc_value;
}

static uint16_t crc_16u(const uint8_t *src, const uint32_t transfer_size, uint16_t crc_value, const uint16_t polynomial)
{
    for (size_t byte = 0; byte < transfer_size; ++byte)
    {
        crc_value = calculate_crc_16u(crc_value, src[byte], polynomial);
    }

    return crc_value;
}

int dml_legacy_dif_check(void *dml_job_ptr_)
{
    dml_job_t *dml_job_ptr = (dml_job_t *)dml_job_ptr_;

    // General constants
    const uint32_t dif_flags   = dml_job_ptr->dif_config.flags;
    const uint32_t block_size  = own_dif_block_sizes[dml_job_ptr->dif_config.block_size];
    const uint32_t source_step = block_size + sizeof(own_dif_t);
    const uint32_t block_count = dml_job_ptr->source_length / source_step;

    // Seed calculation properties
    const uint16_t   crc_seed          = (dif_flags & DML_DIF_FLAG_INVERT_CRC_SEED) ? 0xFFFFu : 0u;
    const dml_bool_t invert_crc_result = (dif_flags & DML_DIF_FLAG_INVERT_CRC_RESULT) ? OWN_TRUE : OWN_FALSE;
    // Check properties
    const dml_bool_t check_guard         = (dif_flags & DML_DIF_FLAG_SRC_GUARD_CHECK_DISABLE) ? OWN_FALSE : OWN_TRUE;
    const dml_bool_t check_reference_tag = (dif_flags & DML_DIF_FLAG_SRC_REF_TAG_CHECK_DISABLE) ? OWN_FALSE : OWN_TRUE;

    // Tags update properties
    const uint8_t  ref_tag_update_value = (dif_flags & DML_DIF_FLAG_SRC_FIX_REF_TAG) ? 0u : 1u;
    const uint8_t  app_tag_update_value = (dif_flags & DML_DIF_FLAG_SRC_INC_APP_TAG) ? 1u : 0u;
    const uint16_t application_tag_mask = ~(dml_job_ptr->dif_config.source_application_tag_mask);

    //Variables
    const uint8_t *source_ptr        = dml_job_ptr->source_first_ptr;
    uint16_t       application_tag   = dml_job_ptr->dif_config.source_application_tag_seed;
    uint32_t       reference_tag     = dml_job_ptr->dif_config.source_reference_tag_seed;
    uint32_t       check_accumulator = 0u;

    // Process data
    for (uint32_t block = 0; block < block_count; block++)
    {
        const own_dif_t *const dif_ptr = (own_dif_t *)(source_ptr + block_size);

        // F Detect Condition
        if ((DML_DIF_FLAG_SRC_F_DETECT_ALL & dif_flags) && (DML_DIF_FLAG_SRC_F_ENABLE_ERROR & dif_flags) &&
            (DML_MAX_64U == (*(uint64_t *)dif_ptr)))
        {
            dml_job_ptr->offset = block * source_step;
            dml_job_ptr->result = DML_DIF_CHECK_ALL_BITS_SET_DETECT_ERROR;

            return DML_STATUS_DIF_CHECK_ERROR;
        }

        // Check Fields
        if (!OWN_F_TAGS_DETECTED(dif_ptr, dif_flags) && !OWN_F_APPLICATION_TAG_DETECTED(dif_ptr, dif_flags))
        {
            // Check guard
            if (check_guard)
            {
                uint16_t crc = crc_seed;
                crc          = crc_16u(source_ptr, block_size, crc, OWN_DIF_CRC_POLYNOMIAL);
                crc          = reverse_bytes_16u((invert_crc_result) ? ~crc : crc);

                if (crc != dif_ptr->guard_tag)
                {
                    check_accumulator |= DML_DIF_CHECK_GUARD_MISMATCH;
                }
            }

            // Check reference tag
            if (check_reference_tag && reverse_bytes_32u(reference_tag) != dif_ptr->reference_tag)
            {
                check_accumulator |= DML_DIF_CHECK_REFERENCE_TAG_MISMATCH;
            }

            // Check Application tag
            if (application_tag_mask != DML_MAX_16U &&
                reverse_bytes_16u(application_tag & application_tag_mask) != dif_ptr->application_tag)
            {
                check_accumulator |= DML_DIF_CHECK_APPLICATION_TAG_MISMATCH;
            }
        }

        if (check_accumulator)
        {
            dml_job_ptr->offset = block * source_step;
            dml_job_ptr->result = check_accumulator;

            return DML_STATUS_DIF_CHECK_ERROR;
        }

        // Update variables
        source_ptr += source_step;
        application_tag += app_tag_update_value;
        reference_tag += ref_tag_update_value;
    }

    // All is OK
    dml_job_ptr->result = 0;

    return DML_STATUS_OK;
}

int dml_legacy_dif_insert(void *dml_job_ptr_)
{
    dml_job_t *dml_job_ptr = (dml_job_t *)dml_job_ptr_;

    const uint32_t dif_flags   = dml_job_ptr->dif_config.flags;
    const uint32_t block_size  = own_dif_block_sizes[dml_job_ptr->dif_config.block_size];
    const uint32_t block_count = dml_job_ptr->source_length / block_size;

    // Constants
    const uint32_t   destination_step     = block_size + sizeof(own_dif_t);
    const uint16_t   crc_seed             = (dif_flags & DML_DIF_FLAG_INVERT_CRC_SEED) ? 0xFFFFu : 0u;
    const dml_bool_t invert_crc_result    = (dif_flags & DML_DIF_FLAG_INVERT_CRC_RESULT) ? OWN_TRUE : OWN_FALSE;
    const uint8_t    ref_tag_update_value = (dif_flags & DML_DIF_FLAG_DST_FIX_REF_TAG) ? 0u : 1u;
    const uint8_t    app_tag_update_value = (dif_flags & DML_DIF_FLAG_DST_INC_APP_TAG) ? 1u : 0u;
    const uint16_t   application_tag_mask = ~(dml_job_ptr->dif_config.destination_application_tag_mask);

    //Variables
    const uint8_t *source_ptr      = dml_job_ptr->source_first_ptr;
    uint8_t       *destination_ptr = dml_job_ptr->destination_first_ptr;

    uint16_t application_tag = dml_job_ptr->dif_config.destination_application_tag_seed;
    uint32_t reference_tag   = dml_job_ptr->dif_config.destination_reference_tag_seed;

    // Process data
    for (uint32_t block = 0u; block < block_count; block++)
    {
        // Variables
        own_dif_t *const dif_ptr = (own_dif_t *)(destination_ptr + block_size);
        uint16_t         crc     = crc_seed;

        // Copy
        dml_ref_mem_move(source_ptr, destination_ptr, block_size);

        // Calculate CRC
        crc = crc_16u(destination_ptr, block_size, crc, OWN_DIF_CRC_POLYNOMIAL);

        // Write data integrity field
        dif_ptr->application_tag = reverse_bytes_16u(application_tag & application_tag_mask);
        dif_ptr->reference_tag   = reverse_bytes_32u(reference_tag);
        dif_ptr->guard_tag       = reverse_bytes_16u((invert_crc_result) ? ~crc : crc);

        // Update variables
        application_tag += app_tag_update_value;
        reference_tag += ref_tag_update_value;
        destination_ptr += destination_step;
        source_ptr += block_size;
    }

    return DML_STATUS_OK;
}

int dml_legacy_dif_strip(void *dml_job_ptr_)
{
    dml_job_t *dml_job_ptr = (dml_job_t *)dml_job_ptr_;

    // General constants
    const uint32_t block_size  = own_dif_block_sizes[dml_job_ptr->dif_config.block_size];
    const uint32_t source_step = block_size + sizeof(own_dif_t);
    const uint32_t block_count = dml_job_ptr->source_length / source_step;

    //Variables
    const uint8_t *source_ptr      = dml_job_ptr->source_first_ptr;
    uint8_t       *destination_ptr = dml_job_ptr->destination_first_ptr;

    // Check source data
    int status = dml_legacy_dif_check(dml_job_ptr);
    if (status != DML_STATUS_OK)
    {
        return status;
    }

    // Process data
    for (uint32_t block = 0; block < block_count; block++)
    {
        dml_ref_mem_move(source_ptr, destination_ptr, block_size);

        source_ptr += source_step;
        destination_ptr += block_size;
    }

    return DML_STATUS_OK;
}

int dml_legacy_dif_update(void *dml_job_ptr_)
{
    dml_job_t *dml_job_ptr = (dml_job_t *)dml_job_ptr_;

    // General constants
    const uint32_t dif_flags   = dml_job_ptr->dif_config.flags;
    const uint32_t block_size  = own_dif_block_sizes[dml_job_ptr->dif_config.block_size];
    const uint32_t step        = block_size + sizeof(own_dif_t);
    const uint32_t block_count = dml_job_ptr->source_length / step;

    // Fix or Increment fields
    const uint8_t  ref_tag_update_value = (dif_flags & DML_DIF_FLAG_DST_FIX_REF_TAG) ? 0u : 1u;
    const uint8_t  app_tag_update_value = (dif_flags & DML_DIF_FLAG_DST_INC_APP_TAG) ? 1u : 0u;
    const uint16_t application_tag_mask = ~(dml_job_ptr->dif_config.destination_application_tag_mask);

    // CRC options
    const uint16_t   crc_seed          = (dif_flags & DML_DIF_FLAG_INVERT_CRC_SEED) ? 0xFFFFu : 0u;
    const dml_bool_t invert_crc_result = (dif_flags & DML_DIF_FLAG_INVERT_CRC_RESULT) ? OWN_TRUE : OWN_FALSE;

    // Update options
    const dml_bool_t calculate_crc             = (dif_flags & DML_DIF_FLAG_DST_PASS_GUARD) ? OWN_FALSE : OWN_TRUE;
    const dml_bool_t calculate_application_tag = (dif_flags & DML_DIF_FLAG_DST_PASS_APP_TAG) ? OWN_FALSE : OWN_TRUE;
    const dml_bool_t calculate_reference_tag   = (dif_flags & DML_DIF_FLAG_DST_PASS_REF_TAG) ? OWN_FALSE : OWN_TRUE;

    // Variables
    const uint8_t *source_ptr      = dml_job_ptr->source_first_ptr;
    uint8_t       *destination_ptr = dml_job_ptr->destination_first_ptr;
    uint16_t       application_tag = dml_job_ptr->dif_config.destination_application_tag_seed;
    uint32_t       reference_tag   = dml_job_ptr->dif_config.destination_reference_tag_seed;

    // Check Source
    const int status = dml_legacy_dif_check(dml_job_ptr);
    if (status != DML_STATUS_OK)
    {
        return status;
    }

    // Process Data
    for (uint32_t block = 0u; block < block_count; block++)
    {
        own_dif_t *const destination_dif_ptr = (own_dif_t *)(destination_ptr + block_size);

        dml_ref_mem_move(source_ptr, destination_ptr, step);

        // Update DIF
        if (calculate_crc)
        {
            uint16_t crc                   = crc_seed;
            crc                            = crc_16u(destination_ptr, block_size, crc, OWN_DIF_CRC_POLYNOMIAL);
            destination_dif_ptr->guard_tag = reverse_bytes_16u((invert_crc_result) ? ~crc : crc);
        }

        if (calculate_application_tag)
        {
            destination_dif_ptr->application_tag = reverse_bytes_16u(application_tag & application_tag_mask);
        }

        if (calculate_reference_tag)
        {
            destination_dif_ptr->reference_tag = reverse_bytes_32u(reference_tag);
        }

        // Update Variables
        application_tag += app_tag_update_value;
        reference_tag += ref_tag_update_value;
        source_ptr += step;
        destination_ptr += step;
    }

    return DML_STATUS_OK;
}
