# Reference implementation for proposed standard C result
(C) 2024 - 2025 Niall Douglas [http://www.nedproductions.biz/](http://www.nedproductions.biz/)

CI: [![CI](https://github.com/ned14/wg14_result/actions/workflows/ci.yml/badge.svg)](https://github.com/ned14/wg14_result/actions/workflows/ci.yml)

Can be configured to be a standard library implementation for your
standard C library runtime. Licensed permissively.

This is a 100% C reimplementation of https://ned14.github.io/outcome/experimental/,
created as Outcome.Experimental is now on the C standardisation track
and I no longer participate on WG21 (C++ standards). Features:

- 100% ABI compatible with the C++ implementation, you can freely switch
between each.
- Unlike the C API which C++ Outcome has shipped for years, this one does
not require the C++ runtime to be linked into any final binaries.

## Example of use

```c
todo
```

## Supported targets

This library should work well on any POSIX implementation, as well as
Microsoft Windows. You will need a minimum of C 11 in your toolchain.

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
