Intel® Data Mover Library (Intel® DML) Release Notes
===============================================================================

### Intel® DML v0.1.5-beta

**Date: November 2021**

**Note**: Release introduces unification of underlying implementation for both C and C++ APIs

**Features**:
* Added internal device selection logic to C API (the same as for C++ API)
  * Selector considers submitting thread's NUMA node id
  * Selector switches devices and work queues with each submission
* Improved range checking for C and C++ APIs

**Bug fix**:
* Lowered memory size requirements for job structure by ~100x.


### Intel® DML v0.1.4-beta

**Date: October 2021**

**Note**: Release introduces several fixes for code and infrastructure

**Bug fix**:
* Fixed crashes when C++ API was used without root permissions.
* Fixed crashes when HW initialization failed.
* Fixed documentation section about system requirements.
* Fixed project install-paths. They are now following GNU guidelines.
* Fixed wrong path for loading `libaccel-config.so` to `/usr/lib64/libaccel-config.so`.

**Warnings**:
* Correct inclusion path for C API is `<dml/dml.h>` now.


### Intel® DML Library v0.1.3-beta

**Date: August, 2021**

**Note**: Initial Library version

Legal Information
=================

No license (express or implied, by estoppel or otherwise) to any
intellectual property rights is granted by this document.
Intel disclaims all express and implied warranties, including without limitation, the implied warranties of merchantability, fitness for a particular purpose, and non-infringement, as well as any warranty arising from course of performance, course of dealing, or usage in trade. This document contains information on products, services and/or processes in development. All information provided here is subject to change without notice. Contact your Intel representative to obtain the latest forecast, schedule, specifications and
roadmaps. The products and services described may contain defects or errors which may cause deviations from published specifications. Current characterized errata are available on request. Intel, the Intel logo, Intel Atom, Intel Core and Xeon are trademarks of Intel Corporation in the U.S. and/or other countries.
\*Other names and brands may be claimed as the property of others. Microsoft, Windows, and the Windows logo are trademarks, or registered trademarks of Microsoft Corporation in the United States and/or other countries. Java is a registered trademark of Oracle and/or its affiliates.

**Copyright 2020-2021 Intel Corporation.**

This software and the related documents are Intel copyrighted materials, and your use of them is governed by the express license under which they were provided to you (**License**). Unless the License provides otherwise, you may not use, modify, copy, publish, distribute, disclose or transmit this software or the related documents without Intel's prior written permission. This software and the related documents are provided as is, with no express or implied warranties, other than those that are expressly stated in the License.
