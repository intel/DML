Intel® Data Mover Library (Intel® DML) Beta
=================================================
[Get Started](#get-started) | [Documentation](#documentation) | [License](#license) | [GitHub repository](https://github.com/intel/DML)

Intel® Data Mover Library (Intel® DML) is an open-source library to provide high-performance data manipulation on Intel CPUs. Intel® DML is intended for streaming data movement and transformation operations optimization that is applicable for effective storage, networking, persistent memory, and various data processing applications. 

## Get Started

To get started with Intel DML, follow instructions below:

1. Make sure your system meets all [System Requirements](https://intel.github.io/DML/documentation/introduction_docs/system_requirements.html).

2. Follow instructions to build the library. See [Installation](https://intel.github.io/DML/documentation/get_started_docs/installation.html) for more details.

## Documentation 

### Online Documentation

Documentation is delivered using GitHub Pages. See full Intel DML [online documentation](https://intel.github.io/DML/index.html).

### Offline Documentation

**Note**: Before building Intel DML offline documentation, make sure you have all documentation generation tools properly installed and set up. See, [Documentation Build Prerequisites](https://intel.github.io/DML/documentation/get_started_docs/installation.html#documentation-build-prerequisites) for more details.

To generate full offline documentation from sources, use the following commands:

```shell
cmd> cd <dml_library catalog path>/doc
cmd> _get_docs.sh
```

After generation process is completed, open the `<dml_library>/doc/build/html/index.html` file.

## How to Contribute

See [Contributing document](./CONTRIBUTING.md) for details about contribution process.

## License

The library is licensed under the MIT license. Refer to the
"[LICENSE](./LICENSE)" file for the full license text.

This distribution includes third party software governed by separate license
terms (see "[THIRD-PARTY-PROGRAMS](./third-party-programs.txt)").
