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
 * @date 05/20/2021
 * @brief Contains operation definitions
 */

#ifndef DML_OPERATIONS_MEM_MOVE_HPP
#define DML_OPERATIONS_MEM_MOVE_HPP

#include <dml_common/result.hpp>

namespace dml
{
    /**
     * @ingroup dmlhl_ops
     * @brief Memory Move operation
     *
     * This operation moves data from the source memory region to the destination memory region.
     *
     * See also @ref dml::mem_move
     */
    class mem_move_operation
    {
    public:
        /**
         * @brief Constructs the operation
         */
        constexpr mem_move_operation() = default;

        /**
         * @brief Result type for this operation
         *
         * See @ref mem_move_result
         */
        using result_type = mem_move_result;
    };

    /**
     * @ingroup dmlhl_ops
     * @brief Predefined instance of @ref mem_move_operation
     *
     * For usage examples see corresponding @ref dml::submit or @ref dml::execute
     */
    constexpr auto mem_move = mem_move_operation();

    /**
     * @ingroup dmlhl_ops
     * @brief Memory Copy operation
     *
     * This operation copies data from the source memory region to the destination memory region.
     *
     * @note This operation doesn't perform copy of overlapping memory buffers
     *
     * See also @ref dml::mem_copy
     */
    class mem_copy_operation
    {
    public:
        /**
         * @brief Constructs the operation
         */
        constexpr mem_copy_operation() = default;

        /**
         * @brief Result type for this operation
         *
         * See @ref mem_copy_result
         */
        using result_type = mem_copy_result;
    };

    /**
     * @ingroup dmlhl_ops
     * @brief Predefined instance of @ref mem_copy_operation
     *
     * For usage examples see corresponding @ref dml::submit or @ref dml::execute
     */
    constexpr auto mem_copy = mem_copy_operation();

    /**
     * @ingroup dmlhl_ops
     * @brief Fill operation
     *
     * This operation fills data at the destination memory region with 64-bit pattern.
     *
     * See also @ref dml::fill
     */
    class fill_operation
    {
    public:
        /**
         * @brief Constructs the operation
         */
        constexpr fill_operation() = default;

        /**
         * @brief Result type for this operation
         *
         * See @ref fill_result
         */
        using result_type = fill_result;
    };

    /**
     * @ingroup dmlhl_ops
     * @brief Predefined instance of @ref fill_operation
     *
     * For usage examples see corresponding @ref dml::submit or @ref dml::execute
     */
    constexpr auto fill = fill_operation();

    /**
     * @ingroup dmlhl_ops
     * @brief Dualcast operation
     *
     * This operation copies data from the source memory region to two destination memory regions.
     *
     * See also @ref dml::dualcast
     */
    class dualcast_operation
    {
    public:
        /**
         * @brief Constructs the operation
         */
        constexpr dualcast_operation() = default;

        /**
         * @brief Result type for this operation
         *
         * See @ref dualcast_result
         */
        using result_type = dualcast_result;
    };

    /**
     * @ingroup dmlhl_ops
     * @brief Predefined instance of @ref dualcast_operation
     *
     * For usage examples see corresponding @ref dml::submit or @ref dml::execute
     */
    constexpr auto dualcast = dualcast_operation();

    /**
     * @ingroup dmlhl_ops
     * @brief Compare operation
     *
     * This operation compares data at the first memory region with data at the second memory region.
     *
     * Comparison result will be written into @ref compare_result.result field.
     *
     * By default the operation doesn't check for comparison result and always returns @ref status_code::ok.
     * To enable result checking use one of this class methods. See the following table for detailed view on options:
     *
     * | Expected result  | Actual result | Returned status                   |
     * | ---------------- | ------------- | --------------------------------- |
     * | Not specified    | Any           | @ref status_code::ok              |
     * | Equal            | Equal         | @ref status_code::ok              |
     * | Equal            | Not equal     | @ref status_code::false_predicate |
     * | Not equal        | Equal         | @ref status_code::false_predicate |
     * | Not equal        | Not equal     | @ref status_code::ok              |
     *
     * See also @ref dml::compare
     */
    class compare_operation
    {
    public:
        /**
         * @brief Constructs the operation with default parameters
         */
        constexpr compare_operation() = default;

