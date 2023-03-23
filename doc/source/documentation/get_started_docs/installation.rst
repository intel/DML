 .. ***************************************************************************
 .. * Copyright (C) 2021 Intel Corporation
 .. *
 .. * SPDX-License-Identifier: MIT
 .. ***************************************************************************/

Installation
############

.. _system_requirements_reference_link:

System Requirements
*******************

Intel® Data Mover Library (DML) supports only the Intel 64 platform.

Software Path Requirements
==========================

| **Minimum requirements**
| x86-64 CPU with Intel® Advanced Vector Extensions 2 support
  (Intel® microarchitecture code name Broadwell).

| **Recommended requirements**
| x86-64 CPU with Intel® Advanced Vector Extensions 512 support
  (Intel® microarchitecture code name Skylake (Server) processor or higher).

.. _system_requirements_hw_path_reference_link:

Hardware Path Requirements
==========================

Execution using the Hardware Path is available only on Linux* OS.

Additionally, the operating system must meet the following requirements:

- Linux kernel version version 5.10 or higher (https://www.kernel.org/doc/Documentation/ABI/stable/sysfs-driver-dma-idxd).
- Virtualization technology for directed I/O (VT-d) is enabled through the BIOS menu.

.. _accelerator_configuration_reference_link:

Accelerator Configuration
*************************


Library requires presetting of hardware configuration before use.
Library doesn't perform hardware setup independently. System
administrator is responsible for correctness of accelerator
configuration. If configuration is invalid or doesn't exist, the library
returns an appropriate status code if the hardware execution is path
used.

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


.. _building_library_reference_link:

Building the Library
********************

.. _building_library_prerequisites_reference_link:

Prerequisites
=============

Before building the library, install and set up the following tools:

-  C++ compiler with C++17 standard support

   -  Linux* OS: gcc 8.2 or higher
   -  Windows* OS: MSVC 19 or higher
-  Linux* OS: Universally Unique ID library (``uuid-dev``) version 2.35.2 or higher
-  Cross-platform build tool: CMake* version 3.12 or higher
-  Make

   - Linux* OS: GNU Make
   - Windows* OS: Microsoft NMAKE`

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

- ``DML_RECORD_SWITCHES=[OFF|ON]`` - Enables ``-frecord-gcc-switches`` flag (``OFF`` by default).

Build
=====

To build the library, complete the following steps:

1. Make sure that all the tools from the :ref:`building_library_prerequisites_reference_link`
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


Installed Package Structure
===========================

::

     ┌── bin
     │   ├── configs                # Configuration files for Intel® DSA
     │   ├── scripts                # Scripts useful for configuration step
     │   ├── dml_benchmarks         # Benchmarks framework
     │   ├── tests*                 # Testing framework
     │   └── dml_test_functional*   # Additional test executables for C and C++ APIs
     │
     ├── include
     │   └── dml
     |       ├── dml.h              # Low-Level C API headers
     |       ├── dml.hpp            # High-Level C++ API headers
     |       ├── dmldefs.h
     |       ├── detail
     |       └── hl
     │
     └── lib or lib64
         ├── cmake
         ├── libdml.a               # Library with High-Level C++ APIs and implementations
         └── libdmlhl.a             # Library with Low-Level C APIs and implementations

Executables for tests and benchmarks framework, as well as
configuration files for Intel® DSA
(see :ref:`accelerator_configuration_reference_link` for more details)
are available in ``bin/`` folder.

Examples are located in ``<dml_library>/build/examples/``.


Building the Documentation
**************************

Prerequisites
=============

If you plan to build an offline version of documentation,
make sure the following documentation generation tools are installed and set up:

-  `Doxygen <https://www.doxygen.nl/index.html>`__ 1.8.17 or higher: ``apt install doxygen``
-  `Python <https://www.python.org/>`__ 3.8.5 or higher: ``apt install python3.X``
-  `Sphinx <https://www.sphinx-doc.org/en/master/>`__ 3.5.4 or higher: ``pip3 install sphinx``
-  `sphinx_book_theme <https://executablebooks.org/en/latest/>`__ : ``pip3 install sphinx-book-theme``
-  `Breathe <https://breathe.readthedocs.io/en/latest/>`__ 4.29.0 or higher: ``pip3 install breathe``

Build Steps
===========

To generate full offline documentation from sources, use the following commands:

.. code-block:: shell

   cmd> cd <dml_library catalog path>/doc
   cmd> _get_docs.sh

.. attention::

   Linux* OS shell (or Windows* OS shell alternatives) is required to run the build script.

After the generation process is completed, open the ``<dml_library>/doc/build/html/index.html`` file.
