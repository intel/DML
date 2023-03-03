 .. ***************************************************************************
 .. * Copyright (C) 2021 Intel Corporation
 .. *
 .. * SPDX-License-Identifier: MIT
 .. ***************************************************************************/

System Requirements
###################

.. _system_requirements_sw_path_reference_link:

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


.. _system_requirements_hw_path_reference_link:

Hardware Path Requirements
**************************

- **Minimal:** CPU with Intel® Data Streaming Accelerator (Sapphire Rapids and higher).
- Virtualization technology for directed I/O (VT-d) is enabled through the BIOS menu.
- **Linux* OS:**

  - kernel version 5.10 or higher (https://www.kernel.org/doc/Documentation/ABI/stable/sysfs-driver-dma-idxd) **Validated 5.10 - 5.12**.
  - accel-config version 3.2 or higher placed in /usr/lib64/ (https://github.com/intel/idxd-config) **Validated 3.2 - 3.4.1**.


.. _system_requirements_benchmark_reference_link:

Benchmark Requirements
**********************

Currently, compiling and running benchmark is only available on Linux operating systems.
And running benchmarks is only available on Intel® Skylake (or later) microarchitecture.
