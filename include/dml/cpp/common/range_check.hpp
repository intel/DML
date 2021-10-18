/*
 * Copyright 2021 Intel Corporation.
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

#ifndef DML_COMMON_RANGE_CHECK_HPP
#define DML_COMMON_RANGE_CHECK_HPP

/**
 * @date 05/19/2021
 * @brief Contains definitions for range checks for operations
 */

#include <dml/cpp/common/status_code.hpp>
#include <dml/cpp/common/types.hpp>

#include <cstdint>

namespace dml
{
    /**
     * @defgroup dml_cpp_common_checks Range checks
     * @ingroup dml_cpp_common
     * @brief Contains range checks descriptions
     * @{
     */

    /**
     * @brief Container for functions, which do range-checks
     */
    struct range_check
    {
        /**
         * @brief Performs checks on input parameters for @ref mem_move_operation
         *
         * @param src     Pointer to the source memory region
         * @param dst     Pointer to the destination memory region
         * @param size    Byte size of the memory regions
         *
         * @return
         *      - @ref status_code::ok
         *      - @ref status_code::bad_size
         *      - @ref status_code::nullptr_error
         */
        static status_code mem_move(const byte_t *const src, const byte_t *const dst, const size_t size) noexcept
        {
            if (src == nullptr || dst == nullptr)
            {
                return status_code::nullptr_error;
            }
            else if (size == 0u)
            {
                return status_code::bad_size;
            }
            else
            {
                return status_code::ok;
            }
        }

        /**
         * @brief Performs checks on input parameters for @ref mem_copy_operation
         *
         * @param src     Pointer to the source memory region
         * @param dst     Pointer to the destination memory region
         * @param size    Byte size of the memory regions
         *
         * @return
         *      - @ref status_code::ok
         *      - @ref status_code::bad_size
         *      - @ref status_code::nullptr_error
         *      - @ref status_code::buffers_overlapping
         */
        static status_code mem_copy(const byte_t *const src, const byte_t *const dst, const size_t size) noexcept
        {
            if (src == nullptr || dst == nullptr)
            {
                return status_code::nullptr_error;
            }
            else if (size == 0u)
            {
                return status_code::bad_size;
            }
            else if ((src <= dst) && ((src + size) > dst))
            {
                return status_code::buffers_overlapping;
            }
            else if ((dst <= src) && ((dst + size) > src))
            {
                return status_code::buffers_overlapping;
            }
            else
            {
                return status_code::ok;
            }
        }

        /**
         * @brief Performs checks on input parameters for @ref fill_operation
         *
         * @param dst   Pointer to the destination memory region
         * @param size  Byte size of the memory region
         *
         * @return
         *      - @ref status_code::ok
         *      - @ref status_code::nullptr_error
         *      - @ref status_code::bad_size
         */
        static status_code fill(const byte_t *const dst, const size_t size) noexcept
        {
            if (dst == nullptr)
            {
                return status_code::nullptr_error;
            }
            else if (size == 0u)
            {
                return status_code::bad_size;
            }
            else
            {
                return status_code::ok;
            }
        }

        /**
         * @brief Performs checks on input parameters for @ref dualcast_operation
         *
         * @param src   Pointer to the source memory region
         * @param dst1  Pointer to the first destination memory region
         * @param dst2  Pointer to the second destination memory region
         * @param size  Byte size of the memory regions
         *
         * @return
         *      - @ref status_code::ok;
         *      - @ref status_code::nullptr_error
         *      - @ref status_code::bad_size
         *      - @ref status_code::dualcast_bad_padding
         */
        static status_code dualcast(const byte_t *const src,
                                    const byte_t *const dst1,
                                    const byte_t *const dst2,
                                    const size_t        size) noexcept
        {
            if (src == nullptr || dst1 == nullptr || dst2 == nullptr)
            {
                return status_code::nullptr_error;
            }
            else if (size == 0u)
            {
                return status_code::bad_size;
            }
            else if ((std::uintptr_t(dst1) & 0xFFFu) != (uint64_t(dst2) & 0xFFFu))
            {
                return status_code::dualcast_bad_padding;
            }
            else if ((src < (dst1 + size)) && (dst1 < (src + size)))
            {
                return status_code::buffers_overlapping;
            }
            else if ((src < (dst2 + size)) && (dst2 < (src + size)))
            {
                return status_code::buffers_overlapping;
            }
            else if ((dst1 < (dst2 + size)) && (dst2 < (dst1 + size)))
            {
                return status_code::buffers_overlapping;
            }
            else
            {
                return status_code::ok;
            }
        }

        /**
         * @brief Performs checks on input parameters for @ref compare_operation
         *
         * @param src1  Pointer to the first source memory region
         * @param src2  Pointer to the second source memory region
         * @param size  Byte size of the memory regions
         *
         * @return
         *      - @ref status_code::ok
         *      - @ref status_code::nullptr_error
         *      - @ref status_code::bad_size
         */
        static status_code compare(const byte_t *const src1, const byte_t *const src2, const size_t size) noexcept
        {
            if (src1 == nullptr || src2 == nullptr)
            {
                return status_code::nullptr_error;
            }
            else if (size == 0u)
            {
                return status_code::bad_size;
            }
            else
            {
                return status_code::ok;
            }
        }

        /**
         * @brief Performs checks on input parameters for @ref compare_pattern_operation
         *
         * @param src   Pointer to the source memory region
         * @param size  Byte size of the memory region
         *
         * @return
         *      - @ref status_code::ok
         *      - @ref status_code::nullptr_error
         *      - @ref status_code::bad_size
         */
        static status_code compare_pattern(const byte_t *const src, const size_t size) noexcept
        {
            if (src == nullptr)
            {
                return status_code::nullptr_error;
            }
            else if (size == 0u)
            {
                return status_code::bad_size;
            }
            else
            {
                return status_code::ok;
            }
        }

