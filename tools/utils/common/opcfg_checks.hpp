/*******************************************************************************
 * Copyright (C) 2024 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#ifndef DML_TOOLS_UTILS_COMMON_OPCFG_CHECKS_HPP_
#define DML_TOOLS_UTILS_COMMON_OPCFG_CHECKS_HPP_

#include "test_hw_dispatcher.hpp"
// tool_common
#include "system_info.hpp"

namespace dml::test {

// Operation codes for Intel® Data Streaming Accelerator (Intel® DSA)
// Used in testing to check OPCFG if operation is enabled/disabled
enum dml_test_opcodes {
    opcode_nop             = 0x00u,      /**< Intel® DSA no-operation code                          */
    opcode_batch           = 0x01u,      /**< Intel® DSA Batch operation code                       */
    opcode_drain           = 0x02u,      /**< Intel® DSA Drain operation code                       */
    opcode_mem_move        = 0x03u,      /**< Intel® DSA Memory move operation code                 */
    opcode_fill            = 0x04u,      /**< Intel® DSA Memory fill operation code                 */
    opcode_compare         = 0x05u,      /**< Intel® DSA Memory compare operation code              */
    opcode_compare_pattern = 0x06u,      /**< Intel® DSA Memory compare with pattern operation code */
    opcode_delta_create    = 0x07u,      /**< Intel® DSA Create delta record operation code         */
    opcode_delta_apply     = 0x08u,      /**< Intel® DSA Apply delta record operation code          */
    opcode_dualcast        = 0x09u,      /**< Intel® DSA Memory copy with dualcast operation code   */
    opcode_crc             = 0x10u,      /**< Intel® DSA Generate CRC operation code                */
    opcode_copy_crc        = 0x11u,      /**< Intel® DSA Copy with CRC operation code               */
    opcode_dif_check       = 0x12u,      /**< Intel® DSA Dif check operation code                   */
    opcode_dif_insert      = 0x13u,      /**< Intel® DSA Dif insert operation code                  */
    opcode_dif_strip       = 0x14u,      /**< Intel® DSA Dif strip operation code                   */
    opcode_dif_update      = 0x15u,      /**< Intel® DSA Dif update operation code                  */
    opcode_cache_flush     = 0x20u       /**< Intel® DSA Cache flush operation code                 */
};

inline bool is_opcfg_capabilities_present() {
    bool are_opcfg_capabilities_present = false;

#if defined(__linux__)
    static auto& dispatcher = hw_dispatcher::get_instance();
    if (dispatcher.is_hw_support()) {
        const auto& device             = dispatcher.device(0);
        are_opcfg_capabilities_present = device.get_opcfg_enabled();
    }
#endif

    return are_opcfg_capabilities_present;
}

/**< OPCFG CHECK FUNCTIONS */
inline bool is_operation_disabled_on_all_wq_on_node(dml_test_opcodes opcode) {
    if (!is_opcfg_capabilities_present()) { return false; }

    bool    is_op_disabled = true;
    int32_t numa_id        = get_numa_id();

#if defined(__linux__)
    static auto& dispatcher = hw_dispatcher::get_instance();
    if (dispatcher.is_hw_support()) {
        for (size_t device_idx = 0; device_idx < dispatcher.device_count(); device_idx++) {
            if (static_cast<int32_t>(dispatcher.device(device_idx).numa_id()) != numa_id) { continue; }
            for (size_t wq_idx = 0; wq_idx < dispatcher.device(device_idx).size(); wq_idx++) {
                if (dispatcher.device(device_idx).get_operation_supported_on_wq(wq_idx, opcode)) { return false; }
            }
        }
    }
#endif

    return is_op_disabled;
}

} // namespace dml::test

#endif // DML_TOOLS_UTILS_COMMON_OPCFG_CHECKS_HPP_
