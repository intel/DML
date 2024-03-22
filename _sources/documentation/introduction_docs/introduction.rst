 .. ***************************************************************************
 .. * Copyright (C) 2021 Intel Corporation
 .. *
 .. * SPDX-License-Identifier: MIT
 .. ***************************************************************************/

.. _introduction_reference_link:

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

-  a simple C/C++ interface and data structures to enhance usability and portability
-  faster time-to-market
-  scalability with Intel® Data Streaming Accelerator (Intel® DSA) hardware

.. note::

   Intel® DML doesn't support all hardware possibilities. Library
   limitations are described in the `Library
   Limitations <../introduction_docs/introduction.html#library-limitations>`__ section.

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

.. _library_limitations_reference_link:

Library Limitations
*******************

-  Library does not work with Dedicated WQs for the accelerator, but uses shared ones only.
-  Library does not have API for the hardware path configuration.
-  Library does not support Hardware execution path on Windows OS.
-  Library is not developed for kernel mode usage. It is user level driver library.

Library APIs
************


Library has several interfaces that provide access to Intel DML
possibilities through different languages and paradigms: -
``Public C API`` - ``Public C++ API``

**C API (Job API):**

Represents a state based interface. The base idea is to allocate a
single state and configure one with different ways to perform necessary
operation. Such API does not allocate memory internally.

More details: `C API Manual <../api_docs/low_level_api.html>`_

**C++ API (High Level API):**

Represents high level operations API written with C++. API provides
compile time optimizations and less operation preparation latency.

More details: `C++ API Manual <../api_docs/high_level_api.html>`__
