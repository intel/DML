 .. ***************************************************************************
 .. * Copyright (C) 2021 Intel Corporation
 .. *
 .. * SPDX-License-Identifier: MIT
 .. ***************************************************************************/

Quick Start
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

.. attention::

   Hardware Path requires first configuring Intel® Data Streaming Accelerator (Intel® DSA).
   Please refer to the :ref:`Accelerator Configuration <accelerator_configuration_reference_link>` section for detailed instructions.

   Starting from **Intel DML 1.2.0** release, the library will select any device from the socket of the calling thread for execution.
   Prior to this release, the library would only select devices from the NUMA node of the calling thread.

   If more fine-grained control is needed, the Low-Level API of the library provides the ability to select devices
   from a specific NUMA node using the ``numa_id`` field in the job structure.
   For more information, see the :ref:`NUMA support for Low-Level API <library_numa_support_ll_reference_link>` section.

   It is the user's responsibility to ensure that the devices are properly configured and available for the library to use.

   Additionally, with the Hardware Path, the user must either place the ``libaccel-config`` library in ``/usr/lib64/``
   or specify the location of ``libaccel-config`` in the ``LD_LIBRARY_PATH`` environment variable for the dynamic loader to find it.

Similarly you can specify ``software_path`` for host execution or ``automatic_path``
for automatic dispatching (choice would be made by library based on accelerator availability
and some other internal heuristics).