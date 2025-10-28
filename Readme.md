# Reference implementation for proposed standard C result
(C) 2024 - 2025 Niall Douglas [http://www.nedproductions.biz/](http://www.nedproductions.biz/)

CI: [![CI](https://github.com/ned14/wg14_result/actions/workflows/ci.yml/badge.svg)](https://github.com/ned14/wg14_result/actions/workflows/ci.yml)

Can be configured to be a standard library implementation for your
standard C library runtime. Licensed permissively.

This is a 100% C reimplementation of https://ned14.github.io/outcome/experimental/,
created as Outcome.Experimental is now on the WG14 C standardisation track
and I no longer participate on WG21 (C++ standards). Features:

- 100% ABI compatible with the C++ implementation, you can freely switch
between each, even by casting pointers on objects.
- Unlike the C API which C++ Outcome has shipped for years, this one does
not require any C++ runtime to be linked into any final binaries.
- Can be configured as a header-only library (unity build) which has headers
also include their source files. If so:
    - The usual C 90 minimum requirement for the headers is raised to C 11,
as the library source code needs minimum C 11.
    - If a C++ compiler is being used, the minimum requirement for the headers
is raised to C++ 20 as previous C++ standards don't implement enough of C 11.

## Example of use

```c
todo
```

## Supported targets

This library should work well on any POSIX implementation, as well as
Microsoft Windows. You will need a minimum of C 11 in your toolchain to compile
the library, however its header files work in all standards of C right back to C 90.

Current CI test targets:

- Ubuntu Linux, x64.
- Mac OS, AArch64.
- Microsoft Windows, x64.

Current compilers:

- GCC
- clang
- MSVC

## Configuration

You can find a number of user definable macros to override in `config.h`.
They have sensible defaults on the major platforms and toolchains.


## Performance

### On my Threadripper 5975WX which is a 3.6Ghz processor bursting to 4.5Ghz on Linux:


### On a MacBook Pro M3 running ARM64


### On a MacBook Pro M3 running ARM64 Windows within a VM


# Todo

- Adjust C++ string_ref thunk function to return a `bool` (true for success).
- `atomic_refcounted_string_ref` should store its atomic at the front of the malloc buffer.
