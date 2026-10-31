add_code_test(result_test SOURCES "result_test.c")

add_code_test(status_code_test SOURCES "status_code_test.c")

# The layout matrix, compiled as C in every C language mode rather than once as
# C++: the status word's placement is decided by the underlying type of the
# flags enum, and C11/C17 (no fixed underlying type available -> the enum there
# is `int` sized) are precisely the modes which used to lay a Result out
# differently from status_result<R>. Outcome's test/tests/cabi-layout.cpp covers
# the C++ side and the identity with status_result<R>; this covers C11, C17 and
# C23. BUILD_TESTS_AS_CXX is deliberately not honoured here - these targets are
# the C compilation.
foreach(WG14_LAYOUT_C_STANDARD 11 17 23)
  set(WG14_LAYOUT_TARGET "layout_test_c${WG14_LAYOUT_C_STANDARD}")
  set_source_files_properties("layout_test.c" PROPERTIES LANGUAGE C)
  add_executable(${WG14_LAYOUT_TARGET} "layout_test.c")
  target_compile_features(${WG14_LAYOUT_TARGET} PRIVATE
    "c_std_${WG14_LAYOUT_C_STANDARD}")
  set_target_properties(${WG14_LAYOUT_TARGET} PROPERTIES
    C_STANDARD ${WG14_LAYOUT_C_STANDARD}
    C_STANDARD_REQUIRED ON
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )
  if(NOT WIN32)
    target_compile_options(${WG14_LAYOUT_TARGET} PRIVATE -Wall -Wextra -Wpedantic)
  endif()
  target_link_libraries(${WG14_LAYOUT_TARGET} PRIVATE ${PROJECT_NAME})
    # COMMAND names the target rather than $<TARGET_FILE:...> so that CMake
  # prepends CMAKE_CROSSCOMPILING_EMULATOR: the big endian lane builds for
  # s390x and runs the binaries under qemu, and a bare path would execute them
  # natively and fail.
  add_test(NAME ${WG14_LAYOUT_TARGET} COMMAND ${WG14_LAYOUT_TARGET})
endforeach()

# The same matrix as C++ at C++11. CMakeLists.txt asks for `cxx_std_11` on the
# tests, but that is a *minimum*, so a build without this target compiles the
# headers at whatever standard the compiler defaults to and never at C++11. This
# target pins C++11: the defaulted assignment operators are not constexpr there,
# and the storage structs' default member initialisers keep them aggregates, so
# C++11 takes paths the later standards do not.
set_source_files_properties("layout_test_cxx.cpp" PROPERTIES LANGUAGE CXX)
add_executable(layout_test_cxx11 "layout_test_cxx.cpp")
set_target_properties(layout_test_cxx11 PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
if(NOT WIN32)
  target_compile_options(layout_test_cxx11 PRIVATE -Wall -Wextra -Wpedantic)
endif()
target_link_libraries(layout_test_cxx11 PRIVATE ${PROJECT_NAME})
add_test(NAME layout_test_cxx11 COMMAND layout_test_cxx11)

# C89 and C++98: the header set must still *parse* in the modes it claims and
# which cannot express a static assertion, so these assert nothing at compile
# time and instead run the same bytes back through result_flags_get_status().
# See the two files for why those modes matter to the flags enum's declaration
# form, which is what a previous version of the header got wrong for C++98 (it
# keyed the sized enum off `#ifdef __cplusplus`).
#
# Both are strict ISO, so the language dialect is the same on every compiler: the
# headers are free of `//` comments, trailing enumerator commas and the other C99
# and C++11 conveniences, and the only thing these modes do not have is a
# variadic macro, which gcc and clang accept as an extension in both (a warning
# only under -pedantic).
add_executable(header_smoke_c89 "header_smoke.c")
set_target_properties(header_smoke_c89 PROPERTIES
  C_STANDARD 90
  C_STANDARD_REQUIRED ON
  C_EXTENSIONS OFF
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
if(NOT WIN32)
  target_compile_options(header_smoke_c89 PRIVATE -Wall -Wextra)
endif()
target_link_libraries(header_smoke_c89 PRIVATE ${PROJECT_NAME})
add_test(NAME header_smoke_c89 COMMAND header_smoke_c89)

# MSVC has no C++98 mode - its oldest is /std:c++14 - so CXX_STANDARD 98 is an
# error there rather than a no-op, and there is no way to ask that compiler for
# the parse this file proves. The C89 half above still runs under MSVC, whose
# legacy C mode is C89-shaped.
if(NOT MSVC)
set_source_files_properties("header_smoke_cxx98.cpp" PROPERTIES LANGUAGE CXX)
add_executable(header_smoke_cxx98 "header_smoke_cxx98.cpp")
set_target_properties(header_smoke_cxx98 PROPERTIES
  CXX_STANDARD 98
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
if(NOT WIN32)
  target_compile_options(header_smoke_cxx98 PRIVATE -Wall -Wextra)
endif()
target_link_libraries(header_smoke_cxx98 PRIVATE ${PROJECT_NAME})
add_test(NAME header_smoke_cxx98 COMMAND header_smoke_cxx98)
endif()
