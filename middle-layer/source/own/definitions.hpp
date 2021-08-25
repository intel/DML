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

#ifndef DML_ML_SOURCE_OWN_DEFINITIONS_HPP
#define DML_ML_SOURCE_OWN_DEFINITIONS_HPP

#if defined(__GNUC__)
    /**
     * @brief Packs a structure byte by byte
     */
    #define DML_PACKED_STRUCT_DECLARATION_BEGIN(name) struct __attribute__((__packed__)) name

    /**
     * @brief Pops a previous structure pack property
     */
    #define DML_PACKED_STRUCT_DECLARATION_END
#elif (_MSC_VER)
    /**
     * @brief Packs a structure byte by byte
     */
    #define DML_PACKED_STRUCT_DECLARATION_BEGIN(name) __pragma(pack(push, 1)) struct name

    /**
    * @brief Pops a previous structure pack property
    */
    #define DML_PACKED_STRUCT_DECLARATION_END         __pragma(pack(pop))
#else
    #error Compiler not supported
#endif

#endif  //DML_ML_SOURCE_OWN_DEFINITIONS_HPP