        /**
         * @brief Result type for this operation
         *
         * See @ref compare_result
         */
        using result_type = compare_result;

        /**
         * @brief Returns a new instance of the operation with "equal" expected result
         *
         * @return New instance of the operation
         */
        [[nodiscard]] constexpr auto expect_equal() const noexcept { return compare_operation(equality::equal); }

        /**
         * @brief Returns a new instance of the operation with "not_equal" expected result
         *
         * @return New instance of the operation
         */
        [[nodiscard]] constexpr auto expect_not_equal() const noexcept
        {
            return compare_operation(equality::not_equal);
        }

        /**
         * @brief Returns underlying options
         *
         * @return Underlying options
         */
        [[nodiscard]] equality get_expected_result() const { return expect; }

    private:
        /**
         * @brief Constructs the operation with specified expected result
         */
        constexpr explicit compare_operation(const equality &expect) noexcept: expect(expect) { }

    private:
        equality expect{equality::not_specified};
    };

    /**
     * @ingroup dmlhl_ops
     * @brief Predefined instance of @ref compare_operation
     *
     * For usage examples see corresponding @ref dml::submit or @ref dml::execute.
     */
    constexpr auto compare = compare_operation();

    /**
     * @ingroup dmlhl_ops
     * @brief Compare with Pattern operation
     *
     * This operation compares data at the source memory region with 64-bit pattern.
     *
     * Comparison result will be written into @ref compare_result.result field
     *
     * By default the operation doesn't check for comparison result and always returns @ref status_code::ok.
     * To enable result checking use one of this class methods. See the following table for detailed view on options:
     *
     * | Expected result  | Actual result | Returned status                   |
     * | ---------------- | ------------- | --------------------------------- |
     * | Not specified    | Any           | @ref status_code::ok              |
     * | Equal            | Equal         | @ref status_code::ok              |
     * | Equal            | Not equal     | @ref status_code::false_predicate |
     * | Not equal        | Equal         | @ref status_code::false_predicate |
     * | Not equal        | Not equal     | @ref status_code::ok              |
     *
     * See also @ref dml::compare_pattern
     */
    class compare_pattern_operation
    {
    public:
        /**
         * @brief Constructs the operation with default parameters
         */
        constexpr compare_pattern_operation() = default;

        /**
         * @brief Result type for this operation
         *
         * See @ref compare_result
         */
        using result_type = compare_result;

        /**
         * @brief Returns a new instance of the operation with "equal" expected result
         *
         * @return New instance of the operation
         */
        [[nodiscard]] constexpr auto expect_equal() const noexcept
        {
            return compare_pattern_operation(equality::equal);
        }

        /**
         * @brief Returns a new instance of the operation with "not_equal" expected result
         *
         * @return New instance of the operation
         */
        [[nodiscard]] constexpr auto expect_not_equal() const noexcept
        {
            return compare_pattern_operation(equality::not_equal);
        }

        /**
         * @brief Returns underlying options
         *
         * @return Underlying options
         */
        [[nodiscard]] equality get_expected_result() const { return expect; }

    private:
        /**
         * @brief Constructs the operation with specified expected result
         */
        constexpr explicit compare_pattern_operation(const equality &expect) noexcept: expect(expect) { }

    private:
        equality expect{equality::not_specified};
    };

    /**
     * @ingroup dmlhl_ops
     * @brief Predefined instance of @ref compare_pattern_operation
     *
     * For usage examples see corresponding @ref dml::submit or @ref dml::execute
     */
    constexpr auto compare_pattern = compare_pattern_operation();

