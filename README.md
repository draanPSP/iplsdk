IPL SDK
===
This is a minimalistic SDK for developing IPL code for the PSP, written in C++.

Building
===
To build the provided examples, you need the latest version of the [psp toolchain](https://github.com/pspdev/psptoolchain). Then use CMake to build:
```bash
psp-cmake -S . -B build && cmake --build build
```