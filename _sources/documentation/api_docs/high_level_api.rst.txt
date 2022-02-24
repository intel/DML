 .. ***************************************************************************
 .. * Copyright (C) 2021 Intel Corporation
 .. *
 .. * SPDX-License-Identifier: MIT
 .. ***************************************************************************/

High-level C++ API
##################

Introduction
************

This document provides quick introduction into usage of High Level API
for Intel® DML. It describes general usage concepts, main entities and
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


Your application only needs to include one header file: ``dml/dml.hpp``,
which includes the entire API definitions.

Namespace
*********


All API declarations are included into ``dml`` namespace.

Identifiers from ``dml::ml`` and ``dml::detail`` namespaces shall not be
used. They are implementation details and are subject to change
frequently.

NUMA support
*************************


The library is NUMA aware and respects the NUMA node id of the calling thread. If a user needs to use a device from a specific node, it can be done in one way:

- Pin thread which performs submissions to the specific NUMA, the library will use devices only from this node.

Load balancer of the library doesn't cross a detected NUMA boundary. Balancing workloads between different nodes is the responsibility of a user.


Page Fault handling
*************************


Page fault can occur during workload processing on a hardware. When the fault occurs, the outcome is based on the execution path:
- Hardware path: ``dml::status_code::partial_completion`` is written to result structure.
- Auto path: the library resolves the fault via completion of the workload on a CPU.

How to Use the Library
***********************


The library usage is based on the following concepts:

- Your application should call either ``dml::submit`` or ``dml::execute``
  function to evaluate an operation.
- Operation type, input data, and input parameters are
  all configured via function arguments.
- Execution path for an operation is provided via the first template argument of
  ``dml::submit`` and ``dml::execute`` functions. It can be on of the
  following: ``dml::software``, ``dml::hardware``.
- Memory regions (data) should be passed as ``dml::data_view`` objects.

Example usage
==============


Here is the generic flow for the library (synchronous execution):

.. code-block:: cpp

   // Some data definitions
   auto src_data = std::vector<uint8_t>{...};
   auto dst_data = std::vector<uint8_t>(src_data.size());

   // Use an operation
   auto result = dml::execute<dml::software>(dml::mem_move,
                                             dml::make_view(src_data),
                                             dml::make_view(dst_data));

   if (result.status != dml::status_code::ok)
   {
       return -1;
   }

Here is the generic flow for the library (asynchronous execution):

.. code-block:: cpp

   // Some data definitions
   auto src_data = std::vector<uint8_t>{...};
   auto dst_data = std::vector<uint8_t>(src_data.size());

   // Use operation
   auto handler = dml::submit<dml::software>(dml::mem_move,
                                             dml::make_view(src_data),
                                             dml::make_view(dst_data));

   // Evaluate other asynchronous operations...

   // Wait for the result
   auto result = handler.get();

   if (result.status != dml::status_code::ok)
   {
       return -1;
   }


.. note::

   Use ``dml::hardware`` to evaluate an operation on a dedicated
   hardware.


Make view
=========


All operations handle user’s data via ``dml::data_view`` so there are
functions to constructs one:

- View from a raw pointer and a size.
- View from a pair of iterators
- View from a range (``.begin()`` and ``.end()`` pair)

Usage:

.. code-block:: cpp

   auto data = std::vector<uint8_t>{1, 2, 3};

   auto view_from_raw_pointer = dml::make_view(data.data(), data.size);
   auto view_from_iterators = dml::make_view(data.begin(), data.end());
   auto view_from_range = dml::make_view(data);


.. note::

   Currently the library uses only ``uint8_t`` views.


Operation Data Type
===================


Operations are represented via
`literal-type <https://en.cppreference.com/w/cpp/named_req/LiteralType>`__
classes and instances of those classes, for example
``dml::mem_move_operation`` and ``dml::mem_move``. Detailed description
for each operation can be found below.

Operations are used for tag-dispatching of ``dml::submit`` and
``dml::execute`` functions, and also for passing additional parameters.
Several operations are configurable via set of methods. Those methods
construct a new object with specified feature enabled, they can be
chained.

Example (pseudocode)

