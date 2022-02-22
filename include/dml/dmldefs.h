/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

/**
 * @file
 *
 * @defgroup dml_job_api DML Job API
 * @{
 * @brief Intel(R) Data Mover Library (Intel® DML) user driver interface.
 *
 * @defgroup dml_job_public Public API
 * @ingroup dml_job_api
 * @{
 *
 * @defgroup dml_job_public_definitions Public Definitions
 * @ingroup dml_job_public
 * @{
 * @brief Common Types and Macro Definitions for Intel(R) Data Mover Library (Intel® DML).
 *
 */

#include <stdint.h>

#ifndef DML_DEFS_H__
#define DML_DEFS_H__

#ifdef __cplusplus
extern "C" {
#endif

/* ################# DML DEFINITIONS ################# */

/* ====== DML Defines ====== */

#define DML_MAX_8U     ( 0xFFu )                    /**< Maximal value presented by unsigned 8-bit integer */
#define DML_MIN_8U     ( 0u )                       /**< Unsigned 0 presented by 8-bit */
#define DML_MAX_16U    ( 0xFFFFu )                  /**< Maximal value presented by unsigned 16-bit integer */
#define DML_MIN_16U    ( 0u )                       /**< Unsigned 0 presented by 16-bit */
#define DML_MAX_32U    ( 0xFFFFFFFFu )              /**< Maximal value presented by unsigned 32-bit integer */
#define DML_MIN_32U    ( 0u )                       /**< Unsigned 0 presented by 32-bit */

#if defined( _WIN32 ) || defined ( _WIN64 )
#define DML_MAX_64U  ( 0xFFFFFFFFFFFFFFFFL )        /**< Maximal value presented by unsigned 64bit integer */
#else
#define DML_MAX_64U  ( 0xFFFFFFFFFFFFFFFFLL )       /**< Maximal value presented by unsigned 64-bit integer */
#endif

#define DML_MIN_64U    ( 0u )                       /**< Unsigned 0 presented by 64-bit */
#define DML_ONE_64U  ( 1ULL )                       /**< Unsigned 1 presented by 64-bit */

#define DML_BASE_DRIVER_ERROR 100u                  /**< Base for all driver errors  */

/* ====== DML Macros ====== */
#define DML_MAX(a, b) ( ((a) > (b)) ? (a) : (b) )   /**< Simple macro to find maximum between pair values */
#define DML_MIN(a, b) ( ((a) < (b)) ? (a) : (b) )   /**< Simple macro to find minimum between pair values */


/* ====== DML Overrides ====== */
typedef uint64_t dml_operation_flags_t;           /**< Redefinition of uint64_t as dml_operation_flags_t   */
typedef uint32_t dml_meta_result_t;               /**< Redefinition of uint32_t as dml_meta_result_t       */
typedef uint8_t dml_internal_data_t;              /**< Hidden internal structure                           */

/* #################  CONTROL FLAGS  ################# */

/**
 * @name Traffic Class Flags
 * @anchor Traffic_Class_Flags
 * @brief This enumeration is used to set a traffic class that is used on the target platform.
 *
 * @brief Must be set as argument of the @ref dml_job_t.flags function
 * @{
 */
/**
 * @brief For @ref dml_operation_t.DML_OP_BATCH sets TC-B for reading the descriptor list
 * @brief For @ref dml_operation_t.DML_OP_COMPARE and @ref dml_operation_t.DML_OP_DELTA_CREATE
 *        sets TC-B for reads from @ref dml_job_t.source_first_ptr
 * @brief For @ref dml_operation_t.DML_OP_DRAIN sets TC-B value used for readback from Readback Address 1
 * @brief For @ref dml_operation_t.DML_OP_DELTA_APPLY sets TC-B for reads from Delta Record address
 * @brief For other operations selects TC-B for reads from the source address
 */
#define DML_FLAG_ADDRESS1_TCB  0x200u
/**
 * @brief For @ref dml_operation_t.DML_OP_DUALCAST sets TC-B for writes to the @ref dml_job_t.destination_first_ptr
 * @brief For @ref dml_operation_t.DML_OP_COMPARE and @ref dml_operation_t.DML_OP_DELTA_CREATE sets TC-B for
 *        reads from the @ref dml_job_t.source_second_ptr
 * @brief For @ref dml_operation_t.DML_OP_DRAIN sets TC-B value used for readback from Readback Address 2
 * @brief For other operations selects TC-B for writes to the destination address
 */
#define DML_FLAG_ADDRESS2_TCB  0x400u
/**
 * @brief For @ref dml_operation_t.DML_OP_DUALCAST indicates sets TC-B for writes
 *        to the @ref dml_job_t.destination_second_ptr
 * @brief For @ref dml_operation_t.DML_OP_CRC and @ref dml_operation_t.DML_OP_COPY_CRC sets TC-B for reads
 *        from the @ref dml_job_t.crc_checksum_ptr
 * @brief For @ref dml_operation_t.DML_OP_DELTA_CREATE sets TC-B for writes to the Delta Record address
 */
#define DML_FLAG_ADDRESS3_TCB  0x800u
/** @}*/

/**
 * @name Memory Type Flags
 * @anchor Memory_Type_Flags
 * @brief This enumeration is used to set a memory type that are used on the target platform.
 *
 * @brief Must be set as argument of the @ref dml_job_t.flags function
 * @{
 */
#define DML_FLAG_DST1_DURABLE 0x8000u /**< Writes to the first destination are identified as writes to durable memory */
/** @}*/

/**
 * @name General Operations Flags
 * @anchor CommonFlags
 * @brief This enumeration is used to indicate various options when submitting a job.
 *
 * @brief Must be set into @ref dml_job_t.flags
 * @{
 */
/* ======  The Flow control flags  ====== */
#define DML_FLAG_BATCH                    0x00000001u /**< The Batch operation queues multiple jobs at once. This flag indicates the first and the next jobs for the batch */
#define DML_FLAG_BATCH_LAST               0x00000002u /**< The last job in the Batch operation */
#define DML_FLAG_FENCE                    0x00000004u /**< Fence flag */
#define DML_FLAG_CHECK_RESULT             0x00000080u /**< Check Result flag */

/* ====== The operation specific flags  ====== */
// DML_OP_MEM_MOVE operation specific flag
#define DML_FLAG_COPY_ONLY                  0x00000020u /**< The move operation is disabled */

// DML_OP_CACHE_FLUSH operation specific flag
#define DML_FLAG_DONT_INVALIDATE_CACHE      0x00000100u /**< Cache control flag for Flush operation */
#define DML_FLAG_PREFETCH_CACHE             0x00000100u /**< Prefetch result of the operation into LLC */

// DML_OP_CRC operation specific flags
#define DML_FLAG_CRC_READ_SEED              0x10000u /**< Use value saved in crc_checksum_ptr as seed                         */
#define DML_FLAG_CRC_BYPASS_REFLECTION      0x20000u /**< Do not use reverse bit order and reflection for CRC seed and result */
#define DML_FLAG_CRC_BYPASS_DATA_REFLECTION 0x40000u /**< Bit 7 of each data byte is the MSB in the CRC computation           */

// DML_OP_DUALCAST operation specific flags
#define DML_FLAG_DUALCAST_DST2_DURABLE      0x10000u /**< Writes to the second destination are identified as writes to durable memory */
/** @} */

/**
 * @name Expected Result Flags
 * @anchor Expected_Result_Flags
 * @brief This enumeration is used to indicate expected result for some operations when submitting a job.
 *
 * @brief Must be set into @ref dml_job_t.expected_result
 * @{
 */
#define DML_FLAG_EXPECT_EQUAL     0x00000001u /**< Expected equal data */
#define DML_FLAG_EXPECT_NOT_EQUAL 0x00000002u /**< Expected not equal data */
#define DML_FLAG_EXPECT_OVERFLOW  0x00000004u /**< Expected the output buffer overflow for the DML_OP_DELTA_CREATE */

/** @} */

/**
 * @name DIF Operations Flags
 * @anchor DIF_Flags
 * @brief This enumeration is used to indicate various options when submitting a job with diff operation.
 *
 * @details Must be set into @ref dml_dif_config_t.flags
 * @{
 */
/* ====== GENERAL DIF FLAGS ====== */
#define DML_DIF_FLAG_INVERT_CRC_SEED            0x040000u  /**< The initial crc seed is 0xFFFF */
#define DML_DIF_FLAG_INVERT_CRC_RESULT          0x080000u  /**< Inverts CRC result */

/* ====== SOURCE DIF FLAGS ====== */
#define DML_DIF_FLAG_SRC_F_ENABLE_ERROR         0x000001u  /**< Throw an error on condition set by the DML_DIF_FLAG_SRC_F_DETECT_ALL flag */
#define DML_DIF_FLAG_SRC_F_DETECT_ALL           0x000002u  /**< Detect if all DIF bytes is equal to 0xFF in the source */
#define DML_DIF_FLAG_SRC_F_DETECT_APP_TAG       0x000004u  /**< Skip check of protected block if Application tag is equal to 0xFF on the source */
#define DML_DIF_FLAG_SRC_F_DETECT_TAGS          0x000008u  /**< Skip check of protected block if Application and Reference tag bytes is equal to 0xFF in the source */
#define DML_DIF_FLAG_SRC_INC_APP_TAG            0x000010u  /**< Source Application format:   Fixed(0) or Incrementing(1) */
#define DML_DIF_FLAG_SRC_GUARD_CHECK_DISABLE    0x000020u  /**< Source Check Guard filed:    Enabled (0) or Disabled (1) */
#define DML_DIF_FLAG_SRC_REF_TAG_CHECK_DISABLE  0x000040u  /**< Source Check Reference tag:  Enabled (0) or Disabled (1) */
#define DML_DIF_FLAG_SRC_FIX_REF_TAG            0x000080u  /**< Source Reference TAG format: Incrementing(0), Fixed(1)   */

/* ====== DESTINATION DIF FLAGS ====== */
#define DML_DIF_FLAG_DST_RESERVED1              0x000100u  /**< Reserved destination flag must be set to zero */
#define DML_DIF_FLAG_DST_RESERVED2              0x000200u  /**< Reserved destination flag must be set to zero */
#define DML_DIF_FLAG_DST_RESERVED3              0x000400u  /**< Reserved destination flag must be set to zero */
#define DML_DIF_FLAG_DST_PASS_APP_TAG           0x000800u  /**< Destination Application calculation: Enabled(0) or Disabled (1 Copy from Source) */
#define DML_DIF_FLAG_DST_INC_APP_TAG            0x001000u  /**< Destination Application tag format:  Fixed(0), Incrementing(1) */
#define DML_DIF_FLAG_DST_PASS_GUARD             0x002000u  /**< Destination Guard calculation:       Enabled(0) or Disabled (1 Copy from Source) */
#define DML_DIF_FLAG_DST_PASS_REF_TAG           0x004000u  /**< Destination Reference calculation:   Enabled(0) or Disabled (1 Copy from Source) */
#define DML_DIF_FLAG_DST_FIX_REF_TAG            0x008000u  /**< Destination Reference tag format:    Incrementing(0) or Fixed(1) */
/** @} */

/**
 * @name DIF Check Error Flags
 * @anchor DIF_Check_Errors
 * @brief This enumeration is used to indicate various type of errors when @ref DML_OP_DIF_CHECK
 * operation completed with the @ref DML_STATUS_DIF_CHECK_ERROR status.
 *
 * @details This information provided in the @ref dml_job_t.result field
 * @{
 */
#define DML_DIF_CHECK_GUARD_MISMATCH             0x01u /**< Data block CRC checksum is not correct              */
#define DML_DIF_CHECK_APPLICATION_TAG_MISMATCH   0x02u /**< Data block Application tag is not correct           */
#define DML_DIF_CHECK_REFERENCE_TAG_MISMATCH     0x04u /**< Data block Reference tag is not correct             */
#define DML_DIF_CHECK_ALL_BITS_SET_DETECT_ERROR  0x08u /**< Hit is detected on the DML_DIF_FLAG_SRC_F condition */
/** @} */


/* #################  ENUMERATIONS  ################# */

/**
 * @brief Enum for execution path selection.
 */
typedef enum
{
    DML_PATH_AUTO = 0x00000000u, /**< Enable auto-dispatching of the execution path */
    DML_PATH_SW   = 0x00000001u, /**< Only software path of DML will be used        */
    DML_PATH_HW   = 0x00000002u  /**< Only hardware path of DML will be used        */
} dml_path_t;


/**
 * @brief Describes the size of the data blocks in the stream that is protected by DIF
 */
typedef enum
{
    DML_DIF_BLOCK_SIZE_512,     /**< Size is 512 bytes  */
    DML_DIF_BLOCK_SIZE_520,     /**< Size is 520 bytes  */
    DML_DIF_BLOCK_SIZE_4096,    /**< Size is 4096 bytes */
    DML_DIF_BLOCK_SIZE_4104     /**< Size is 4104 bytes */
} dml_dif_block_size_t;


/**
 * @brief Enum of all supported operations.
 */
typedef enum
{
    DML_OP_NOP             = 0x00u,      /**< No operation                          */
    DML_OP_BATCH           = 0x01u,      /**< Batch operation                       */
    DML_OP_DRAIN           = 0x02u,      /**< Drain operation                       */
    DML_OP_MEM_MOVE        = 0x03u,      /**< Memory move operation                 */
    DML_OP_FILL            = 0x04u,      /**< Memory fill operation                 */
    DML_OP_COMPARE         = 0x05u,      /**< Memory compare operation              */
    DML_OP_COMPARE_PATTERN = 0x06u,      /**< Memory compare with pattern operation */
    DML_OP_DELTA_CREATE    = 0x07u,      /**< Create delta record operation         */
    DML_OP_DELTA_APPLY     = 0x08u,      /**< Apply delta record operation          */
    DML_OP_DUALCAST        = 0x09u,      /**< Memory copy with dualcast operation   */
    DML_OP_CRC             = 0x10u,      /**< Generate CRC operation                */
    DML_OP_COPY_CRC        = 0x11u,      /**< Copy with CRC operation               */
    DML_OP_DIF_CHECK       = 0x12u,      /**< Dif check operation                   */
    DML_OP_DIF_INSERT      = 0x13u,      /**< Dif insert operation                  */
    DML_OP_DIF_STRIP       = 0x14u,      /**< Dif strip operation                   */
    DML_OP_DIF_UPDATE      = 0x15u,      /**< Dif update operation                  */
    DML_OP_CACHE_FLUSH     = 0x20u       /**< Cache flush operation                 */
} dml_operation_t;


/**
 * @brief All possible return values of the Intel DML Library functions.
 *
 * @note All general statuses are described here.
 */
typedef enum
{
    // General Statuses
    DML_STATUS_OK                           = 0u,   /**< Operation completed successfully. */
    DML_STATUS_FALSE_PREDICATE_OK           = 1u,   /**< Success with false predicate. */
    DML_STATUS_BEING_PROCESSED              = 2u,   /**< Operation is still being processed. */
    DML_STATUS_INTERNAL_ERROR               = 3u,   /**< Unexpected internal error condition. */
    DML_STATUS_NULL_POINTER_ERROR           = 4u,   /**< Null pointer error. */
    DML_STATUS_LIMITS_ERROR                 = 5u,   /**< One of the job parameters exceeds configured DML limits. */
    DML_STATUS_PATH_ERROR                   = 6u,   /**< Invalid dmlPath parameter. */
    DML_STATUS_HINT_ERROR                   = 7u,   /**< Invalid hint parameter. */
    DML_STATUS_CRC_ALIGN_ERROR              = 8u,   /**< crc_checksum_ptr must be 4-byte aligned */
    DML_STATUS_BATCH_ERROR                  = 9u,   /**< One or more operation in the batch completed with status > 0. */
    DML_STATUS_DELTA_ASCENDT_ERROR          = 10u,  /**< Offsets in the delta record were not in increasing order. */
    DML_STATUS_DELTA_OFFSET_ERROR           = 11u,  /**< The input length is greater than max available offset */
    DML_STATUS_DIF_CHECK_ERROR              = 12u,  /**< DIF check failed. */
    DML_STATUS_JOB_OPERATION_ERROR          = 13u,  /**< Invalid op field in dml_job_t. */
    DML_STATUS_JOB_FLAGS_ERROR              = 14u,  /**< Invalid flags field in dml_job_t. */
    DML_STATUS_JOB_LENGTH_ERROR             = 15u,  /**< Invalid length field in dml_job_t. */
    DML_STATUS_BATCH_LIMITS_ERROR           = 16u,  /**< Invalid number of Jobs for batch operation (LT than 2 or GT than dml_limits_t.dmlMaxBatchSize) */
    DML_STATUS_DELTA_RECORD_SIZE_ERROR      = 17u,  /**< Delta Record Size is out of range or delta record size is not multiple of 10. */
    DML_STATUS_OVERLAPPING_BUFFER_ERROR     = 18u,  /**< Overlapping buffers. */
    DML_STATUS_DUALCAST_ALIGN_ERROR         = 19u,  /**< Bit 11:0 of pDst1 and pDst2 differ in Memory Copy with Dualcast operation. */
    DML_STATUS_DELTA_ALIGN_ERROR            = 20u,  /**< Misaligned address for Delta Record: pSrc1, pSrc2, pDst1 or length is not 8 - byte aligned. */
    DML_STATUS_DELTA_INPUT_SIZE_ERROR       = 21u,  /**< Input size is not multiple of 8 for delta operations. */
    DML_STATUS_MEMORY_OVERFLOW_ERROR        = 22u,  /**< Buffer detected an overflow. */
    DML_STATUS_JOB_CORRUPTED                = 23u,  /**< @ref dml_job_t structure is corrupted */
    DML_STATUS_WORK_QUEUE_OVERFLOW_ERROR    = 24u,  /**< WQ overflow. */
    DML_STATUS_PAGE_FAULT_ERROR             = 25u,  /**< Page Fault occurred during processing on hardware */
    dml_status_tC_A_NOT_AVAILABLE           = 26u,  /**< WQs configured to work with TC-A are not visible */
    dml_status_tC_B_NOT_AVAILABLE           = 27u,  /**< WQs configured to work with TC-B are not visible */
    DML_STATUS_BATCH_TASK_INDEX_OVERFLOW    = 28u,  /**< Batch task index is bigger than size of the batch */
    DML_STATUS_BATCH_SIZE_ERROR             = 29u,  /**< The desired batch size is bigger than the possible one */
    DML_STATUS_DRAIN_PAGE_FAULT_ERROR       = 30u,  /**< A page fault occured while translating a Readback Addres in a Drain descriptor */
    DML_STATUS_UNKNOWN_CACHE_SIZE_ERROR     = 31u,  /**< Max cache size can't be calculated */
    DML_STATUS_DIF_STRIP_ADJACENT_ERROR     = 32u,  /**< SRC Address for DIF Strip operation should be greater than (DST Address + SRC Size) */

    // Initialization Errors
    DML_STATUS_LIBACCEL_NOT_FOUND           = (DML_BASE_DRIVER_ERROR + 0u),  /**< Unable to initialize job because hardware driver was not found */
    DML_STATUS_LIBACCEL_ERROR               = (DML_BASE_DRIVER_ERROR + 1u),  /**< Unable to initialize job because hardware driver API is incompatible */
    DML_STATUS_WORK_QUEUES_NOT_AVAILABLE    = (DML_BASE_DRIVER_ERROR + 2u),  /**< Enabled work queues are not found */
} dml_status_t;


/*  ################# DML General Structures  ################# */


/**
 * @brief Contains information about the current build.
 */
typedef struct
{
    uint8_t  name[30];               /**< Name of the library            */
    uint8_t  minimal_cpu_isa[16];    /**< Minimal supported CPU ISA      */
    uint8_t  date[15];               /**< Build date                     */
    uint8_t  version[20];            /**< Version of the library         */
    uint8_t  hw_minimal_version[30]; /**< Minimal supported HW version   */
    uint8_t  commit[12];             /**< Commit hash                    */
    uint32_t major;                  /**< Major version                  */
    uint32_t minor;                  /**< Minor version                  */
    uint32_t patch;                  /**< Patch version                  */
} dml_library_version_t;



/**
 * @brief Describes the library limitations.
 */
typedef struct
{
    uint32_t dml_max_transfer_Size;       /**< WQ Maximum Transfer Size     */
    uint32_t dml_max_batch_size;          /**< WQ Maximum Batch Size        */
    uint32_t dml_max_delta_record_size;   /**< WQ Maximum Delta Record Size */
} dml_limits_t;


/**
 * @brief Contains a properties for Data Integrity Field(DIF) features configuration.
 */
typedef struct
{
    uint32_t flags;                              /**< Specific DIF feature flags           */
    uint32_t source_reference_tag_seed;          /**< Reference tag seed for source        */
    uint16_t source_application_tag_seed;        /**< Application tag seed for source      */
    uint16_t source_application_tag_mask;        /**< Application tag mask for source      */
    uint32_t destination_reference_tag_seed;     /**< Reference tag seed for destination   */
    uint16_t destination_application_tag_seed;   /**< Application tag seed for destination */
    uint16_t destination_application_tag_mask;   /**< Application tag mask for destination */
    dml_dif_block_size_t block_size;             /**< Size of data blocks in the stream    */
} dml_dif_config_t;


/**
 * @brief Primary structure in the DML Job API to specify DML operation and this context.
 */
typedef struct
{
    uint8_t               *source_first_ptr;       /**< Pointer to source data 1                                    */
    uint8_t               *source_second_ptr;      /**< Pointer to source data 2                                    */
    uint8_t               *destination_first_ptr;  /**< Pointer to destination data 1                               */
    uint8_t               *destination_second_ptr; /**< Pointer to destination data 2                               */
    uint32_t              *crc_checksum_ptr;       /**< CRC - Input and Output                                      */
    uint32_t               source_length;          /**< Number of bytes in source 1 to proceed                      */
    uint32_t               destination_length;     /**< Available bytes count in destination buffer                 */
    uint32_t               offset;                 /**< Count of successfully processed bytes                       */
    uint8_t                pattern[8];             /**< Pattern for "Compare Pattern" operation                     */
    dml_operation_t        operation;              /**< DML operation                                               */
    dml_meta_result_t      result;                 /**< Result field for some operations                            */
    dml_meta_result_t      expected_result;        /**< Expected result for some operations                         */
    dml_operation_flags_t  flags;                  /**< Auxiliary DML operation flags - see below                   */
    dml_dif_config_t       dif_config;             /**< Properties for DIF operations                               */
    uint32_t               numa_id;                /**< NUMA node id for submission                                 */
    dml_internal_data_t   *internal_data_ptr;      /**< Internal memory buffers & structures for all DML operations */
} dml_job_t;


#ifdef __cplusplus
}
#endif

#endif /* DML_DEFS_H__ */

/**
 * @} Close DML Job APIPublic Definitions
 * @} Close DML Job API Public API group
 * @} Close DML Job API group
 */
