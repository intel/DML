 .. ***************************************************************************
 .. * Copyright (C) 2021 Intel Corporation
 .. *
 .. * SPDX-License-Identifier: MIT
 .. ***************************************************************************/

Release Notes
#############

v0.1.8-beta
***********

**Date**: February 2022

**Note**: Release introduces the auto execution path and manual NUMA selection for C++ API as well as several page fault handling bugfixes.

**Features**:

- Implemented the auto execution path (software fallback) for C++ API. The library tries to use hardware, but in case it is unavailable, there is a software fallback.
- Added ``numa_id`` parameter for ``dml::execute`` and ``dml::submit`` functions to specify custom NUMA node id for submission. Setting a number allows the library to do cross-socket submissions.
- Removed DML_HW cmake option. The library is built with HW support by default.
- Added dynamic optimization dispatcher. The library checks if a necessary instruction set is supported on the system at runtime.

**Bug fix**:

- Fixed erroneous results for Compare operations when a page fault occurred during processing.
- Fixed wrong detection for the on-write page faults.

**Optimizations**:

- Optimized reflected CRC operation.


v0.1.7-beta
***********

**Date**: January 2022

**Note**: Release introduces initial implementation for the auto execution path, page fault handling, and manual NUMA node selection API

**Features**:

- Implemented the auto execution path (software fallback) for C API. The library will try to use hardware, but in case it is unavailable there is a software fallback.
- Added page fault handling:
    - Removed usage of BlockOnFault flag
    - If page fault occurred during descriptor processing:
        - For the hardware execution path an erroneous status is returned
        - For the auto execution path there is a software fallback, so the remainder of the workload is processed on CPU.
- Added ``numa_id`` field for ``dml_job_t`` structure to specify custom NUMA node id for submission. Setting a number allows the library to do cross-socket submissions.

**Optimizations**:

- Optimized CRC operation for short lengths


v0.1.6-beta
***********

**Date**: December 2021

**Note**: Release introduces bug fixes and several minor improvements

**Features**:

- Improved incorrect input checking
- Added check for adjacent buffers for the DIF Strip operation. Status: ``DML_STATUS_DIF_STRIP_ADJACENT_ERROR``
- Reworked hardware related statuses for C API
- Added new status to indicate submission failure:

  - ``DML_STATUS_WORK_QUEUES_NOT_AVAILABLE`` for C API
  - ``dml::status_code::queue_busy`` for C++ API

- Removed LIBACCEL_3_2 cmake option. The supported version of accel-config is now 3.2 and higher
- NUMA node id is detected before each submission now, so threads are safe to change nodes at any time

**Bug fix**:

- Fixed the issue when batch operation doesn't work for buffer not aligned on 64 bytes boundary
- Fixed the issue when current thread NUMA node id is deduced incorrectly
- Fixed crashes when there are no available devices for the current thread NUMA node id
- Removed dependencies on C++ runtime from C API

**Warnings**:

- As NUMA node id of the current thread is now deduced correctly,
  ensure that accelerators' configuration is compatible.
  The library does no cross-socket submissions.
  If there is no available device for the current NUMA node id,
  then an error status code is reported.


v0.1.5-beta
***********

**Date**: November 2021

**Note**: Release introduces unification of underlying implementation for both C and C++ APIs

**Features**:

- Added internal device selection logic to C API (the same as for C++ API)

  - Selector considers submitting thread's NUMA node id
  - Selector switches devices and work queues with each submission

- Improved range checking for C and C++ APIs

**Bug fix**:

- Lowered memory size requirements for job structure by ~100x


v0.1.4-beta
***********


**Date**: Ocotber 2021

**Note**: Release introduces several fixes for code and infrastructure

**Bug fix**:

- Fixed crashes when C++ API was used without root permissions.
- Fixed crashes when HW initialization failed.
- Fixed documentation section about system requirements.
- Fixed project install-paths. They are now following GNU guidelines.
- Fixed wrong path for loading ``libaccel-config.so`` to ``/usr/lib64/libaccel-config.so``.

**Warnings**:

- Correct inclusion path for C API is ``<dml/dml.h>`` now.


v0.1.3-beta
***********


**Date**: August, 2021

**Note**: Initial library version
