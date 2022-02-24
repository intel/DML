 .. ***************************************************************************
 .. * Copyright (C) 2021 Intel Corporation
 .. *
 .. * SPDX-License-Identifier: MIT
 .. ***************************************************************************/

Low-level C API
###############


Introduction
************


This document provides instructions on how to use the Intel® Data Mover Library.
It describes general usage concepts, main entities and
detailed operation descriptions.

For general introduction to Intel® DML, see `Introduction
<../introduction_docs/introduction.html>`__.

Disclaimer
===========


The library exists in a prototype form and is being actively
developed. This means that some of the things described here may change
in the future. However, the library is released now with the expectation
that it will be more useful than nothing to people who may use it.

Header Files
************


Your application only needs to include one header file: ``dml.h``, which
includes the entire API definition.

How to Use the Library
**********************


The library operation is based on the following concepts:

- Your application needs to fill in a data structure with the necessary
  parameters to describe the job to be done. The structure is passed to
  the library and contains the results (error codes, output parameters,
  and so on) after operation.
- The Intel® DML library does not perform any internal memory allocations.
- Your application must provide all the memory buffers that are required for the execution.

Memory Allocation and Job Structure Initialization
**************************************************


As the library supports several implementations/execution paths – ``DML_PATH_HW``,
``DML_PATH_SW``, and ``DML_PATH_AUTO`` – the size of the required memory
buffer for a job structure is not predefined and depends on the used
CPU/HW and execution path.

Before submitting any job to the library, your application must
determine the size of the required memory buffer, allocate, and
initialize it. The algorithm of these steps is as follows:

1. Call the ``dml_get_job_size()`` function:

.. code-block:: c

        status = dml_get_job_size(path, job_size_ptr);

where:

- ``status`` - has type ``dml_status_t`` and contains execution status of the function
  – success or error code – see ``dml.h`` for details.
- ``path`` – has type ``dml_path_t``, which can be:

  - ``DML_PATH_HW``   – all hardware-supported features are executed by Intel® Data Streaming Accelerator
  - ``DML_PATH_SW``   – all supported features is executed by the software path of the library
  - ``DML_PATH_AUTO`` – the library automatically dispatches execution of the requested jobs either to Intel® Data Streaming Accelerator or to the software path of the library depending on internal heuristics

- ``job_size_ptr`` – has type ``uint32_t*`` (a pointer to unsigned int)
  and stores the calculated memory buffer size returned by the function.

1. Allocate the required memory buffer (size in bytes =
   ``*job_size_ptr``). The Intel® Data Mover Library does not allocate
   or free memory, therefore an application must provide required memory
   buffer for the job structure and free it after it becomes
   unnecessary. During allocation, the memory buffer should be casted to
   ``dml_job_t *data type`` – for example:

.. code-block:: c

        dml_job_ptr = (dml_job_t *) malloc(*job_size_ptr);

Check allocation results for success.

3. Call the ``dml_init_job()`` function:

.. code-block:: c

        status = dml_init_job(path, dml_job_ptr);

where:

-  ``status`` - has type ``dml_status_t`` and contains execution status
   of the function: success or error code. See ``dml.h`` for details.
-  ``path`` - has type ``dml_path_t`` and supports the following values:

   -  ``DML_PATH_HW`` – all hardware-supported features are executed
      by Intel® Data Streaming Accelerator
   -  ``DML_PATH_SW`` – all supported features are executed by the
      software path of the library
   -  ``DML_PATH_AUTO`` – the library automatically dispatches execution
      of the requested jobs either to accelerator or to the software
      path of the library depending on internal heuristics

   ..

      **NOTE:** It shall have the same value as at the first step – the
      call to ``dml_get_job_size()``.

-  ``dml_job_ptr`` – has type dml_job_t (Intel DML job structure) and
   stores the initialized Intel DML job structure returned by the
   function.

An example of C code for these first steps:

