cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
set(CMAKE_C_VISIBILITY_PRESET "hidden" CACHE STRING "Only make explicitly exported symbols visible")
set(CMAKE_C_STANDARD "11" CACHE STRING "C standard to use")
option(HEADER_ONLY_BUILD "Build using header only (unity build)" OFF)
option(BUILD_POSIX_SUPPORT "Build POSIX code support" ON)
option(BUILD_SWIG_WRAPPERS "Build SWIG wrappers" ON)
option(BUILD_RUST_WRAPPERS "Build RUST wrappers" ON)
option(BUILD_TESTS_AS_CXX "Build tests as C++" ON)
option(BUILD_SWIG_AS_CXX "Build SWIG wrappers as C++" ON)

project(wg14_result LANGUAGES C CXX)
include(GNUInstallDirs)
enable_testing()

set(LIBRARY_SOURCES
  "src/wg14_result/status_code_domain.c"
  "src/wg14_result/status_code_generic.c"
)
if(BUILD_POSIX_SUPPORT)
  list(APPEND LIBRARY_SOURCES "src/wg14_result/status_code_posix.c")
endif()

set(all_targets)
set(target ${PROJECT_NAME})
list(APPEND all_targets ${target})
add_library(${target} ${LIBRARY_SOURCES})
if(HEADER_ONLY_BUILD)
  target_compile_features(${target} PUBLIC c_std_11)
  target_compile_definitions(${target} PUBLIC WG14_RESULT_ENABLE_HEADER_ONLY)
else()
  target_compile_features(${target} PRIVATE c_std_11)
endif()
target_compile_definitions(${target} PRIVATE WG14_RESULT_SOURCE)
if(BUILD_POSIX_SUPPORT)
  target_compile_definitions(${target} PUBLIC WG14_RESULT_HAVE_POSIX_SUPPORT)
endif()
target_include_directories(${target} PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include>"
)
set_target_properties(${target} PROPERTIES
  POSITION_INDEPENDENT_CODE ON
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
if(WIN32)
  target_compile_options(${target} PRIVATE /W4 /experimental:c11atomics)
  target_compile_options(${target} PUBLIC /Zc:preprocessor)
else()
  target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

install(TARGETS ${all_targets}
        EXPORT ${PROJECT_NAME}Exports
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

configure_file(
  "${CMAKE_CURRENT_LIST_DIR}/cmake/ProjectConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  @ONLY
)
install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)
install(EXPORT ${PROJECT_NAME}Exports
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

if(BUILD_SWIG_WRAPPERS)
  find_package(SWIG COMPONENTS python)
  include(UseSWIG)
  foreach(language python)
    if(language MATCHES python)
      find_package(Python COMPONENTS Development.Module REQUIRED)
      set(link_libraries Python::Module)
      # We want the member functions to appear in Python
      if(BUILD_SWIG_AS_CXX)
        set_property(SOURCE "bindings/SWIG/${PROJECT_NAME}.i" PROPERTY CPLUSPLUS ON)
      endif()
      set_property(SOURCE "bindings/SWIG/${PROJECT_NAME}.i" PROPERTY COMPILE_OPTIONS "-doxygen")
    else()
      set_property(SOURCE "bindings/SWIG/${PROJECT_NAME}.i" PROPERTY CPLUSPLUS OFF)
    endif()
    #set_property(SOURCE "bindings/SWIG/${PROJECT_NAME}.i" PROPERTY COMPILE_OPTIONS "-E")
    set_property(SOURCE "bindings/SWIG/${PROJECT_NAME}.i" APPEND PROPERTY OBJECT_DEPENDS ${LIBRARY_SOURCES})
    swig_add_library(${PROJECT_NAME}_${language} TYPE SHARED LANGUAGE ${language}
      OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bindings/SWIG/${language}/"
      SOURCES "bindings/SWIG/${PROJECT_NAME}.i"
    )
    target_compile_definitions(${PROJECT_NAME}_${language} PUBLIC WG14_RESULT_DISABLE_CONVENIENCE_MACROS)
    set_target_properties(${PROJECT_NAME}_${language} PROPERTIES
      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
      SWIG_USE_TARGET_INCLUDE_DIRECTORIES TRUE
    )
    if(BUILD_SWIG_AS_CXX)
      target_compile_features(${PROJECT_NAME}_${language} PRIVATE cxx_std_11)
    endif()
    target_link_libraries(${PROJECT_NAME}_${language} PUBLIC ${PROJECT_NAME} ${link_libraries})
  endforeach()
endif()

if(BUILD_RUST_WRAPPERS)
  find_program(BINDGEN_PROGRAM bindgen)
  find_program(RUSTC_PROGRAM rustc REQUIRED)
  if(BINDGEN_PROGRAM)
    add_custom_command(OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/bindings/rust/raw_bindings.rs"
      COMMAND "${BINDGEN_PROGRAM}" -o "${CMAKE_CURRENT_SOURCE_DIR}/bindings/rust/raw_bindings.rs"
      "${CMAKE_CURRENT_SOURCE_DIR}/include/wg14_result/all.hpp"
      --allowlist-function 'result.*'
      --allowlist-function 'status_code.*'
      --allowlist-function 'wg14_result.*'
      --allowlist-type 'result.*'
      --allowlist-type 'status_code.*'
      --allowlist-type 'wg14_result.*'
      --allowlist-var 'result.*'
      --allowlist-var 'status_code.*'
      --allowlist-var 'wg14_result.*'
      --generate-inline-functions
      --no-layout-tests
      --with-derive-default
      --must-use-type result
      --
      -I ${CMAKE_CURRENT_LIST_DIR}/include
      DEPENDS ${LIBRARY_SOURCES}
    )
    add_custom_target(${PROJECT_NAME}_regenerate_rust_bindings DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/bindings/rust/raw_bindings.rs")
    add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_regenerate_rust_bindings)
  endif()
endif()

if(PROJECT_IS_TOP_LEVEL AND (NOT DEFINED BUILD_TESTING OR BUILD_TESTING))
  function(add_code_example target)
    cmake_parse_arguments(ADD_EXAMPLE "" "" "SOURCES;FEATURES;PROPERTIES" ${ARGN})
    if(BUILD_TESTS_AS_CXX)
      set_source_files_properties(${ADD_EXAMPLE_SOURCES} PROPERTIES LANGUAGE CXX)
    endif()
    add_executable(${target} ${ADD_EXAMPLE_SOURCES})
    if(WIN32)
      target_compile_options(${target} PRIVATE /W4 /experimental:c11atomics)
    else()
      if(NOT CMAKE_C_STANDARD STREQUAL 90)
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
      endif()
    endif()
    target_compile_features(${target} PRIVATE ${ADD_EXAMPLE_FEATURES})
    target_link_libraries(${target} PRIVATE ${PROJECT_NAME})
    set_target_properties(${target} PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
      ${ADD_EXAMPLE_PROPERTIES}
    )
    if(BUILD_TESTS_AS_CXX)
      target_compile_features(${target} PRIVATE cxx_std_11)
    endif()
  endfunction()
  function(add_code_test target)
    add_code_example(${target} ${ARGN})
    add_test(NAME ${target} COMMAND $<TARGET_FILE:${target}>)
  endfunction()

  add_subdirectory("test")
endif()

