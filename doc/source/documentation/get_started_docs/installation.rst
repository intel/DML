 .. ***************************************************************************
 .. * Copyright (C) 2021 Intel Corporation
 .. *
 .. * SPDX-License-Identifier: MIT
 .. ***************************************************************************/

Installation
############


Repository Structure
********************


::

     ├── cmake
     │   └── Modules         # Cmake modules and configuration
     ├── doc                 # Contains library documentation
     ├── examples
     │   ├── high_level_api  # C++ API usage examples
     │   └── dml_job_api     # C API usage examples
     ├── google-benchmark    # link to google-bechmarks
     ├── google-test         # link to google-tests
     ├── high-level-api      # C++ API headers
     ├── include             # C API headers
     ├── isal                # Compression backend
     ├── sources             # Library Sources
     │   ├── core            # High optimized kernels
     │   ├── hw-path         # HW interconnect
     │   ├── include         # Private includes
     │   └── sw-path         # SW path of the library
     └── tests
         ├── common          # Common service functions for testing
         ├── benchmarks      # Benchmarks
         ├── testdata        # Corpuses for Compression testing
         ├── high-level-api  # C++ API usage examples
         └── dml_job_tests   # C API usage examples


Library Presetting and Building
*******************************


Library requires presetting of hardware configuration before usage.
Library doesn’t perform hardware setup independently. System
administrator is responsible for correctness of accelerator
configuration. If configuration is invalid or doesn’t exist, library
returns an appropriate status code in case if hardware execution path
used.

.. note::

   Intel® DML doesn’t support all hardware possibilities. Library
   limitations are described in the `Library
   Limitations <../introduction_docs/introduction.html#library-limitations>`__ section.


System Requirements
===================


For more information about system requirements,
see `System Requirements <../introduction_docs/system_requirements.html>`__.

.. _accelerator_configuration_reference_link:

Accelerator Configuration
=========================

An Intel® Data Streaming Accelerator (Intel® DSA) device can be
configured with the ``libaccel-config`` library, which can be found at
https://github.com/intel/idxd-config.

For instance, users can use the following commands
to directly configure the device with the accel-config using existing config file:

.. code-block:: shell

    accel-config load-config -c <config file>
    accel-config enable-device <device>
    accel-config enable-wq <device>/<wq>

Intel® DML also provides a way to configure Intel® DSA through calling
either of the following commands based on whether you are setting up
via the Intel® DML source or from the Intel® DML installed directory:

.. code-block:: shell

   sudo python3 <dml-library>/tools/scripts/accel_conf.py --load=<path to config file>

.. code-block:: shell

   sudo python3 <install-dir>/bin/scripts/accel_conf.py --load=<path to config file>

With configuration files found at either ``<dml-library>/tools/configs/`` or ``<install-dir>/bin/configs/``.
With configuration files of the format ``<# nodes>n<# devices>d<# engines>e<# of workqueues>w-s.conf`` or
``<# nodes>n<# devices>d<# engines>e<# of workqueues>w-s-n<which node>.conf``.

.. attention::

   Sudo privileges are required to configure Intel® DSA.


Build Prerequisites
===================


Before building the library, install and set up the following tools:

-  Compiler:

   -  Linux* OS: gcc 8.2 or higher
   -  Windows* OS: MSVC 19 or higher

- Libraries:

   -  Linux* OS: Universally Unique ID library: ``uuid-dev`` version 2.35.2 or higher

-  Cross-platform build tool: CMake* version 3.12 or higher
-  Make: GNU ‘make’ (Linux* OS) or ‘nmake’ (Windows* OS)


Documentation Build Prerequisites
=================================


If you plan to build an offline version of documentation,
make sure the following documentation generation tools are installed and set up:

-  `Doxygen <https://www.doxygen.nl/index.html>`__ 1.8.17 or higher: ``apt install doxygen``
-  `Python <https://www.python.org/>`__ 3.8.5 or higher: ``apt install python3.X``
-  `Sphinx <https://www.sphinx-doc.org/en/master/>`__ 3.5.4 or higher: ``pip3 install sphinx``
-  `sphinx_book_theme <https://executablebooks.org/en/latest/>`__ : ``pip3 install sphinx-book-theme``
-  `Breathe <https://breathe.readthedocs.io/en/latest/>`__ 4.29.0 or higher: ``pip3 install breathe``


.. note::

   Linux* OS shell (or Windows* OS shell alternatives) is required to run the build script.

To generate full offline documentation from sources, use the following commands:

.. code-block:: shell

   cmd> cd <dml_library catalog path>/doc
   cmd> _get_docs.sh


After generation process is completed, open the `<dml_library>/doc/build/html/index.html` file.

.. _building_library_build_options_reference_link:

Available Build Options
=======================

Intel DML supports the following build options:

-  ``-DSANITIZE_MEMORY=[ON|OFF]`` - Enables memory sanitizing (``OFF`` by default).
-  ``-DSANITIZE_THREADS=[ON|OFF]`` - Enables threads sanitizing (``OFF`` by default).
-  ``-DLOG_HW_INIT=[ON|OFF]`` - Enables hardware initialization log (``OFF`` by default).
-  ``-DDML_BUILD_EXAMPLES=[OFF|ON]`` - Enables building library examples (``ON`` by default).
   For more information on existing examples, see :ref:`code_examples_reference_link`.

-  ``-DDML_BUILD_TESTS=[OFF|ON]`` - Enables building library testing and benchmarks frameworks (``ON`` by default).
   For more information on library testing, see :ref:`library_testing_reference_link` section.
   For information on benchmarking the library, see :ref:`library_benchmarking_reference_link`.

.. attention::

   To build Intel DML from the GitHub release package (``.tar``, ``.tgz``)
   without downloading sub-module dependencies for testing and benchmarking,
   use ``-DDML_BUILD_TESTS=OFF``.

.. _building_library_reference_link:

Build
=====

To build the library, complete the following steps:

1. Make sure that all the tools from the `Build Prerequisites <#build-prerequisites>`__
   section are available from your environment.

2. Extract git sources using the following command:


   .. code-block:: shell

      git clone --recursive https://github.com/intel/DML.git

3. Build the library and tests by executing the following list of
   commands from ``<dml_library>`` depending on OS:


**Windows\* OS:**

.. code-block:: shell

   mkdir build
   cd build
   cmake -DCMAKE_INSTALL_PREFIX=<install_dir> -G "NMake Makefiles" ..
   cmake --build . --target install


**Linux\* OS:**

.. code-block:: shell

   mkdir build
   cd build
   cmake -DCMAKE_INSTALL_PREFIX=<install_dir> ..
   cmake --build . --target install

If you need to build a particular type of the library and tests
(``DEBUG`` | ``RELEASE``), use the ``CMAKE_BUILD_TYPE`` flag as follows:

.. code-block:: shell

   # Debug
   cmake -DCMAKE_BUILD_TYPE=Debug <path_to_cmake_folder>

   # Release
   cmake -DCMAKE_BUILD_TYPE=Release <path_to_cmake_folder>

   # Release with debug information
   cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo <path_to_cmake_folder>

CMake generates the script for release build as default.
On Linux* OS, the default ``<install_dir>`` is ``/usr/local/``.

To enable ``-frecord-gcc-switches`` flag, use the ``DML_RECORD_SWITCHES`` option as follows:

.. code-block:: shell

   # Enable -frecord-gcc-switches
   cmake -DCMAKE_BUILD_TYPE=Release -DDML_RECORD_SWITCHES=ON <path_to_cmake_folder>

The resulting library is available in the ``<install_dir>/lib`` folder.