    /**
     * @ingroup dmlhl_ops
     * @brief Create Delta operation
     *
     * This operation compares data at the first source memory region with data at the second memory region.
     * Then it writes delta record data to the memory region.
     * The delta record contains the information needed to update the data at the first source memory region
     * to match the data at the second source memory region.
     *
     * Paired with @ref apply_delta_operation
     *
     * See also @ref dml::create_delta
     */
    class create_delta_operation
    {
    public:
        /**
         * @brief Constructs the operation
         */
        constexpr create_delta_operation() = default;

        /**
         * @brief Result type for this operation
         *
         * See @ref create_delta_result
         */
        using result_type = create_delta_result;
    };

    /**
     * @ingroup dmlhl_ops
     * @brief Predefined instance of @ref create_delta_operation
     *
     * For usage examples see corresponding @ref dml::submit or @ref dml::execute
     */
    constexpr auto create_delta = create_delta_operation();

    /**
     * @ingroup dmlhl_ops
     * @brief Apply Delta operation
     *
     * This operation applies delta record written to the memory region onto data at the destination memory region.
     * This operation is used in pair with @ref create_delta_operation to update first memory region to match the second
     * using written delta record.
     *
     * See also @ref dml::apply_delta
     */
    class apply_delta_operation
    {
    public:
        /**
         * @brief Constructs the operation
         */
        constexpr apply_delta_operation() = default;

        /**
         * @brief Result type for this operation
         *
         * See @ref apply_delta_result
         */
        using result_type = apply_delta_result;
    };

    /**
     * @ingroup dmlhl_ops
     * @brief Predefined instance of @ref apply_delta_operation
     *
     * For usage examples see corresponding @ref dml::submit or @ref dml::execute
     */
    constexpr auto apply_delta = apply_delta_operation();

    /**
     * @ingroup dmlhl_ops
     * @brief CRC operation
     *
     * This operation computes CRC on the data at the source memory region.
     * Initial crc seed is provided via crc_seed argument.
     *
     * This operation supports 2 options: @ref crc_parameters.bypass_reflection and @ref crc_parameters.bypass_data_reflection
     *
     * To enable any option, use this class methods.
     *
     * See also @ref dml::crc
     */
    class crc_operation
    {
    public:
        /**
         * @brief Result type for this operation
         *
         * See @ref crc_result
         */
        using result_type = crc_result;

        /**
         * @brief Constructs CRC operation with default options
         *
         * Reflection and data reflection are enabled.
         */
        constexpr crc_operation() noexcept = default;

        /**
         * @brief Returns a new instance of the operation with @ref crc_parameters.bypass_reflection option enabled.
         *
         * @return New instance of the operation
         */
        [[nodiscard]] constexpr auto bypass_reflection() const noexcept
        {
            return crc_operation({true, params.bypass_data_reflection});
        }

        /**
         * @brief Returns a new instance of the operation with @ref crc_parameters.bypass_data_reflection option enabled.
         *
         * @return New instance of the operation
         */
        [[nodiscard]] constexpr auto bypass_data_reflection() const noexcept
        {
            return crc_operation({params.bypass_reflection, true});
        }

        /**
         * @brief Returns underlying options
         *
         * @return Underlying options
         */
        [[nodiscard]] const crc_parameters &get_params() const { return params; }

    private:
        /**
         * @brief Constructs the operation with specified parameters
         */
        constexpr explicit crc_operation(const crc_parameters &params) noexcept: params(params) { }

    private:
        crc_parameters params{false, false}; /**< CRC parameters */
    };

    /**
     * @ingroup dmlhl_ops
     * @brief Predefined instance of @ref crc_operation
     *
     * For usage examples see corresponding @ref dml::submit or @ref dml::execute
     */
    constexpr auto crc = crc_operation();

    /**
     * @ingroup dmlhl_ops
     * @brief Copy + CRC operation
     *
     * This operation copies data at the source memory region to the destination memory region.
     * Then it computes CRC on the data copied.
     * Initial crc seed is provided via crc_seed argument.
     *
     * This operation supports 2 options: @ref crc_parameters.bypass_reflection and @ref crc_parameters.bypass_data_reflection
     *
     * To enable any option, use this class methods.
     *
     * See also @ref dml::crc
     */
    class copy_crc_operation
    {
    public:
        /**
         * @brief Result type for this operation
         *
         * See @ref crc_result
         */
        using result_type = crc_result;

