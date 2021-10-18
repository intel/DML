Intel® Data Mover Library (Intel® DML) Reference Manual
===============================================================================


Notices and Disclaimers
-----------------------

No license (express or implied, by estoppel or otherwise) to any intellectual
property rights is granted by this document.

Intel disclaims all express and implied warranties, including without
limitation, the implied warranties of merchantability, fitness for a particular
purpose, and non-infringement, as well as any warranty arising from course of
performance, course of dealing, or usage in trade. This document contains
information on products, services and/or processes in development. All
information provided here is subject to change without notice. Contact your
Intel representative to obtain the latest forecast, schedule, specifications and
roadmaps. The products and services described may contain defects or errors
which may cause deviations from published specifications. Current characterized
errata are available on request. Intel, the Intel logo, Intel Atom, Intel Core
and Xeon are trademarks of Intel Corporation in the U.S. and/or other countries.
\*Other names and brands may be claimed as the property of others. Microsoft,
Windows, and the Windows logo are trademarks, or registered trademarks of
Microsoft Corporation in the United States and/or other countries. Java is a
registered trademark of Oracle and/or its affiliates.

© Intel Corporation.

This software and the related documents are Intel copyrighted materials, and
your use of them is governed by the express license under which they were
provided to you ("License"). Unless the License provides otherwise, you may
not use, modify, copy, publish, distribute, disclose or transmit this software
or the related documents without Intel's prior written permission. This software
and the related documents are provided as is, with no express or implied
warranties, other than those that are expressly stated in the License.


Introduction
------------

This document describes architecture of the Intel® Data Mover Library (Intel® DML) software library. Intel® Data Mover Library (Intel® DML) is an open-source library to provide high-performance data manipulation on Intel CPUs. Intel® DML is intended for streaming data movement and transformation operations optimization that is applicable for effective storage, networking, persistent memory, and various data processing applications.

Intel® DML supports:

Use the Intel® Data Mover Library (Intel® DML) to improve
performance of database, enterprise data, communications, and
scientific/technical applications. Intel® DML is a common interface for a
number of commonly used algorithms. Using this library enables you to
automatically tune your application to many generations of processors without
changes in your application. The Intel® DML provides high performance
implementations of data processing functions for existing hardware accelerator,
and/or software path in case if hardware accelerator is not available. Code written
with the library automatically takes advantage of available modern CPU
capabilities. This can provide tremendous development and maintenance savings.
The goal of the Intel® DML is to provide application programming
interface (API) with:

-   a simple C/C++ interface and data structures to enhance usability and portability
-   faster time-to-market
-   scalability with Intel® DSA hardware

Getting Started
---------------
### Requirements and Build
Information about how to build Intel® Data Mover Library (Intel® DML) is available in <README.md> file.

### Repository Structure
```
  ├── cmake
  │   └── Modules         # Cmake modules and configuration
  ├── doc                 # Contains library documentation
  ├── examples            
  │   ├── high_level_api  # C++ API usage examples
  │   └── dml_job_api     # C API usage examples
  ├── google-benchmark    # link to google-bechmarks
  ├── google-test         # link to google-tests
  ├── high-level-api      # C++ API headers
  ├── include             # C API headers
  ├── isal                # Compression backend
  ├── sources             # Library Sources
  │   ├── core            # High optimized kernels
  │   ├── hw-path         # HW interconnect
  │   ├── include         # Private includes
  │   └── sw-path         # SW path of the library
  └── tests
      ├── common          # Common service functions for testing 
      ├── benchmarks      # Benchmarks
      ├── testdata        # Corpuses for Compression testing
      ├── high-level-api  # C++ API usage examples
      └── dml_job_tests   # C API usage examples
```

Library Presetting
------------------

