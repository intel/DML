# Copyright (C) 2021 Intel Corporation
# Copyright (C) 2023 Hamish Nicholson
#
# SPDX-License-Identifier: MIT

list(APPEND DML_SECURITY_COMPILE_OPTIONS
  -fPIC
  -fstack-protector
  --param=ssp-buffer-size=8
  -fstack-clash-protection
  -U_FORTIFY_SOURCE
  -D_FORTIFY_SOURCE=2
  )


list(APPEND DML_SECURITY_LINK_OPTIONS
  -Wl,-z,relro,-z,now
  -Wl,-z,noexecstack
  )

list(APPEND DML_QUALITY_OPTIONS
  -Wall
  -Wextra
  -pedantic
  )

list(APPEND DML_CPP_PRIVATE_OPTIONS
  -fno-exceptions
  -fno-rtti
  )
