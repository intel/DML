Intel® Data Mover Library (Intel® DML) Beta
=================================================
[Get Started](#get-started) | [Documentation](#documentation) | [License](#license) | [GitHub repository](https://github.com/intel-innersource/libraries.performance.accelerators.dml.dml-library.git)

Intel® Data Mover Library (Intel® DML) is an open-source library to provide high-performance data manipulation on Intel CPUs. Intel® DML is intended for streaming data movement and transformation operations optimization that is applicable for effective storage, networking, persistent memory, and various data processing applications. 

## Get Started

### Prerequisites
- Minimal CPU vector instruction set supported for not optimized code is Intel® Streaming SIMD Extensions 2 (Pentium® 4 processor or higher)
- 64-bit x86 CPU
- Compiler:
    - Linux* OS: gcc 8.2 or higher
    - Windows* OS: MSVC 19 or higher
- Cross-platform build tool: CMake* version 3.12 or higher
- Make: GNU 'make' (Linux* OS) or 'nmake' (Windows*)
- Documentation generator: Doxygen 1.8.17 or higher
- Universally Unique ID library: `uuid-dev` version 2.35.2 or higher.
### Build

To build the library, complete the following steps:

1.  Make sure that all the tools from the Prerequisites section are available from your environment.

2.  Extract git sources using the following command:

```shell
git clone --recursive https://github.com/intel/DML.git
```

3.  Build the library and tests by executing the following list of commands from \<dml_library\>:

#### Windows* OS 

```shell
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=<install_dir> -G "NMake Makefiles" ..
cmake --build . --target install
```

#### Linux* OS 

```shell
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=<install_dir> ..
cmake --build . --target install
```

Note: If you need to build a particular type of the library and tests (`DEBUG`|`RELEASE`), use the `CMAKE_BUILD_TYPE` flag as follows:

```shell
# Debug
cmake -DCMAKE_BUILD_TYPE=Debug <path_to_cmake_folder>

# Release
cmake -DCMAKE_BUILD_TYPE=Release <path_to_cmake_folder>

# Release with debug information
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo <path_to_cmake_folder>
```

Note: 
- CMake generates the script for release build as default.

- On Linux* OS, the default `<install_dir>` is `/usr/local/`.

- The library builds by default with software path only. If you need to build the library with enabled hardware, use the DML_HW option as follows:

```shell
# Enable hardware path
cmake -DCMAKE_BUILD_TYPE=Release -DDML_HW=ON <path_to_cmake_folder>
```

```shell
# Enable libaccel-config-3.2 support for hardware path
cmake -DCMAKE_BUILD_TYPE=Release -DDML_HW=ON -DLIB_ACCEL_3_2=ON <path_to_cmake_folder> 
```

- To enable `-frecord-gcc-switches` flag, use the DML_RECORD_SWITCHES option as follows:

```shell
# Enable -frecord-gcc-switches
cmake -DCMAKE_BUILD_TYPE=Release -DDML_RECORD_SWITCHES=ON <path_to_cmake_folder>
```

The resulting library is available in the `<install_dir>/lib` folder.

## Documentation

- [Intel DML Reference Manual](./doc/DML_REFERENCE_MANUAL.md)
- [Security Policy](doc/SECURITY.md) 

To generate full documentation from sources with Doxygen, use the following commands:

```shell
cmd> cd <dml_library catalog path>
cmd> doxygen ./Doxyfile
```

To open the generated HTML Reference, open the `<dml_library>/doc/html/index.html` file.

***Note:*** You can find the Doxygen processor at [Doxygen official site](http://www.doxygen.nl/).

## How to Contribute

See [Contributing document](CONTRIBUTING.md) for details about contribution process.

## License

The library is licensed under the MIT license. Refer to the
"[LICENSE](LICENSE)" file for the full license text.

This distribution includes third party software governed by separate license
terms (see "[THIRD-PARTY-PROGRAMS](third-party-programs.txt)").
