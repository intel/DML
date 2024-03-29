 .. ***************************************************************************
 .. * Copyright (C) 2021 Intel Corporation
 .. *
 .. * SPDX-License-Identifier: MIT
 .. ***************************************************************************/

.. _code_examples_reference_link:

Code Samples and Examples
#########################

.. _code_examples_c_reference_link:

Low-level C API Examples
************************


cache_flush
===========


.. literalinclude:: ../../../../examples/low-level-api/cache_flush_example.c
    :language: c


compare_pattern
===============


.. literalinclude:: ../../../../examples/low-level-api/compare_pattern_example.c
    :language: c


compare
=======


.. literalinclude:: ../../../../examples/low-level-api/compare_example.c
    :language: c


copy_crc
========


.. literalinclude:: ../../../../examples/low-level-api/copy_crc_example.c
    :language: c


crc
===


.. literalinclude:: ../../../../examples/low-level-api/crc_example.c
    :language: c


delta
=====


.. literalinclude:: ../../../../examples/low-level-api/delta_example.c
    :language: c


dualcast
========


.. literalinclude:: ../../../../examples/low-level-api/dualcast_example.c
    :language: c


fill
====


.. literalinclude:: ../../../../examples/low-level-api/fill_example.c
    :language: c


mem_move
========


.. literalinclude:: ../../../../examples/low-level-api/move_example.c
    :language: c


batch
========


.. literalinclude:: ../../../../examples/low-level-api/batch_example.c
    :language: c


batch
========


.. literalinclude:: ../../../../examples/low-level-api/dif_example.c
    :language: c



Multi-Socket Library Usage
==========================


This sample shows how to utilize several sockets on the system using manual ``numa_id``
setting in the ``dml_job_t`` structure. Thus, the ``memory_move`` operation can be applied
to one data array using several sockets at the same time.


.. literalinclude:: ../../../../examples/low-level-api/multi_socket_example.c
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


batch
========


.. literalinclude:: ../../../../examples/high-level-api/batch_example.cpp
    :language: cpp



Multi-Socket Library Usage
==========================


This sample shows how to utilize several sockets on the system.
Thus, the ``memory_move`` operation can be applied
to one data array using several sockets at the same time.


.. literalinclude:: ../../../../examples/high-level-api/multi_socket_example.cpp
    :language: cpp
