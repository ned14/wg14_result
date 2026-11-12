# A genuine 32-bit build on a 64-bit runner.
#
# The header is an ABI, and the one thing a 64-bit-only CI cannot see about an
# ABI is what it does when pointers are 4 bytes. On i386, GCC's *preferred*
# alignment for `long long` and `double` is 8 while the required alignment - and
# the alignment the compiler actually gives a struct holding one - is 4. Nothing
# in a 64-bit build can distinguish those two questions, because there they have
# the same answer, so an assertion which asked the wrong one passed everywhere
# until Outcome added a `-m32` lane and it failed there on a correct layout.
#
# The flags live in a toolchain file rather than in CFLAGS for two reasons: they
# then reach CMake's own compiler-capability and try_compile probes, so the
# configure step tests 32-bit behaviour rather than 64-bit; and the linker flags
# travel with them, which a bare CFLAGS does not do (a -m32 object cannot be
# linked by an unflaged 64-bit link step). g++-multilib supplies the 32-bit libc,
# libstdc++ and libgcc.
set(CMAKE_C_FLAGS_INIT "-m32")
set(CMAKE_CXX_FLAGS_INIT "-m32")
set(CMAKE_EXE_LINKER_FLAGS_INIT "-m32")
set(CMAKE_MODULE_LINKER_FLAGS_INIT "-m32")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "-m32")
