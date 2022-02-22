 .. ***************************************************************************
 .. * Copyright (C) 2021 Intel Corporation
 .. *
 .. * SPDX-License-Identifier: MIT
 .. ***************************************************************************/

Introduction
############


The Intel® Data Mover Library (Intel® DML) is used to improve performance of
database, enterprise data, communications, and scientific/technical
applications. Intel DML is a common interface for a number of commonly
used algorithms. Using this library enables you to automatically tune
your application to many generations of processors without changes in
your application. The Intel DML provides high performance
implementations of data processing functions for existing hardware
accelerator, and/or software path in case if hardware accelerator is not
available. Code written with the library automatically takes advantage
of available modern CPU capabilities. This can provide tremendous
development and maintenance savings. The goal of the Intel DML is to
provide application programming interface (API) with:

-  a simple C/C++ interface and data structures to enhance usability and
   portability
-  faster time-to-market
-  scalability with Intel® Data Streaming Accelerator (Intel® DSA) hardware


Library Possibilities
*********************


**Execution paths:**

The library supports several implementation/execution paths that help to
achieve the optimal system resources utilization:

- ``Hardware Path`` - all hardware-supported functions are executed by the Intel® DSA.
- ``Software Path`` - all supported functionality is executed by the software library.
- ``Auto Path`` - the library automatically dispatches execution of the requested jobs either to Intel® Data Streaming Accelerator or to the software path of the library depending on internal heuristics.

**Operations:**

The library supports several groups of operations:

- Flow Control
- Memory Transferring and Filling
- Memory Compare and Restore
- Memory Hash
- Utility

Library Limitations
*******************


-  Library doesn’t work with Dedicated WQs for the accelerator, but uses
   shared ones only.
-  Library doesn’t have API for the hardware path configuration.
-  Library doesn’t support Hardware execution path on Windows OS.
-  Library is not developed for kernel mode usage. It is user level
   driver library.

Library APIs
************


Library has several interfaces that provide access to Intel DML
possibilities through different languages and paradigms: -
``Public C API`` - ``Public C++ API``

**C API (Job API):**

Represents a state based interface. The base idea is to allocate a
single state and configure one with different ways to perform necessary
operation. Such API doesn’t allocate memory internally.

More details: `C API Manual <../api_docs/low_level_api.html>`_

**C++ API (High Level API):**

Represents high level operations API written with C++. API provides
compile time optimizations and less operation preparation latency.

More details: `C++ API Manual <../api_docs/high_level_api.html>`__

Issue Reporting & Classification
********************************


Intel DML has several execution paths and large number of different
operations and modes, thus correct issue description is important. Well
filled description helps to detect and solve problems faster.
Use GitHub issues to report any problems.

Issues can be classified depending on the step where they occur:

- Configuration step.
- Hardware path initialization step
- [TBD] Operation execution step.

All these classes are described below:

**Configuration step issues:**

Intel DML library works with accelerator configuration created by
system administrator. There can be a lot of different problems. They can
affect library work correctness. To avoid these problems follow
instructions in the `Library presetting
<../get_started_docs/installation.html#library-presetting-and-building>`__
section. These classes of issue are placed out of scope Intel DML
library and can be addressed to **idxd-driver** team or **idxd-config**
team.

**Initialization step issues:**

Intel DML library has 2 different status code groups. The first group
helps to detect initialization issues. This issue can be caused by:

- Incorrect configuration, that library doesn’t support by reason listed
  in `Library Limitation <#library-limitations>`__ section
- Bug in the library initialization code.
  This kind of problem must be reported to Intel DML team.
