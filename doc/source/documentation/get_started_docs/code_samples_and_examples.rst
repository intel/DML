 .. ***************************************************************************
 .. * Copyright (C) 2021 Intel Corporation
 .. *
 .. * SPDX-License-Identifier: MIT
 .. ***************************************************************************/

.. _code_examples_reference_link:

Code Samples and examples
#########################

.. _code_examples_c_reference_link:

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


.. literalinclude:: ../../../../examples/low-level-api/multisocket.c
    :language: c


.. _code_examples_cpp_reference_link:

High-level C++ API Examples
***************************


cache_flush
===========


.. literalinclude:: ../../../../examples/high-level-api/cache_flush_example.cpp
    :language: cpp


compare_pattern
===============


.. literalinclude:: ../../../../examples/high-level-api/compare_pattern_example.cpp
    :language: cpp


compare
=======


.. literalinclude:: ../../../../examples/high-level-api/compare_example.cpp
    :language: cpp


copy_crc
========


.. literalinclude:: ../../../../examples/high-level-api/copy_crc_example.cpp
    :language: cpp


crc
===


.. literalinclude:: ../../../../examples/high-level-api/crc_example.cpp
    :language: cpp


delta
=====


.. literalinclude:: ../../../../examples/high-level-api/delta_example.cpp
    :language: cpp


dualcast
========


.. literalinclude:: ../../../../examples/high-level-api/dualcast_example.cpp
    :language: cpp


fill
====


.. literalinclude:: ../../../../examples/high-level-api/fill_example.cpp
    :language: cpp


mem_move
========


.. literalinclude:: ../../../../examples/high-level-api/mem_move_example.cpp
    :language: cpp



Multi-Socket Library Usage
==========================


This sample shows how to utilize several sockets on the system.
Thus, the ``memory_move`` operation can be applied
to one data array using several sockets at the same time.


.. literalinclude:: ../../../../examples/high-level-api/multi_socket_example.cpp
    :language: cpp
