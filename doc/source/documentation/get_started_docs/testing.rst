 .. ***************************************************************************
 .. * Copyright (C) 2021 Intel Corporation
 .. *
 .. * SPDX-License-Identifier: MIT
 .. ***************************************************************************/

Library Testing
###############

The Intel® Data Mover Library (Intel® DML) is distributed with
its own test system based on the GoogleTest framework.
You can find various types of tests in ``<install_dir>/bin/`` folder.

.. attention::

    Intel DML test framework development is still in progress.
    Expect to see more coverage and refactoring in future releases.

Functional Tests
================

Functional validation of Intel DML could be done with different types of tests provided.

Low-Level API (or Job API) and core unit tests (comparing internal optimized code path with some
reference code path) include:

- Algorithmic tests (``ta_*``) serve to find errors that reflect logic
  errors in data processing on correct data.
- Bad argument tests (``tb_*``) verify the code path for invalid arguments.

Such tests are combined together into ``tests`` and ``tests_mt`` (multithreaded version) executables.
In order to use those, following commands could be used:

.. code-block:: shell

    ./tests --path=sw

.. code-block:: shell

    ./tests_mt --path=sw --nthreads=2

Full list of parameters is available via ``./<executable_name> --help``.

Additional High-Level API and Low-Level API tests are compiled into
``dml_test_functional_<api>_<path>_<operation>``, where ``api=c`` refers to
Low-Level Job API and ``api=cpp`` refers to High-Level API tests,
``<path>`` could be one of ``sw`` (DML_PATH_SW for Low-Level API or dml::software for High-Level API, used for host execution),
``hw`` (DML_PATH_HW or hardware, for accelerator offloading), or auto (DML_PATH_AUTO or dml::automatic).

In order to execute those, use the following command:

.. code-block:: shell

    ./dml_test_functional_<api>_<path>_<operation>

.. attention::

   Hardware Path requires first configuring Intel® Data Streaming Accelerator (Intel® DSA).
   See :ref:`Accelerator Configuration <accelerator_configuration_reference_link>`.