.. code-block:: cpp

   auto result = dml::execute(dml::some_operation.enable_feature1().enable_feature3(),
                              args1, args2, args3);


.. _sequence_cpp_api:

Sequence
========


You should prepare ``dml::sequence`` to use the ``dml::batch``
operation. It is a special container for ready to be evaluated
operations. You can construct the one by passing number of operations
and allocator. After construction, operations can be appended via
``.add()`` method. The method has similar to ``dml::execute`` interface,
but it stores an operation instead of starting evaluation.

Add method returns status code, reporting successful (or not)
completion. If an error occurred during an attempt to add an
operation to a sequence, then the sequence remains the same as it was
before the call.

Example:

.. code-block:: cpp

   auto sequence = dml::sequence(op_count, std::allocator<dml::byte_t>());

   auto status = sequence.add(dml::mem_move, src_view, dst1_view);
   if (status != dml::status_code::ok)
   {
       return -1;
   }

   status = sequence.add(dml::fill, pattern, dst2_view);
   if (status != dml::status_code::ok)
   {
       return -1;
   }


Handler
=======


Each ``dml::submit`` function returns ``dml::handler``. This object
provides a handle to a concurrently running operation. When it is
needed to wait for the finish and get the result, there is a ``.get()``
method provided.


Results
=======


Each operation has the corresponding result type. Results are C-structs
with at least one status field. Use cases for the results are:

- Check whether an operation is completed successfully or not
- Check what amount of work has been done before the failure occurred (if applicable)
- Check specific operation’s result (e.g. computed crc, mismatch position for compare operations)
- Use as input to other operation (e.g create_delta -> apply_delta)

.. note::

   There are several operations, which reuse other operation’s result
   structure: ``dml::crc`` and ``dml::copy_crc``, ``dml::compare`` and
   ``dml::compare_pattern``


Advanced usage
==============


The function ``dml::submit`` can be configured for custom thread and
memory management. The configuration is done via
``dml::execution_interface`` class. Each submit-function is provided
with default execution interface for each supported execution path
(``dml::software`` and ``dml::hardware``).

Default behavior for **software** execution path is:

- Uses ``std::thread(task).detach()`` to evaluate operations in a thread.
- Uses ``std::allocator`` to manage memory.

Default behavior for **hardware** execution path is:

- Uses ``task()`` to submit a task sequentially onto asynchronously working dedicated hardware.
- Uses ``std::allocator`` to manage memory.

You can use custom execution interface. To create the one use:

.. code-block:: cpp

   auto my_thread_pool = get_my_pool();
   auto my_allocator = get_my_alloc();

   // The first argument is a lambda that takes a task and call it in a custom way.
   // The second argument is an instance of an allocator.
   auto my_exec_iface = dml::execution_interface(
       [&my_thread_pool] (auto&& task)
       {
           my_thread_pool.push(std::forward<decltype(task)>(task));
       },
       my_allocator);

Every ``dml::submit`` function has ``dml::execution_interface`` as the
last argument:

.. code-block:: cpp

   auto handler = dml::submit<dml::software>(dml::mem_move,
                                             dml::make_view(src_data),
                                             dml::make_view(dst_data),
                                             my_exec_iface);

.. note::

   The library supports STL-compliant allocators.


Operations
**********


The library supports several groups of operations:

- Flow Control
- Memory Transferring and Filling
- Memory Compare and Restore
- Memory Hash
- Utility

Flow Control Features
=====================


This group of operations is used for configuration of operation
sequences.

No-op operation
---------------


Not implemented

Batch operation
---------------


This operation process multiple operations at once. Sequence of
operations to process is provided via ``sequence`` argument.

The operation is presented as ``dml::batch`` object.

Usage:

.. code-block:: cpp

   auto result = dml::execute<dml::software>(dml::batch, sequence);

Result for this operation is:

.. code-block:: cpp

   struct batch_result
   {
       status_code status{status_code::error}; /**< @todo */
       size_t      operations_completed{};     /**< @todo */
   };

See detailed :ref:`sequence_cpp_api` description.


Memory Transferring and Filling Features
========================================


This group of operations is used for memory moving, copying and filling.


Memory Move
-----------


This operation moves or copies data from a memory region represented via
``src_view`` to a memory region represented via ``dst_view``.

