/*
 * Copyright 2020-2021 Intel Corporation.
 *
 * This software and the related documents are Intel copyrighted materials,
 * and your use of them is governed by the express license under which they
 * were provided to you ("License"). Unless the License provides otherwise,
 * you may not use, modify, copy, publish, distribute, disclose or transmit
 * this software or the related documents without Intel's prior written
 * permission.
 *
 * This software and the related documents are provided as is, with no
 * express or implied warranties, other than those that are expressly
 * stated in the License.
 *
 */

/**
 * @details Contain implementation for Delta Record feature:
 *       - @ref dmlc_create_delta_record_8u
 *
 * @date 2/17/2020
 *
 */

#include "core_compare.h"
#include "own_dmlc_definitions.h"


/**
 * @defgroup own_delta_record Delta record own API
 * @ingroup core_own
 * @{
 * @brief Delta record own definitions and functions
 *
 */

/* ------ DELTA RECORD INTERNAL DEFINITIONS ------ */

typedef pattern_t region_t;            /**< Redefinition for the @ref pattern_t type        */
typedef uint16_t  offset_t;            /**< Redefinition to make more abstract offset type  */

/* Delta note sizes  */
#define DELTA_NOTE_OFFSET_FIELD_SIZE  sizeof(offset_t)    /**< Size of offset field in delta note */
#define DELTA_NOTE_REGION_FIELD_SIZE  sizeof(region_t)    /**< Size of delta field in delta note */
#define DELTA_NOTE_SIZE (DELTA_NOTE_OFFSET_FIELD_SIZE + DELTA_NOTE_REGION_FIELD_SIZE) /**< Delta note size*/

/* Checkers */

/**< Input pointer must be aligned to 8 bytes */
#define OWN_DELTA_CHECK_PTR_ALIGNMENT(ptr) \
    DML_CORE_BAD_ARGUMENT_RETURN((((uint64_t)ptr) % 8u), DML_STATUS_DELTA_ALIGN_ERROR)

#define MAX_AVAILABLE_INPUT_SIZE  0x80000u                 /**< Input vector size limit*/

#if defined(__GNUC__)
    typedef struct __attribute__ ((__packed__))
#elif(_MSC_VER)
    #pragma pack(2)
    typedef struct
#else
    #error Compiler not supported
#endif
{
    offset_t offset;          /**< Offset of mismatched region, which delta written in delta field*/
    region_t reference_data;  /**< Contain delta between standard vector and compared*/
} own_delta_note_t;           /**< Presents a single element of Delta Record stream */

/** @} */

/* ------ DELTA RECORD PUBLIC FUNCTIONS IMPLEMENTATION ------ */