.. code-block:: c

       uint32_t size;
       dml_job_t *dml_job_ptr;
       dml_status_t status;

       status  = dml_get_job_size(DML_PATH_AUTO, &size );
       dml_job_ptr = (dml_job_t *) malloc( size );
       status  = dml_init_job(DML_PATH_AUTO, 0u, dml_job_ptr);

At this stage, the application has the initialized job structure and can
request any library operation it needs.

Job Execution
*************


The simplest interface is ``dml_execute_job()``. It does not return any
result until the job has completed. It is the “synchronous” interface.

If the application wants to do other work while the job is being
processed, it can use the “asynchronous” interface. The job is submitted
with ``dml_submit_job()``. The application can then periodically query
the status of the job with ``dml_check_job()``. If the application needs
to wait until the job completes, it can call ``dml_wait_job()``.

Note that the ``dml_execute_job()`` function is essentially a
combination of ``dml_submit_job()`` followed by ``dml_wait_job()``.

In the context of the behavioral model (i.e. without actual hardware
support), the job is processed when it is submitted, and so the
``dml_wait_job()`` function always returns completed status. It is not
the case when real hardware is involved.

The job structure contains three types of data:

#. Parameters from the application to the library defining the job to be
   done;

#. Results from the library to the application;

#. Internal state.

In some cases, a larger overall task may be broken into a series of
separate library calls. For example, an application copying a large
amount of data might call the library repeatedly with 64kB buffers,
until the end of the data buffer is reached. The mechanism used to
relate these calls to each other is that they all use the same job
structure. As it will be described later, flags are used to indicate
whether a particular job submission is the start of a new overall task
or whether it is continuing a previous one. Because of it, there is no
separate “init” function; the initialization is implied by the job’s
parameters.

There are also a number of auxiliary functions, which will be described
as they become relevant.

A number of examples can be found in the `Examples <#examples>`__ section.

Low Bandwidth Memory
********************


The Library includes features to improve system performance when
accessing memory with lower bandwidth or higher latency than DRAM. To
use these features, WQs must be configured to work with various traffic
classes (TC). The next flags allow you to choose which of the TCs to use
for the operation:

- ``DML_FLAG_ADDRESS1_TCB`` - for most operation types selects TC-B for reads from the source address
- ``DML_FLAG_ADDRESS2_TCB`` - for most operation types selects TC-B for writes to the destination address
- ``DML_FLAG_ADDRESS3_TCB`` - specific flag that is interpreted differently for each operation

You can read more about the flags in our docs

Persistent Memory Support
*************************


The Library provides several features intended to improve its utility
with persistent memory:

1. The Steering Tag selector - allows user to select platform-defined
   steering tag. In the Library it is represented by two flags:

   -  ``DML_FLAG_DST1_DURABLE`` - writes to the first destination are
      identified as writes to durable memory
   -  ``DML_FLAG_DUALCAST_DST2_DURABLE`` - specific flag for memory copy
      with dualcast operation. Writes to the second destination are
      identified as writes to durable memory

You can read more about the flags in our docs

NUMA support
*************************

The library is NUMA aware and respects the NUMA node id of the calling thread. If a user needs to use a device from a specific node, it can be done in two ways:

- Pin thread which performs submissions to the specific NUMA, the library will use devices only from this node.
- Set NUMA id parameter of the job to the specific node id, then devices will be selected only from this node.

Load balancer of the library doesn't cross a detected or specified NUMA boundary. Balancing workloads between different nodes is the responsibility of a user.


Page Fault handling
*************************


Page fault can occur during workload processing on a hardware. When the fault occurs, the outcome is based on the execution path:
- Hardware path: ``DML_STATUS_PAGE_FAULT_ERROR`` is returned.
- Auto path: the library resolves the fault via completion of the workload on a CPU.


Basic Fields of Job Structure
*****************************


A complete description of the job structure fields can be found in
Section (N).

