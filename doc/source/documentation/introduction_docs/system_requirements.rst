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


.. _accelerator_configuration_reference_link:

Accelerator Configuration
*************************

Accelerator configuration can be performed with ``libaccel-config`` library,
that can be founded at https://github.com/intel/idxd-config.

For instance, users can use the following commands
to directly configure the device with the accel-config using existing config file:

.. code-block:: shell

    accel-config load-config -c <config file>
    accel-config enable-device <device>
    accel-config enable-wq <device>/<wq>

.. attention::

   Sudo privileges are required to configure Intel® Data Streaming Accelerator (Intel® DSA).