DML_CORE_API(dmlc_status_t, create_delta_record_8u, (const uint8_t *reference_vector_ptr,
                                                     const uint8_t *second_vector_ptr,
                                                     const uint32_t compared_bytes,
                                                     const uint32_t delta_record_max_size,
                                                     uint8_t* delta_record_ptr,
                                                     uint32_t *const record_size_ptr))
{
    DML_CORE_CHECK_NULL_POINTER(reference_vector_ptr)
    DML_CORE_CHECK_NULL_POINTER(second_vector_ptr)
    DML_CORE_CHECK_NULL_POINTER(delta_record_ptr)
    DML_CORE_CHECK_NULL_POINTER(record_size_ptr)
    OWN_DELTA_CHECK_PTR_ALIGNMENT(reference_vector_ptr)
    OWN_DELTA_CHECK_PTR_ALIGNMENT(second_vector_ptr)

    (*record_size_ptr) = 0u;

    DML_CORE_CHECK_INPUT_SIZE(compared_bytes % DELTA_NOTE_REGION_FIELD_SIZE, DML_STATUS_DELTA_ALIGN_ERROR)
    DML_CORE_CHECK_INPUT_SIZE(compared_bytes > MAX_AVAILABLE_INPUT_SIZE, DML_STATUS_DELTA_OFFSET_ERROR)
    DML_CORE_CHECK_OUTPUT_SIZE(delta_record_max_size % DELTA_NOTE_SIZE, DML_STATUS_DELTA_INPUT_SIZE_ERROR)
    DML_CORE_CHECK_OUTPUT_SIZE(0u == delta_record_max_size, DML_STATUS_DELTA_INPUT_SIZE_ERROR)

    // Delta Record
    const uint32_t delta_note_count           = delta_record_max_size / DELTA_NOTE_SIZE;
    const uint32_t regions_count              = (uint32_t)(compared_bytes / DELTA_NOTE_REGION_FIELD_SIZE);
    own_delta_note_t* current_delta_notes_ptr = (own_delta_note_t*) delta_record_ptr;
    own_delta_note_t* end_delta_notes_ptr     = current_delta_notes_ptr + delta_note_count;

    // Create delta
    for (uint32_t i = 0u; i < regions_count; i++)
    {
        const uint64_t base_region   = *(uint64_t *) reference_vector_ptr;
        const uint64_t vector_region = *(uint64_t *) second_vector_ptr;

        reference_vector_ptr += DELTA_NOTE_REGION_FIELD_SIZE;
        second_vector_ptr    += DELTA_NOTE_REGION_FIELD_SIZE;

        // Write delta note into delta record in case:
        if (base_region != vector_region)
        {
            if(current_delta_notes_ptr < end_delta_notes_ptr)
            {
                current_delta_notes_ptr->reference_data = base_region;
                current_delta_notes_ptr->offset         = i;
                current_delta_notes_ptr++;
                (*record_size_ptr) += DELTA_NOTE_SIZE;
            }
            else
            {
                return DML_STATUS_DELTA_RECORD_SIZE_ERROR;
            }
        }
    }

    return DML_STATUS_OK;
}


DML_CORE_API(dmlc_status_t, apply_delta_record_8u, (uint8_t * memory_region_ptr,
                                                   const uint8_t  *delta_record_ptr,
                                                   const uint32_t memory_region_size,
                                                   const uint32_t delta_record_size))
{
    DML_CORE_CHECK_NULL_POINTER(memory_region_ptr)
    DML_CORE_CHECK_NULL_POINTER(delta_record_ptr)
    OWN_DELTA_CHECK_PTR_ALIGNMENT(memory_region_ptr)
    DML_CORE_CHECK_INPUT_SIZE(memory_region_size > MAX_AVAILABLE_INPUT_SIZE, DML_STATUS_DELTA_INPUT_SIZE_ERROR)
    DML_CORE_CHECK_INPUT_SIZE(memory_region_size % DELTA_NOTE_REGION_FIELD_SIZE, DML_STATUS_DELTA_ALIGN_ERROR)
    DML_CORE_CHECK_INPUT_SIZE(delta_record_size % DELTA_NOTE_SIZE, DML_STATUS_DELTA_RECORD_SIZE_ERROR)
    DML_CORE_CHECK_OVERLAPPING_FORWARD(delta_record_ptr, memory_region_ptr, memory_region_size)
    DML_CORE_CHECK_OVERLAPPING_FORWARD(memory_region_ptr, delta_record_ptr, delta_record_size)

    // Constants
    const uint32_t delta_notes_count = delta_record_size / DELTA_NOTE_SIZE;

    // Variables
    own_delta_note_t *delta_note_ptr = (own_delta_note_t *) delta_record_ptr;
    region_t *regions_ptr            = (region_t *) memory_region_ptr;

    for (uint32_t i = 0u; i < delta_notes_count; i++)
    {
        const offset_t region_offset  = delta_note_ptr[i].offset;
        const region_t reference_data = delta_note_ptr[i].reference_data;

        if (region_offset < memory_region_size)
        {
            regions_ptr[region_offset] = reference_data;
        }
        else
        {
            return DML_STATUS_MEMORY_OVERFLOW_ERROR;
        }
    }

    return DML_STATUS_OK;
}