The operation is presented as ``dml::mem_move`` object.

Usage:

.. code-block:: cpp

   auto result = dml::execute<dml::software>(dml::mem_move, src_view, dst_view);

Result for this operation is:

.. code-block:: cpp

   struct mem_move_result
   {
       status_code status{status_code::error}; /**< @todo */
       result_t    result{};                   /**< @todo */
   };


Memory Copy with Dualcast
-------------------------


This operation copies data from a memory region represented via
``src_view`` to memory regions represented via ``dst1_view`` and
``dst2_view``.

The operation is presented as ``dml::dualcast`` object.

Usage:

.. code-block:: cpp

   auto result = dml::execute<dml::software>(dml::dualcast, src_view, dst1_view, dst2_view);

Result for this operation is:

.. code-block:: cpp

   struct dualcast_result
   {
       status_code status{status_code::error}; /**< @todo */
   };

.. note::

   1. There are no alignment requirements for ``src_view`` data.

   2. For ``dst1_view`` and ``dst2_view``, the data address’ 11:0 bits must be the same.

   3. The data regions passed to the operation shall not overlap.


Fill
----


This operation fills data at memory region represented via ``dst_view``
with 64-bit ``pattern``.

The operation is presented as ``dml::fill`` object.

Usage:

.. code-block:: cpp

   auto result = dml::execute<dml::software>(dml::fill, pattern, dst_view);

Result for this operation is:

.. code-block:: cpp

   struct fill_result
   {
       status_code status{status_code::error}; /**< @todo */
   };

.. note::

   1. The ``pattern`` size is always 8 bytes.

   2. The ``dst_view`` size does not need to be a multiple of 8.


Memory Compare and Restoring Features
=====================================


This group of operation is used for memory comparing and restoring to
proper state.

Compare
-------


This operation compares data at memory region represented via
``src1_view`` with data at memory region represented via ``src2_view``.

The operation is presented as ``dml::compare`` object.

.. doxygenclass:: dml::compare_operation
   :project: Intel DML
   :members:

Usage:

.. code-block:: cpp

   auto result = dml::execute<dml::software>(dml::compare, src1_view, src2_view);

Result for this operation is:

.. code-block:: cpp

   struct compare_result
   {
       status_code status{status_code::error}; /**< @todo */
       result_t    result{};                   /**< @todo */
       size_t      bytes_completed{};          /**< @todo */
   };


Compare Pattern
---------------


This operation compares data at memory region represented via
``src_view`` with 64-bit ``pattern``.

The operation is presented as ``dml::compare`` object.

The operation ``execute`` method has the following arguments:

- ``pattern`` - 64-bit pattern for data comparison
- ``data`` - source data represented via ``dml::data_view`` object

Usage:

.. code-block:: cpp

   auto result = dml::execute<dml::software>(dml::compare_pattern, pattern, src_view);

Result for this operation is:

.. code-block:: cpp

   struct compare_result
   {
       status_code status{status_code::error}; /**< @todo */
       result_t    result{};                   /**< @todo */
       size_t      bytes_completed{};          /**< @todo */
   };

.. note::

   The ``src_view`` parameter size does not need to be a multiple of 8.


Create Delta Record
-------------------


This operation compares data at memory region represented via
``src1_view`` with data at memory region represented via ``src2_view``.
Then it writes delta record to memory region represented via
``delta_view``. The delta record contains the information needed to
update the data at the ``src1_view`` memory region to match the data at
the ``src2_view`` memory region.

The operation is presented as ``dml::create_delta`` object.

The operation ``execute`` method has the following arguments:

- ``base`` - base source data represented via ``dml::data_view`` object
- ``data`` - source data represented via ``dml::data_view`` object
- ``delta`` - destination data represented via ``dml::data_view`` object for delta record

Usage:

.. code-block:: cpp

   auto result = dml::execute<dml::software>(dml::create_delta, src1_view, src2_view, delta_view);

Result for this operation is:

.. code-block:: cpp

   struct create_delta_result
   {
       status_code status{status_code::error}; /**< @todo */
       result_t    result{};                   /**< @todo */
       size_t      bytes_completed{};          /**< @todo */
       size_t      delta_record_size{};        /**< @todo */
   };

