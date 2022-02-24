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
