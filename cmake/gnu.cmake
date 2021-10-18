#
# Copyright 2021 Intel Corporation.
#
# This software and the related documents are Intel copyrighted materials,
# and your use of them is governed by the express license under which they
# were provided to you ("License"). Unless the License provides otherwise,
# you may not use, modify, copy, publish, distribute, disclose or transmit
# this software or the related documents without Intel's prior written
# permission.
#
# This software and the related documents are provided as is, with no
# express or implied warranties, other than those that are expressly
# stated in the License.
#

list(APPEND DML_SECURITY_OPTIONS
    -fPIC
    -fstack-protector
    --param=ssp-buffer-size=8
    -Wl,-z,relro,-z,now
    -Wl,-z,noexecstack
    -fstack-clash-protection
    )

list(APPEND DML_SECURITY_DEFINITIONS
    $<$<CONFIG:RELEASE>:_FORTIFY_SOURCE=2>)

list(APPEND DML_QUALITY_OPTIONS
    $<$<BOOL:${DML_WERROR}>:-Werror>
    -Wall
    -Wextra
    -pedantic
    )

list(APPEND DML_AVX512_OPTIONS -march=skylake-avx512) #  -mavx512dq -mavx512vl -mavx512bw -mclflushopt -mclwb
