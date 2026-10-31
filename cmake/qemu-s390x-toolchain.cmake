# A big endian cross build that can still be run, under qemu user emulation.
#
# The Result's status word is two uint16_t halves inside a 32 bit word, and which
# half sits where depends on the endianness, so a big endian machine lays it out
# the other way round. That path is easy to get wrong and impossible to notice on
# an x86 CI runner, which is why there is a lane for it.
#
#   cmake -B build -DCMAKE_TOOLCHAIN_FILE=../cmake/qemu-s390x-toolchain.cmake \
#         -DCMAKE_BUILD_TYPE=Release
#   cmake --build build && ctest --test-dir build
#
# Needs gcc-s390x-linux-gnu / g++-s390x-linux-gnu and qemu-user-static. Static
# linking is used so that qemu needs no target sysroot to find shared objects.
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR s390x)
# `if NOT DEFINED` so a caller can point at versioned compiler names, which is
# how several distributions package the cross toolchain.
if(NOT DEFINED CMAKE_C_COMPILER)
  set(CMAKE_C_COMPILER s390x-linux-gnu-gcc)
endif()
if(NOT DEFINED CMAKE_CXX_COMPILER)
  set(CMAKE_CXX_COMPILER s390x-linux-gnu-g++)
endif()
set(CMAKE_FIND_ROOT_PATH /usr/s390x-linux-gnu)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_EXE_LINKER_FLAGS_INIT "-static")
# So that `ctest` runs the cross built binaries rather than failing on them.
set(CMAKE_CROSSCOMPILING_EMULATOR qemu-s390x-static)
