### Testing

You can find the tests in `<dml_library>/cmake/.build/tests` or in `<install_dir>/bin/tests` if DCMAKE_INSTALL_PREFIX set.
To test the library, execute the following command:

```shell
./tests
```

If you want to launch tests with custom parameters,
then use special flags. To see all of them, execute the following command:

```shell
./tests --help
```

Additional tests can be run via CTest tool.

```shell
cd <build_dir>
ctest
```
