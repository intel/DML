 .. ***************************************************************************
 .. * Copyright (C) 2021 Intel Corporation
 .. *
 .. * SPDX-License-Identifier: MIT
 .. ***************************************************************************/

Quick start
###########


The quick start sample below shows how to compute CRC on the data
using Intel® Data Mover Library (Intel® DML).
See `CRC Generation <../api_docs/high_level_api.html#crc-generation>`__
operation for more information.

.. literalinclude:: ../../../../examples/high-level-api/crc_example.cpp
    :language: cpp

In order to build the library and all the examples, including the one above,
follow the steps at :ref:`building_library_reference_link`.
The compiled examples will then be located in ``<dml_library>/build/examples/``.

To run the example on the Hardware Path, use:

.. code-block:: shell

   sudo ./compression_example hardware_path

Similarly you can specify ``software_path`` for host execution or ``automatic_path``
for automatic dispatching (choice would be made by library based on accelerator availability
and some other internal heuristics).

.. attention::

   With the Hardware Path, the user must either place the ``libaccel-config`` library in ``/usr/lib64/``
   or specify the location of ``libaccel-config`` in ``LD_LIBRARY_PATH`` for the dynamic loader to find it.

.. attention::

   Hardware Path requires first configuring Intel® Data Streaming Accelerator (Intel® DSA).
   See :ref:`Accelerator Configuration <accelerator_configuration_reference_link>`.