.. list-table::
   :header-rows: 1

   * - Field Name
     - Description
   * - ``source_first_ptr``
     - The starting point of the input data.
   * - ``source_length``
     - Length of the input data.
       On completion, the ``source_length`` field contains the number of
       bytes remained unprocessed in case of any library/HW issue, or it
       contains zero if everything is successful.
   * - ``source_second_ptr``
     - The starting point of the input data in case if the second buffer is
       required (length is described with ``source_length``).
   * - ``destination_first_ptr``
     - The starting point of the resulting data.
   * - ``destination_length``
     - Length of the resulting data.
   * - ``destination_second_ptr``
     - The starting point of the resulting data
       in case if the second output buffer is required (length is described
       with ``destination_length``).
   * - ``operation``
     - Enumerated field that defines that operation to be performed.
   * - ``flags``
     - Flags that affect the operation behavior.
   * - ``crc``
     - Sets CRC initial value (seed). This field also returns the calculated CRC value.
   * - ``pattern[8]``
     - An array of patterns for the Compare Pattern operation.
   * - ``numa_id``
     - NUMA node id which is used to select a device.

Operations
**************


Job Flow Control Features
============================


No-op operation
---------------


The No-op operation can be used with the ``Fence`` flag in the batch to
ensure that all previous operations in the batch completed. It performs
no any other operation.

Batch operation
---------------


The Batch operation processes multiple jobs at once. Batch operation has
sequence of operations at ``destination_first_ptr``, the size of memory
region for a specific number of operations can be calculated via
``dml_get_batch_size``. Operations can be added to the batch via set of
set-functions:

-  ``dml_batch_set_nop_by_index``
- ``dml_batch_set_mem_move_by_index``
- ``dml_batch_set_dualcast_by_index``
- ``dml_batch_set_compare_by_index``
- ``dml_batch_set_compare_pattern_by_index``
- ``dml_batch_set_crc_by_index``
- ``dml_batch_set_copy_crc_by_index``
- ``dml_batch_set_fill_by_index``
- ``dml_batch_set_cache_flush_by_index``
- ``dml_batch_set_delta_create_by_index``
- ``dml_batch_set_delta_apply_by_index``
- ``dml_batch_set_dif_check_by_index``
- ``dml_batch_set_dif_update_by_index``
- ``dml_batch_set_dif_insert_by_index``
- ``dml_batch_set_dif_strip_by_index``

Result and status of specific operation can be obtained via
get-functions:

- ``dml_batch_get_result``
- ``dml_batch_get_status``

The maximum number of jobs available in a batch can be obtained with the service function:
``dml_status_t dml_get_limits(dml_job_t *dml_job_ptr, dml_limits_t *dml_limits_ptr)``.

Drain
-----


Use the Drain operation to wait for all jobs completion before exiting.
It cannot be a part of batch and is treated as unsupported operation
type for a batch.

Memory Transferring and Filling Features
==========================================


Memory Move
-----------


The Memory Move operation copies (moves) memory from
``source_first_ptr`` to ``destination_first_ptr``. The number of moved
bytes is given by the ``source_length`` parameter.

-  The ``destination_length`` parameter must contain the size of
   destinations (for internal validation).
-  The ``source_length`` parameter must contain count of bytes to copy.

.. note::

   By default, the Memory Move operation is enabled for overlapping
   buffers. If you need to disable Move and enable only Copy behavior,
   you should set the ``DML_FLAG_COPY`` flag.


Memory Copy with Dualcast
-------------------------


The Memory Copy with Dualcast operation copies memory from the
``source_first_ptr`` to both ``destination_first_ptr`` and
``destination_second_ptr``.

- The ``destination_length`` parameter must contain the size of destinations (for internal validation).
- The ``source_length`` parameter must contain count of bytes to copy.
- There are no alignment requirements for ``source_first_ptr`` and ``source_length``.
- For ``destination_first_ptr`` and ``destination_second_ptr``, the 11:0 bits must be the same.

.. note::

   Move operation is not supported for Memory Copy with Dualcast. If
   ``source_first_ptr``, ``destination_first_ptr`` and
   ``destination_second_ptr`` overlap in any combination, the error is
   thrown.