        /**
         * @brief Performs checks on input parameters for @ref create_delta_operation
         *
         * @param src1            Pointer to the first source memory region
         * @param src2            Pointer to the second source memory region
         * @param size            Byte size of the memory region
         * @param delta_record    Pointer to the memory region for delta record
         * @param max_delta_size  Byte size of the delta record memory region
         *
         * @return
         *      - @ref status_code::ok
         *      - @ref status_code::nullptr_error
         *      - @ref status_code::bad_size
         *      - @ref status_code::bad_alignment
         *      - @ref status_code::delta_bad_size
         */
        static status_code create_delta(const byte_t *const src1,
                                        const byte_t *      src2,
                                        const size_t        size,
                                        const byte_t *      delta_record,
                                        const size_t        max_delta_size) noexcept
        {
            // TODO: Add more checks
            if (src1 == nullptr || src2 == nullptr || delta_record == nullptr)
            {
                return status_code::nullptr_error;
            }
            else if (size == 0u || size % 8 != 0)
            {
                return status_code::bad_size;
            }
            else if (reinterpret_cast<std::uintptr_t>(src1) % 8 != 0 ||
                     reinterpret_cast<std::uintptr_t>(src2) % 8 != 0)
            {
                return status_code::bad_alignment;
            }
            else if (max_delta_size % 10 != 0 || max_delta_size < 80)
            {
                return status_code::delta_bad_size;
            }
            else
            {
                return status_code::ok;
            }
        }

        /**
         * @brief Performs checks on input parameters for @ref apply_delta_operation
         *
         * @param delta_record  Pointer to the memory region with delta record
         * @param delta_size    Byte size of the delta record memory region
         * @param dst           Pointer to the destination memory region
         * @param size          Byte size of the memory region
         *
         * @return
         *      - @ref status_code::ok
         *      - @ref status_code::nullptr_error
         *      - @ref status_code::bad_size
         *      - @ref status_code::bad_alignment
         *      - @ref status_code::delta_bad_size
         */
        static status_code apply_delta(const byte_t *const delta_record,
                                       const size_t        delta_size,
                                       const byte_t *      dst,
                                       const size_t        size) noexcept
        {
            // TODO: Add checks
            if (delta_record == nullptr || dst == nullptr)
            {
                return status_code::nullptr_error;
            }
            else if (size == 0u || size % 8 != 0)
            {
                return status_code::bad_size;
            }
            else if (reinterpret_cast<std::uintptr_t>(delta_record) % 8 != 0 ||
                     reinterpret_cast<std::uintptr_t>(dst) % 8 != 0)
            {
                return status_code::bad_alignment;
            }
            else if (delta_size % 10 != 0 || delta_size == 0u)
            {
                return status_code::delta_bad_size;
            }
            else
            {
                return status_code::ok;
            }
        }

        /**
         * @brief Performs checks on input parameters for @ref crc_operation
         *
         * @param src   Pointer to the source memory region
         * @param size  Byte size of the memory region
         *
         * @return
         *      - @ref status_code::ok
         *      - @ref status_code::nullptr_error
         *      - @ref status_code::bad_size
         */
        static status_code crc(const byte_t *const src, const size_t size) noexcept
        {
            if (src == nullptr)
            {
                return status_code::nullptr_error;
            }
            else if (size == 0u)
            {
                return status_code::bad_size;
            }
            else
            {
                return status_code::ok;
            }
        }

        /**
         * @brief Performs checks on input parameters for @ref copy_crc_operation
         *
         * @param src   Pointer to the source memory region
         * @param dst   Pointer to the destination memory region
         * @param size  Byte size of the memory regions
         *
         * @return
         *      - @ref status_code::ok
         *      - @ref status_code::nullptr_error
         *      - @ref status_code::bad_size
         */
        static status_code copy_crc(const byte_t *const src, const byte_t *const dst, const size_t size) noexcept
        {
            if (src == nullptr || dst == nullptr)
            {
                return status_code::nullptr_error;
            }
            else if (size == 0u)
            {
                return status_code::bad_size;
            }
            else if ((src < (dst + size)) && (dst < (src + size)))
            {
                return status_code::buffers_overlapping;
            }
            else
            {
                return status_code::ok;
            }
        }

        /**
         * @brief Performs checks on input parameters for @ref cache_flush_operation
         *
         * @param dst   Pointer to the destination memory region
         * @param size  Byte size of the memory regions
         *
         * @return
         *      - @ref status_code::ok
         *      - @ref status_code::nullptr_error
         *      - @ref status_code::bad_size
         */
        static status_code cache_flush(const byte_t *const dst, const size_t size) noexcept
        {
            if (dst == nullptr)
            {
                return status_code::nullptr_error;
            }
            else if (size == 0u)
            {
                return status_code::bad_size;
            }
            else
            {
                return status_code::ok;
            }
        }

        /**
         * @brief Performs checks on input parameters for @ref batch_operation
         *
         * @param src     Pointer to the source memory region
         * @param length  Number of operation stored in the memory region
         *
         * @return
         *      - @ref status_code::ok
         *      - @ref status_code::nullptr_error
         *      - @ref status_code::bad_length
         */
        static status_code batch(const void *const src, const size_t length) noexcept
        {
            if (src == nullptr)
            {
                return status_code::nullptr_error;
            }
            else if (length < 4u)
            {
                return status_code::bad_length;
            }
            else
            {
                return status_code::ok;
            }
        }
    };

    /**
     * @}
     */
}  // namespace dml

#endif  //DML_COMMON_RANGE_CHECK_HPP