.. note::

   1. The ``src1_view`` parameter is limited by the maximum offset that can
   be stored in the delta record.

   1. ``src1_view``, ``src2_view`` and ``delta_view`` addresses must be
   8-byte aligned (multiple of 8).

   1. ``delta_view`` size must be a multiple of 10, and not less that 80
   bytes.

   1. Since the ``offset`` field in the delta record is ``uint16_t``
   representing a multiple of 8 bytes, the maximum offset that can be
   expressed is ``0x7fff8``, so the maximum transfer size (the
   ``src1_view`` parameter) is ``0x80000`` bytes (512KB).


Apply Delta Record
------------------


This operation applies delta record written to memory region represented
via ``delta_view`` view onto data at memory region represented via
``dst_view`` view.

This operation is used in pair with Create Delta operation to update
``src1_view`` so that it matches ``src2_view``, using ``delta_view``
written.

The operation is presented as ``dml::apply_delta`` object.

Usage:

.. code-block:: cpp

   auto create_result = dml::execute<dml::software>(dml::create_delta, src1_view, src2_view, delta_view);
   auto result = dml::execute<dml::software>(dml::apply_delta, delta_view, src1_view, create_result);

Result for this operation is:

.. code-block:: cpp

   struct apply_delta_result
   {
       status_code status{status_code::error}; /**< @todo */
   };

.. note::

   ``delta_view`` parameter shall be got from a successful call of
   create_delta, it is automatically checked through ``create_result``
   parameter.


Memory Hash Features
====================


This group of operation is used for ensuring data integrity with hash
algorithms.


CRC Generation
--------------


This operation computes CRC on the data at memory region represented via
``src_view``. Initial crc seed is provided via ``crc_seed`` argument.

The operation is presented as ``dml::crc`` object. The operation is
configurable via ``.bypass_reflection()`` and ``.bypass_data_reflection()`` methods.

Usage:

.. code-block:: cpp

   auto result = dml::execute<dml::software>(dml::crc, src_view, crc_seed);

Result for this operation is:

.. code-block:: cpp

   struct crc_result
   {
       status_code status{status_code::error}; /**< @todo */
       uint32_t    crc_value{};                /**< @todo */
   };

.. note::

   This operation computes CRC using the polynomial 0x11edc6f41 (iSCSI
   Protocol, RFC 3720). When the size of ``src_view`` parameter is not a
   multiple of 4 bytes, the source data is padded to the end with zeros.


Copy with CRC Generation
------------------------


This operation copies data at memory region represented via ``src_view``
to memory region represented via ``dst_view``, and then computes CRC on
the data copied. Initial crc seed is provided via ``crc_seed`` argument.

The operation is presented as ``dml::crc`` object. The operation is
configurable via ``.bypass_reflection()`` and ``.bypass_data_reflection()`` methods.

Usage:

.. code-block:: cpp

   auto result = dml::execute<dml::software>(dml::copy_crc, src_view, dst_view, crc_seed);

Result for this operation is:

.. code-block:: cpp

   struct crc_result
   {
       status_code status{status_code::error}; /**< @todo */
       uint32_t    crc_value{};                /**< @todo */
   };

.. note::

   Memory regions represented via ``src_view`` and ``dst_view`` should
   not overlap.


Utility Features
================


This group of operation is used for utility purposes.


Cache Flush
-----------


The Cache Flush operation flushes the processor caches at memory region
represented via ``dst_view``.

The operation is presented as ``dml::cache_flush`` object. The operation
is configurable via ``.dont_invalidate_cache()`` method.

Usage:

.. code-block:: cpp

   auto result = dml::execute<dml::software>(dml::cache_flush, dst_view);

Result for this operation is:

.. code-block:: cpp

   struct cache_flush_result
   {
       status_code status{status_code::error}; /**< @todo */
   };

.. note::

   By default this operation invalidates affected cache lines from every
   level of cache hierarchy. To disable that, use
   ``dml::cache_flush.dont_invalidate_cache()`` as the first argument.


Operation Status Values
***********************


.. doxygenenum:: status_code
  :project: Intel DML
  :no-link:
