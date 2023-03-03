 .. ***************************************************************************
 .. * Copyright (C) 2023 Intel Corporation
 .. *
 .. * SPDX-License-Identifier: MIT
 .. ***************************************************************************/

.. _library_benchmarking_reference_link:


Benchmarks Framework Guide
##########################

Goal of the benchmarks is to provide performance guidance on various use case scenarios
of the Intel® Data Mover Library (Intel® DML).
To cover these cases the benchmarks provide several performance metrics and modes of operation.

.. attention::

    Currently, the Intel DML benchmarks framework offers limited support.

    - Only mem_move cases are currently supported.

Quick Start
***********

Benchmark are based on the Google benchmark library and built as a part of Intel DML.
Refer to :ref:`Installation page <building_library_reference_link>`
for details on how to build the library and resolve all prerequisites.

Please refer to :ref:`benchmark system requirements <system_requirements_benchmark_reference_link>`
for more information on the requirements needed for building and running Benchmark.

The example below demonstrates running a benchmark on a memory-move operation via 
the Low-Level C API using the accelerator, a queue size of 16, and 1 MB of data.


.. warning::

    Make sure to resolve :ref:`requirements for running on hardware path <system_requirements_hw_path_reference_link>` and
    configure :ref:`Intel® Data Streaming Accelerator (Intel® DSA) <accelerator_configuration_reference_link>`
    before executing the example.

.. code-block:: shell

    ./<install_dir>/bin/dml_benchmarks --benchmark_min_time=0.3 --benchmark_filter=move/api:c/path:.*/exec:.*/qsize:16/bsize:0/in_mem:llc/out_mem:cc_llc/timer:full/size:1048576/.*

.. code-block:: shell
    :caption: Output to terminal:

    ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    Benchmark                                                                                                                       Time             CPU   Iterations UserCounters...
    ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    move/api:c/path:dsa/exec:sync/qsize:16/bsize:0/in_mem:llc/out_mem:cc_llc/timer:full/size:1048576/real_time                 833022 ns       831025 ns          503 Latency=833.022us Latency/Op=52.0639us Throughput=20.1402G/s
    move/api:c/path:dsa/exec:async/qsize:16/bsize:0/in_mem:llc/out_mem:cc_llc/timer:full/size:1048576/real_time/threads:1      763158 ns       760850 ns          549 Latency=763.158us Latency/Op=47.6974us Throughput=21.9839G/s
    move/api:c/path:dsa/exec:async/qsize:16/bsize:0/in_mem:llc/out_mem:cc_llc/timer:full/size:1048576/real_time/threads:2      361855 ns       722324 ns         1134 Latency=361.855us Latency/Op=45.2319us Throughput=23.1822G/s
    move/api:c/path:dsa/exec:async/qsize:16/bsize:0/in_mem:llc/out_mem:cc_llc/timer:full/size:1048576/real_time/threads:4      182710 ns       729806 ns         2132 Latency=182.71us Latency/Op=45.6775us Throughput=22.9561G/s
    move/api:c/path:dsa/exec:async/qsize:16/bsize:0/in_mem:llc/out_mem:cc_llc/timer:full/size:1048576/real_time/threads:8      144228 ns      1151468 ns         2896 Latency=144.228us Latency/Op=72.114us Throughput=14.5405G/s
    move/api:c/path:dsa/exec:async/qsize:16/bsize:0/in_mem:llc/out_mem:cc_llc/timer:full/size:1048576/real_time/threads:16      81297 ns      1297643 ns         5120 Latency=81.2965us Latency/Op=81.2965us Throughput=12.8982G/s


Key Terms
*********

* **API**: Currently, only Low-Level C API, case naming: ``api:c``, High-Level C++ API, case naming ``api:cpp``, and standard glibc mem_move and mem_copy implementations, case naming ``api::glibc`` are supported.
* **Path**: Represents execution path for the library. Case naming: ``path:dsa`` for executing on the accelerator, ``path:cpu`` for running on the CPU.

.. warning::

    Executing on ``path:dsa`` requires the accelerator to be already configured.
    Refer to :ref:`accelerator configuration page <accelerator_configuration_reference_link>`.

* **Execution mode**: Defines synchronous or asynchronous execution. Case naming: ``exec:async``, ``exec:sync``.

    **Sync mode**: Each measurement loop, one call to Intel DML operation is submitted always followed by blocking wait.
    Thus only one operation is processed at a time and only one engine of the accelerator is loaded.
    Is not affected by ``--threads`` argument. Main output of sync mode is **Latency** metric.
    ``--queue_size`` can be used to submit several operations at once to measure latency of the various queue sizes.

    **Async mode**: Each case spawns ``--threads`` number of threads and each thread runs it's own measurement loop.
    For each loop, benchmark will submit ``--queue_size``/``--threads`` operations without blocking wait
    and resubmit operation as soon as it is completed always keeping device busy.
    Main output of async mode is **Throughput** metric.
    For small workloads, higher number of threads may be required to saturate devices,
    for big workloads, even one thread may reach capacity.

* **Blocks**: Input data is split by blocks of size ``--block_size=XXXX`` (with XXXX being in bytes) and each block is processed separately.

Using the Benchmarks Framework
******************************

Intel DML benchmark framework is based on Google benchmark library,
run ``./dml_benchmarks --help`` to get a full list of supported commands and input arguments with detailed description.

.. attention::

    If no accelerators are available on the system, you can use ``--no_hw`` to suppress Intel DSA initialization check warning.

In order to set up a specific run configuration ``--benchmark_filter`` should be used,
which input is a regex based on the case name.

For example, use the following expression to launch a copy operation using the Low-Level C API 
with synchronous execution on a CPU: ``--benchmark_filter="copy/api:c/path:cpu/exec:sync/.*"``.

Executing on Hardware Path
==========================

.. warning::

    Make sure to resolve :ref:`requirements for running on hardware path <system_requirements_hw_path_reference_link>` and
    :ref:`configure accelerator <accelerator_configuration_reference_link>` before executing the example.

.. attention::

    It is the user's responsibility to configure the accelerator.
    Currently, the library doesn't provide a way to specify a number of Intel DSA
    instances for execution and will use everything available on the system.
    If you need to run one or more Intel DSA instances, make sure your system is configured appropriately.

Latency Tests
=============

For reporting or tracking latency metric, use ``sync`` mode, a single Intel DSA instance, and a single thread.

Below are examples for mem_copy (``copy``) and mem_move (``move``) using 4kb block_sizes:

.. code-block:: shell

    sudo ./<install_dir>/bin/dml_benchmarks --benchmark_filter="copy/.*/path:dsa/exec:sync/.*/size:4096/.*" --benchmark_min_time=0.1


.. code-block:: shell

    sudo ./<install_dir>/bin/dml_benchmarks --benchmark_filter="move/.*/path:dsa/exec:sync/.*/size:4096/.*" --benchmark_min_time=0.1

Throughput Tests
================

For reporting or tracking throughput metric, use ``async`` mode, 1 to 4 Intel DSA devices, and multiple threads.

Below are examples for mem_copy (``copy``) and mem_move (``move``) using 4kb block_size and ``queue_size=128``

on the Low-Level C API:

.. code-block:: shell

    sudo ./<install_dir>/bin/dml_benchmarks --benchmark_filter="copy/api:c/path:dsa/exec:async/qsize:128/.*/size:4096/.*/threads:8" --benchmark_min_time=0.5


.. code-block:: shell

    sudo ./<install_dir>/bin/dml_benchmarks --benchmark_filter="move/api:c/path:dsa/exec:async/qsize:128/.*/size:4096/.*/threads:8" --benchmark_min_time=0.5