        /**
         * @brief Constructs CRC operation with default options
         *
         * Reflection and data reflection are enabled.
         */
        constexpr copy_crc_operation() noexcept = default;

        /**
         * @brief Returns a new instance of the operation with @ref crc_parameters.bypass_reflection option enabled.
         *
         * @return New instance of the operation
         */
        [[nodiscard]] constexpr auto bypass_reflection() const noexcept
        {
            return copy_crc_operation({true, params.bypass_data_reflection});
        }

        /**
         * @brief Returns a new instance of the operation with @ref crc_parameters.bypass_data_reflection option enabled.
         *
         * @return New instance of the operation
         */
        [[nodiscard]] constexpr auto bypass_data_reflection() const noexcept
        {
            return copy_crc_operation({params.bypass_reflection, true});
        }

        /**
         * @brief Returns underlying options
         *
         * @return Underlying options
         */
        [[nodiscard]] const crc_parameters &get_params() const { return params; }

    private:
        /**
         * @brief Constructs the operation with specified parameters
         */
        constexpr explicit copy_crc_operation(const crc_parameters &params) noexcept: params(params) { }

    private:
        crc_parameters params{false, false}; /**< CRC parameters */
    };

    /**
     * @ingroup dmlhl_ops
     * @brief Predefined instance of @ref copy_crc_operation
     *
     * For usage examples see corresponding @ref dml::submit or @ref dml::execute
     */
    constexpr auto copy_crc = copy_crc_operation();

    /**
     * @ingroup dmlhl_ops
     * @brief Cache Flush operation
     *
     * The Cache Flush operation flushes the processor caches at the destination memory region.
     *
     * By default this operation invalidates affected cache lines from every level of cache hierarchy.
     * To disable that, use @ref dont_invalidate_cache
     *
     * See also @ref dml::cache_flush
     */
    class cache_flush_operation
    {
    public:
        /**
         * @brief Result type for this operation
         *
         * See @ref cache_flush_result
         */
        using result_type = cache_flush_result;

        /**
         * @brief Constructs Cache Flush operation with default option
         *
         * Cache invalidation is enabled.
         */
        constexpr cache_flush_operation() noexcept = default;

        /**
         * @brief Disables cache invalidation
         *
         * @return New instance of the operation with cache invalidation off
         */
        [[nodiscard]] constexpr auto dont_invalidate_cache() const noexcept { return cache_flush_operation(true); }

        /**
         * @brief Returns underlying options
         *
         * @return Underlying options
         */
        [[nodiscard]] bool get_params() const { return do_not_invalidate; }

    private:
        /**
         * @brief Constructs the operation with specified parameter
         */
        constexpr explicit cache_flush_operation(bool do_not_invalidate): do_not_invalidate(do_not_invalidate) { }

    private:
        bool do_not_invalidate = false; /**< Flag for cache invalidation */
    };

    /**
     * @ingroup dmlhl_ops
     * @brief Predefined instance of @ref cache_flush_operation
     *
     * For usage examples see corresponding @ref dml::submit or @ref dml::execute
     */
    constexpr auto cache_flush = cache_flush_operation();

    /**
     * @ingroup dmlhl_ops
     * @brief Batch operation
     *
     * This operation process multiple operations at once.
     *
     * See also @ref dml::batch
     */
    class batch_operation
    {
    public:
        /**
         * @brief Constructs the operation
         */
        constexpr batch_operation() = default;

        /**
         * @brief Result type for this operation
         *
         * See @ref batch_result
         */
        using result_type = batch_result;
    };

    /**
     * @ingroup dmlhl_ops
     * @brief Predefined instance of @ref batch_operation
     *
     * For usage examples see corresponding @ref dml::submit or @ref dml::execute
     */
    constexpr auto batch = batch_operation();
}  // namespace dml

#endif  //DML_OPERATIONS_HPP