Fill
''''


The Memory Fill operation fills the memory at ``destination_first_ptr``
with the bytes from the ``pattern[8]`` parameter.

- The ``destination_length`` parameter must contain the number of bytes to fill.
- The ``pattern`` size is always 8 bytes.
- The ``source_length`` parameter does not need to be a multiple of 8.

Memory Compare and Restoring Features
=======================================


Compare
-------


The Memory Compare operation compares memory at ``source_first_ptr``
with memory at ``source_second_ptr``.

-  The number of bytes compared is given by the ``source_length``
   parameter.
-  You can configure the operation with the ``DML_FLAG_CHECK_RESULT``
   flag and the ``expected_result`` field to implement
   ``is_equal_memory`` and ``is_not_equal_memory``. This allows
   subsequent jobs in the same batch with the DML_FLAG_FENCE flag to
   continue or stop execution of the batch based on the result of
   comparison.

.. list-table::
   :header-rows: 1

   * - ``DML_FLAG_CHECK_RESULT``
     - ``expected_result`` value
     - Operational Goal
   * - 0
     - X
     - Check if memory regions are equal
   * - 1
     - 0
     - Check if memory regions are equal
   * - 1
     - 1
     - Check if memory regions aren't equal

- If memory regions match each other, the result field contains 0, otherwise, it has 1.
- If ``result == 1``, the ``offset`` field contains the offset of the first difference.
- If operation is successful and ``DML_FLAG_CHECK_RESULT`` flag is set, the ``result`` field bit 1 is set according to the table below.

.. list-table::
   :header-rows: 1

   * - ``DML_FLAG_CHECK_RESULT``
     - ``expected_result`` value
     - result bit-0
     - result bit-1
     - meaning
   * - 0
     - X
     - X
     - 0
     - EQUAL(expected result)
   * - 1
     - 0
     - 0
     - 0
     - EQUAL(expected result)
   * - 1
     - 0
     - 1
     - 1
     - NOT EQUAL(expected result)
   * - 1
     - 1
     - 0
     - 1
     - NOT EQUAL(not expected result)
   * - 1
     - 1
     - 1
     - 0
     - EQUAL(not expected result)

Compare Pattern
---------------


The Compare Pattern operation compares memory at ``source_first_ptr``
with the value in the pattern field.

- If ``source_first_ptr`` memory region matches the pattern, then the
  result field contains 0, otherwise it has 1.
- If ``result == 1``, the ``offset`` field contains the offset of the
  first difference. (It may not be the exact byte location, but it is
  guaranteed to be no greater than the first difference).
- The ``source_length`` parameter does not need to be a multiple of 8.

.. note::

   This operation can be used with the ``DML_FLAG_CHECK_RESULT`` flag and
   ``expected_result`` field. You can find mode details in the Compare
   Operation section.

Create Delta Record
-------------------


The Create Delta Record operation compares memory at
``source_first_ptr`` with memory at ``source_second_ptr`` and generates
a delta record containing the information needed to update
``source_first_ptr`` to match ``source_second_ptr``. The number of bytes
compared is given by the ``source_length`` parameter.

- The delta record is written into the address specified in
  ``destination_first_ptr``. Destination buffer size must be set in the
  ``destination_length`` field.
- The ``source_length`` parameter is limited by the maximum offset that
  can be stored in the delta record.

.. note::

   ``source_first_ptr``, ``source_second_ptr`` and ``source_length``
   must be 8-byte aligned (multiple of 8).

   ``destination_length`` must be a multiple of 10.


Delta record format is described below:

.. list-table::

   * - Off set
     - Data byte 0
     - Data byte 1
     - Data byte 2
     - Data byte 3
     - Data byte 4
     - Data byte 5
     - Data byte 6
     - Data byte 7
   * - Off set 1
     - Data byte 0
     - Data byte 1
     - Data byte 2
     - Data byte 3
     - Data byte 4
     - Data byte 5
     - Data byte 6
     - Data byte 7
   * - ...
     - ...
     - ...
     - ...
     - ...
     - ...
     - ...
     - ...
     - ...
   * - Off set n
     - Data byte 0
     - Data byte 1
     - Data byte 2
     - Data byte 3
     - Data byte 4
     - Data byte 5
     - Data byte 6
     - Data byte 7

The maximum size of the delta record is given by the
``dml_limits_t.dml_max_delta_record_size`` field (service function
``dml_status_t dml_get_limits(dml_job_t* dml_job_ptr, dml_limits_t* dml_limits_ptr)``).

The delta records are written to ``destination_first_ptr``, therefore it
must point to the memory buffer of the appropriate size. The ``offset``
field of the delta record has ``uint16_t`` data type, therefore the size
of the delta record must be:

- a multiple of the delta size (10 bytes)
- less than the maximum number of deltas that can be generated from a single cache line (80 bytes)
- not greater than the value allowed by the ``dml_limits_t.dml_max_transfer_size``

The actual size of the generated delta record depends on the number of
differences detected. This size is written to the ``destination_length``
field of the ``dml_job_t`` structure. The result of comparison is
written to the ``result`` field of the ``dml_job_t`` structure.

1. If two regions match exactly, then the ``result`` field is 0,
   ``bytes_processed`` field is 0, ``destination_length`` field is 0.

2. If two regions do not match, and a complete set of deltas is written
   to the delta record, then the result field is 1, the ``offset`` field
   contains the first difference’s offset, and the
   ``destination_length`` field contains byte size of the delta record.

3. If two regions do not match, and the space needed to record all the
   deltas exceeds the significant ``destination_length``, then result is
   2, when:

   -  ``destination_length`` contains a created delta record size before
      ``destination_first_ptr`` buffer size has been overflowed
   -  ``source_length`` contains the remaining number of bytes to
      compare
   -  ``offset`` field contains the first difference’s offset

.. note::

   Since the ``offset`` field in the delta record is ``uint16_t``
   representing a multiple of 8 bytes, the maximum offset that can be
   expressed is ``0x7fff8``, so the maximum transfer size (the
   ``source_length`` parameter) is ``0x80000`` bytes (512KB).

   This operation can be used with the ``DML_FLAG_CHECK_RESULT`` flag and
   ``expected_result`` field. Mode details are in the Compare Operation
   description.

Apply Delta Record
------------------


The Apply Delta Record operation applies a delta record at
``source_first_ptr`` to ``destination_first_ptr``. The delta record must
be created by the Create Delta Record operation with ``result`` equal to
1.

-  The ``destination_length`` parameter must contain the size of memory
   region to update.
-  The ``source_length`` parameter must contain the delta record size as
   returned to the ``destination_length`` field after the Create Delta
   Record operation.

Memory Hash Features
========================


CRC Generation
--------------


The CRC Generation computes the CRC on memory at ``source_first_ptr``.
The number of bytes used for CRC computation is given by the
``source_length`` parameter. The ``crc_ptr`` field in the ``dml_job_t``
is used as a seed for CRC calculation, therefore it can be used for
continuation.

This operation computes CRC using the polynomial ``0x11edc6f41`` (iSCSI
Protocol, RFC 3720). When the ``source_length`` parameter is not a
multiple of 4 bytes, the source data is padded to the end with zeros.

Copy with CRC Generation
------------------------


The Copy with CRC Generation operation copies memory from
``source_first_ptr`` to ``destination_first_ptr`` and computes CRC on
the data copied.

The number of bytes copied is given by the ``source_length`` parameter.
This operation runs as a sequential call of two operations.

If ``source_first_ptr`` and ``destination_first_ptr`` overlap, the error
is thrown.

Data Integrity Field Features
=================================


The Data Integrity Field (DIF) provides a system solution to protect the
communication path between a host and storage device for end-to-end
integrity.

Data block protected with DIF has the following format:

- Data block (512, 520, 4096 or 4104 bytes)
- Reference tag (4 bytes)
- Meta tag (2 bytes)
- Guard (2 bytes)

where:

-  Data block contains some binary data.
-  The Reference Tag nominally contains information associated with a
   specific data block within some context, such as the lower 4 bytes of
   the Logical Block Address.
-  The Meta Tag (called as Application in Intel DML context) contains
   additional context information that is nominally held fixed within
   the context of a given I/O operation.
-  Guard field is computed from the data in the standard data block with
   CRC16.

You can find more information about End-to-End Data Protection
Justification with DIF
`here <https://www.t10.org/ftp/t10/document.03/03-224r0.pdf>`__.

**How to configure Data Integrity Field Operation**

You can configure all the Intel DML operations that work with data
protected with DIF using the ``dml_dif_config_t`` structure. This
structure is provided as a field of ``dml_job_t``.

.. code-block:: c

   typedef struct
   {
       uint32_t flags;
       uint32_t source_reference_tag_seed;
       uint16_t source_application_tag_seed;
       uint16_t source_application_tag_mask;
       uint32_t destination_reference_tag_seed;
       uint16_t destination_application_tag_seed;
       uint16_t destination_application_tag_mask;
       dml_dif_block_size_t block_size;
   } dml_dif_config_t;

1. ``block_size`` - field can be used to set the size of a block that is
   protected with DIF. All possible data block sizes are presented
   below:

   .. code-block:: c

      typedef enum
      {
          DML_DIF_BLOCK_SIZE_512,     /**< Size is 512 bytes  */
          DML_DIF_BLOCK_SIZE_520,     /**< Size is 520 bytes  */
          DML_DIF_BLOCK_SIZE_4096,    /**< Size is 4096 bytes */
          DML_DIF_BLOCK_SIZE_4104     /**< Size is 4104 bytes */
      } dml_dif_block_size_t;

2. The ``target_reference_tag_seed`` and ``target_application_tag_seed``
   fields contain values for tags of the first data block (seed). For
   each operation, fill in these fields in accordance with the table
   below:

   .. list-table::
      :header-rows: 1

      * - Operation
        - Source Tag Seeds
        - Destination Tag Seeds
      * - DML_OP_DIF_CHECK
        - Required
        - Not required
      * - DML_OP_DIF_INSERT
        - Not required
        - Required
      * - DML_OP_DIF_STRIP
        - Required if DIF check enabled
        - Not required
      * - DML_OP_DIF_UPDATE
        - Required if DIF check enabled
        - Required

3. The ``target_aplication_tag_mask`` field can be used to mask some
   application tag bits. Note that 1 bit in the mask means that the
   corresponding bit is masked in the tag. For example, 0xFE masks all
   application tag bits except the first low bit.

4. With the ``flags`` field, you can thoroughly configure the operation
   behavior. Flags are separated by groups and presented below.

   1. СRC generation can be configured with the following flags:

      .. list-table::
         :header-rows: 1

         * - Flag
           - Description
         * - ``DML_DIF_FLAG_INVERT_CRC_SEED``
           - The initial crc seed is ``0xFFFF``
         * - ``DML_DIF_FLAG_INVERT_CRC_RESULT``
           - Invert CRC result

   2. Source DIF can be configured with following flags:

      .. list-table::
         :header-rows: 1

         * - Flag
           - Description
         * - ``DML_DIF_FLAG_SRC_F_ENABLE_ERROR``
           - Throw an error on condition set by the ``DML_DIF_FLAG_SRC_F_DETECT_ALL`` flag
         * - ``DML_DIF_FLAG_SRC_F_DETECT_ALL``
           - Detect if all DIF bytes are equal to ``0xFF`` in the source
         * - ``DML_DIF_FLAG_SRC_F_DETECT_APP_TAG``
           - Skip check of protected block if Application tag is equal to ``0xFF`` on the source
         * - ``DML_DIF_FLAG_SRC_F_DETECT_TAGS``
           - Skip check of protected block if Application and Reference tag bytes is equal to ``0xFF`` in the source
         * - ``DML_DIF_FLAG_SRC_INC_APP_TAG``
           - Source Application format: Fixed(0) or Incrementing(1)
         * - ``DML_DIF_FLAG_SRC_GUARD_CHECK_DISABLE``
           - Source Check Guard filed: Enabled (0) or Disabled (1)
         * - ``DML_DIF_FLAG_SRC_REF_TAG_CHECK_DISABLE``
           - Source Check Reference tag: Enabled (0) or Disabled (1)
         * - ``DML_DIF_FLAG_SRC_FIX_REF_TAG``
           - Source Reference TAG format: Incrementing(0), Fixed(1)

   3. Destination DIF can be configured with following flags:

      .. list-table::
         :header-rows: 1

         * - Flag
           - Description
         * - ``DML_DIF_FLAG_DST_PASS_APP_TAG``
           - Destination Application calculation: Enabled(0) or Disabled (1 Copy from Source)
         * - ``DML_DIF_FLAG_DST_INC_APP_TAG``
           - Destination Application tag format: Fixed(0), Incrementing(1)
         * - ``DML_DIF_FLAG_DST_PASS_GUARD``
           - Destination Guard calculation: Enabled(0) or Disabled (1 Copy from Source)
         * - ``DML_DIF_FLAG_DST_PASS_REF_TAG``
           - Destination Reference calculation: Enabled(0) or Disabled (1 Copy from Source)
         * - ``DML_DIF_FLAG_DST_FIX_REF_TAG``
           - Destination Reference tag format: Incrementing(0) or Fixed(1)

DIF Check
---------


The DIF Check operation computes the Data Integrity Field (DIF) on the
source data and compares the computed DIF to the DIF contained in the
source data (``source_first_ptr``).

- The number of source bytes read is given by the ``source_length``
  parameter. The DIF computation is performed on each block of source
  data that is 512, 520, 4096, or 4104 bytes.
- The ``source_length`` parameter should be a multiple of the source
  block size plus 8 bytes for each source block.
- If an error is detected in the DIF in the source data, the operation
  stops.
- The ``result`` field contains a type of the DIF error in flag format
  (see the table below for more information) and the ``offset`` field
  contains the number of bytes successfully processed (it does not
  include the block in which the error was detected).

.. list-table::
   :header-rows: 1

   * - Hex Value
     - Result Flag Name
     - Description
   * - ``0x 01``
     - ``DML_DIF_CHECK_GUARD_MISMATCH``
     - Guard mismatch. This value is reported under the following conditions:

       - Guard Check Disable is 0
       - F Detect condition is not detected
       - The guard value computed from the source data does not match the Guard field in the source Data Integrity Field

   * - ``0x 02``
     - ``DML_DIF_CHECK_APPLICATION_TAG_MISMATCH``
     - Application Tag mismatch. This value is reported under the following conditions:

       - Source Application Tag Mask is not equal to ``0xFFFF``
       - F Detect condition is not detected
       - The computed Application Tag value does not match the Application Tag field in the source Data Integrity Field

   * - ``0x 04``
     - ``DML_DIF_CHECK_REFERENCE_TAG_MISMATCH``
     - Reference Tag mismatch. This value is reported under the following conditions:

       - Reference Tag Check Disable is 0
       - F Detect condition is not detected
       - The computed Application Tag value does not match the Application Tag field in the source Data Integrity Field

   * - ``0x 08``
     - ``DML_DIF_CHECK_ALL_BITS_SET_DETECT_ERROR``
     - All F Detect Error. This value is reported under the following conditions:

       - All F Detect is 1
       - Enable All F Detect Error is 1
       - All bits of the Application Tag, Reference Tag, and Guard fields of the source Data Integrity Field are equal to 1


DIF Insert
----------


The DIF Insert operation copies memory from the ``source_first_ptr`` to
``destination_first_ptr`` (length bytes) while computing the DIF on the
source data and inserting the DIF into the output data.

-  The number of bytes copied is given by the ``source_length``
   parameter.
-  The number of available bytes in ``destination_first_ptr`` is given
   by the ``destination_length`` parameter.
-  The ``destination_length`` is the number of bytes that are written to
   ``destination_first_ptr`` (can be calculated as ``source_length``
   plus 8 bytes for each source block).
- If ``source_first_ptr`` and ``destination_first_ptr`` overlap, the error is thrown.


DIF Strip
---------


The DIF Strip operation copies memory from ``source_first_ptr`` to
``destination_first_ptr``, removing the DIF.

Additionally, it computes the DIF value on ``source_first_ptr`` data and
compares to the DIF contained in ``source_first_ptr`` according to the
DIF Source Flags values.

-  The number of bytes copied is given by the ``source_length``
   parameter and must be a multiple of block size plus 8 for each source
   block.
-  The number of bytes available to write into destination buffer must
   be set into ``destination_size``.
-  The ``destination_size`` field after job execution contains the
   successfully written bytes to ``destination_first_ptr`` (the result
   size is always ``source_length`` minus 8 bytes for each source
   block).
- If ``source_first_ptr`` and ``destination_first_ptr`` overlap, the error is thrown.
- If an error is detected in the DIF in the source data, the operation
   stops. The job structure is updated in the same way as in the DIF Check operation.


DIF Update
----------


The DIF Update operation copies memory from ``source_first_ptr`` to
``destination_first_ptr`` and updates DIFs in accordance with
destination flags. (length bytes).

Additionally, it computes the DIF value on the ``source_first_ptr`` data
and compares it to the DIF contained in the ``source_first_ptr``
according to the DIF Source Flags values.

-  The number of bytes copied is given by the ``source_length``
   parameter.
-  The ``destination_length`` parameter must contain the size of
   destinations (for internal validation).
- If ``source_first_ptr`` and ``destination_first_ptr`` overlap, the error is thrown.
- If an error is detected in the DIF in the source data, the operation stops.
   The job structure is updated in the same way as in the DIF Check operation.


Cache Flush
-----------


The Cache Flush operation flushes the processor caches at the
``destination_first_ptr`` address. The number of bytes flushed is given
by the ``destination_length`` parameter.

-  If ``DML_FLAG_CACHE_CRTL`` flag is not set, the affected cache lines
   are invalidated from every level of the cache hierarchy. If it is set
   to 1, then modified cache lines are written to main memory, but are
   not evicted from the caches.

Job Structure Description
*************************


The Job structure is defined in ``dmldefs.h``.

.. list-table::
  :header-rows: 1

  * - Field
    - Type
    - Description
  * - ``source_first_ptr``
    - IN
    - Pointer to the next input byte in input stream #1
  * - ``source_second_ptr``
    - IN
    - Pointer to the next input byte in input stream #2
  * - ``destination_first_ptr``
    - OUT
    - Pointer to the next output byte in output stream #1
  * - ``destination_second_ptr``
    - OUT
    - Pointer to the next output byte in output stream #2
  * - ``crc_ptr``
    - IN/OUT
    - CRC for OUT, CRC seed for IN
  * - ``source_length``
    - IN
    - Number of bytes in source to process
  * - ``destination_length``
    - IN
    - Available bytes count in destination buffer
  * - ``offset``
    - OUT
    - Offset field for operations
  * - ``pattern``
    - IN
    - Pattern for operations with pattern
  * - ``operation``
    - IN
    - Operation to be done
  * - ``flags``
    - IN
    - Flags indicating specific modes of operations
  * - ``result``
    - OUT
    - Result field for some operation where status is not enough
  * - ``expected_result``
    - IN
    - Expected result for some operations
  * - ``dif_tag_seed``
    - IN
    - Initial tag seed for DIF operations
  * - ``spec``
    - IN
    - Internal spec structure


Operation Status Values
***********************


The return values are defined in ``dmldefs.h``.

.. doxygenenum:: dml_status_t
  :project: Intel DML
  :no-link:
