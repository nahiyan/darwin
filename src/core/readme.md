# Darwin Core

This directory holds code defining Darwin's core. It's meant to be compiled into a static library, to be linked with extensions.

# Building

Use CMake to build the Makefile:

```bash
cmake .
```

Use Make to compile and link the static library:

```bash
make
```

A static library will be generated afterwards. You can configure your linker to use this static library; don't forget to include the \*.h files in the "include" directory during compilation.
