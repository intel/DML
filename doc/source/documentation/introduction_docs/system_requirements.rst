 .. ***************************************************************************
 .. * Copyright (C) 2021 Intel Corporation
 .. *
 .. * SPDX-License-Identifier: MIT
 .. ***************************************************************************/

System Requirements
###################


Software Path Requirements
**************************


.. list-table::
    :header-rows: 0

    * - **Minimal**
      - x86-64 CPU with Intel® Advanced Vector Extensions 2 support
        (Intel® microarchitecture code name Broadwell) not optimized code.
    * - **Recommended**
      - x86-64 CPU with Intel® Advanced Vector Extensions 512 (Intel® AVX-512) support
        (Intel® microarchitecture code name Skylake (Server) processor or higher).


Hardware Path Requirements
**************************


- **Minimal:** CPU with Intel® Data Streaming Accelerator (Sapphire Rapids and higher).
- Virtualization technology for directed I/O (VT-d) is enabled through the BIOS menu.
- **Linux* OS:**

  - kernel version 5.10 or higher (https://www.kernel.org/doc/Documentation/ABI/stable/sysfs-driver-dma-idxd) **Validated 5.10 - 5.12**.
  - accel-config version 3.2 or higher placed in /usr/lib64/ (https://github.com/intel/idxd-config) **Validated 3.2 - 3.4.1**.


Accelerator Configuration
*************************


Accelerator configuration can be performed with libaccel-config application.
Such application can be founded here: https://github.com/intel/idxd-config.

How to work with it correctly is not a goal of this chapter.
Follow instructions that are placed here: https://github.com/intel/idxd-config.
