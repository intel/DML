 .. ***************************************************************************
 .. * Copyright (C) 2021 Intel Corporation
 .. *
 .. * SPDX-License-Identifier: MIT
 .. ***************************************************************************/

Code Samples and examples
#########################


Low-level C API Examples
************************


The ``<dml_library>/examples/dml_job_api/job_wrapper_examples.c`` source
file contains a number of simple examples, illustrating how you can use
Intel DML.

The wrapper's descriptions are placed in the ``dml_examples.h``
header file.

The examples are compiled during the Intel DML project compilation or
as a separate CMake target (``job_api_samples``).

.. note::

  These examples are intended to be illustrative and functional, 
  but they are not intended to be examples of a complete implementation. 
  In particular, their handling of error conditions is rather primitive.


Multi-Socket Library Usage
==========================


This sample shows how to utilize several sockets on the system using manual ``numa_id`` 
setting in the ``dml_job_t`` structure. Thus, the ``memory_move`` operation can be applied 
to one data array using several sockets at the same time.


.. literalinclude:: ../../../../examples/multisocket.c
    :language: c


High-level C++ API Examples
***************************


cache_flush
===========


.. literalinclude:: ../../../../examples/high_level_api/cache_flush.cpp
    :language: cpp


compare_pattern
===============


.. literalinclude:: ../../../../examples/high_level_api/compare_pattern.cpp
    :language: cpp


compare
=======


.. literalinclude:: ../../../../examples/high_level_api/compare.cpp
    :language: cpp


copy_crc
========


.. literalinclude:: ../../../../examples/high_level_api/copy_crc.cpp
    :language: cpp


crc
===


.. literalinclude:: ../../../../examples/high_level_api/crc.cpp
    :language: cpp


delta
=====


.. literalinclude:: ../../../../examples/high_level_api/delta.cpp
    :language: cpp


dualcast
========


.. literalinclude:: ../../../../examples/high_level_api/dualcast.cpp
    :language: cpp


fill
====


.. literalinclude:: ../../../../examples/high_level_api/fill.cpp
    :language: cpp


mem_move
========


.. literalinclude:: ../../../../examples/high_level_api/mem_move.cpp
    :language: cpp



Multi-Socket Library Usage
==========================


This sample shows how to utilize several sockets on the system. 
Thus, the ``memory_move`` operation can be applied 
to one data array using several sockets at the same time.


.. literalinclude:: ../../../../examples/multisocket.cpp
    :language: cpp
