# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT

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
    -Wall
    -Wextra
    -pedantic
    )

list(APPEND DML_CPP_PRIVATE_OPTIONS
        -fno-exceptions
        -fno-rtti
        -fno-threadsafe-statics
        )