Library implements several executon paths. See [Execution Paths](#execution-paths) for more details.

Hardware path is required to set up environment to utilize Intel DSA accelerator:

  - Library requires presetting of hardware configuration before usage. 
  - Library doesn't perform hardware setup independently.
  - System administrator is responsible for correctness of accelerator configuration. If configuration is invalid or doesn't exist,
    library returns an appropriate status code in case if hardware execution path used.

**Note**: Intel® DML doesn't support all hardware possibilities. Library limitations are described in the [Library Limitations](#library-limitations) section.


### System Requirements
- Software path requirements:
    - Minimal: x86-64 CPU with Intel® Advanced Vector Extensions 2 support (Intel® microarchitecture code name Broadwell) not optimized code.
    - Optimal: x86-64 CPU with Intel® Advanced Vector Extensions 512 (Intel® AVX-512) support (Intel® microarchitecture code name Skylake (Server) processor or higher).

- Hardware path requirements:
    - Minimal: CPU with Intel® DSA (Sapphire Rapids and higher).
    - Virtualization technology for directed I/O (VT-d) is enabled through the BIOS menu.
    - Linux OS:
        - kernel version 5.10 or higher (https://www.kernel.org/doc/Documentation/ABI/stable/sysfs-driver-dma-idxd) **Validated 5.10 - 5.12**.
        - accel-config version 3.2 or higher placed in /usr/lib64/ (https://github.com/intel/idxd-config) **Validated 3.2 - 3.4.1**.

### Build Prerequisites
- Compiler:
    - Linux* OS: gcc 8.2 or higher
    - Windows* OS: MSVC 19 or higher
 - Libraries:    
    - Linux: Universally Unique ID library: `uuid-dev` version 2.35.2 or higher.
- Cross-platform build tool: CMake* version 3.12 or higher
- Make: GNU 'make' (Linux* OS) or 'nmake' (Windows*)
- Documentation generator: 
    - Doxygen 1.8.17 or higher



### Accelerator Configuration
Accelerator configuration can be performed with libaccel-config application.
Such application can be founded here: `https://github.com/intel/idxd-config`

How to work with it correctly is not a goal of this Manual. Follow instructions that are placed here: `https://github.com/intel/idxd-config`


Library Possibilities
---------------------
In this section is high level overview of the Intel® DML possibilities provided.

### Execution paths
The library supports several implementation/execution paths that help to achieve the optimal system resources utilization:
- `Hardware Path` - all hardware-supported functions are executed by the Intel® DSA.
- `Software Path` - all supported functionality is executed by the software library.

### Operations
The library supports several groups of operations:
* Flow Control
* Memory Transferring and Filling
* Memory Compare and Restore
* Memory Hash
* Utility

Library Limitations
---------------------
- Library doesn't work with Dedicated WQs for the accelerator, but uses shared ones only. 
- Library doesn't have API for the hardware path configuration.
- Library doesn't support Hardware execution path on Windows OS.
- Library is not developed for kernel mode usage. It is user level driver library.

Library APIs
------------
Library has several interfaces that provide access to Intel® DML possibilities through different languages and paradigms:
- `Public C API`
- `Public C++ API`

### C API (Job API)
Represents a state based interface. The base idea is to allocate a single state and configure one with different ways 
to perform necessary operation. Such API doesn't allocate memory internally.

More details: [C API Manual](./LOW_LEVEL_API_GUIDE.md)

### C++ API (High Level API)
Represents high level operations API written with C++. API provides compile time optimizations and less operation preparation latency.

More details: [C++ API Manual](./HIGH_LEVEL_API_GUIDE.md)


Issues Reporting
------------------
Intel® DML has several execution paths and large number of different operations and modes, 
thus correct issue description is important. Well filled description helps to detect and solve problems faster. 

### Issue classification
Issues can be classified depending on the step where they occur:
- Configuration step.
- Intel® DML Hardware path initialization step
- Intel® DML operation execution step.

All these classes are described below:

#### Configuration step issue
Intel® DML library works with accelerator configuration created by system administrator. 
There can be a lot of different problems. They can affect library work correctness. To avoid these problems follow
instructions in the [Library presetting](#library-presetting) section.
These classes of issue are placed out of scope Intel® DML library and can be addressed to `idxd-driver` team or `idxd-config` team.


#### Initialization step issue
Intel® DML library has 2 different status code groups. The first group helps to detect initialization issues. 
This issue can be caused by:
- Incorrect configuration, that library doesn't support by reason listed in [Library Limitation](#library-limitations) section
- Bug in the library initialization code. This kind of problem must be reported to Intel® DML team.
