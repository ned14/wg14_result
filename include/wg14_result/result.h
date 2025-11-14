/* Proposed WG14 Result
(C) 2024 - 2026 Niall Douglas <http://www.nedproductions.biz/>
File Created: Nov 2025


Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License in the accompanying file
Licence.txt or at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef WG14_RESULT_STATUS_CODE_RESULT_H
#define WG14_RESULT_STATUS_CODE_RESULT_H

#include "status_code_system.h"

#ifdef __cplusplus
extern "C"
{
#endif

  // Identical to
  // https://github.com/ned14/outcome/blob/develop/include/outcome/detail/value_storage.hpp
  // As a result, it does contain some values peculiar to C++, however that then
  // shows correctly in the debugger for results which came from C++. As
  // described there, it has been found the painful way that spelling out every
  // possible configuration of status bits is the least worst approach for
  // codegen.
  enum WG14_RESULT_PREFIX(result_status_flags)
#if __STDC_VERSION__ >= 202000L
  : uint16_t
#endif
  {
    // WARNING: These bits are not tracked by abi-dumper, but changing them will
    // break ABI! bits 0-5 in use.
    WG14_RESULT_PREFIX(result_status_flag_none) = 0,

    WG14_RESULT_PREFIX(result_status_flag_have_value) = (1U << 0U),
    WG14_RESULT_PREFIX(result_status_flag_have_error) = (1U << 1U),
    WG14_RESULT_PREFIX(result_status_flag_have_exception) = (2U << 1U),
    WG14_RESULT_PREFIX(result_status_flag_have_error_exception) = (3U << 1U),

    // failed to complete a strong swap
    WG14_RESULT_PREFIX(result_status_flag_have_lost_consistency) = (1U << 3U),

    WG14_RESULT_PREFIX(result_status_flag_have_value_lost_consistency) =
    (1U << 0U) | (1U << 3U),
    WG14_RESULT_PREFIX(result_status_flag_have_error_lost_consistency) =
    (1U << 1U) | (1U << 3U),
    WG14_RESULT_PREFIX(result_status_flag_have_exception_lost_consistency) =
    (2U << 1U) | (1U << 3U),
    WG14_RESULT_PREFIX(
    result_status_flag_have_error_exception_lost_consistency) = (3U << 1U) |
                                                                (1U << 3U),

    // can errno be set from this error?
    WG14_RESULT_PREFIX(result_status_flag_have_error_is_errno) = (1U << 4U),

    WG14_RESULT_PREFIX(result_status_flag_have_error_error_is_errno) =
    (1U << 1U) | (1U << 4U),
    WG14_RESULT_PREFIX(result_status_flag_have_error_exception_error_is_errno) =
    (3U << 1U) | (1U << 4U),

    WG14_RESULT_PREFIX(
    result_status_flag_have_error_lost_consistency_error_is_errno) =
    (1U << 1U) | (1U << 3U) | (1U << 4U),
    WG14_RESULT_PREFIX(
    result_status_flag_have_error_exception_lost_consistency_error_is_errno) =
    (3U << 1U) | (1U << 3U) | (1U << 4U),

    // value has been moved from
    WG14_RESULT_PREFIX(result_status_flag_have_moved_from) = (1U << 5U),

    WG14_RESULT_PREFIX(result_status_flag_have_value_moved_from) = (1U << 0U) |
                                                                   (1U << 5U),
    WG14_RESULT_PREFIX(result_status_flag_have_error_moved_from) = (1U << 1U) |
                                                                   (1U << 5U),
    WG14_RESULT_PREFIX(result_status_flag_have_exception_moved_from) =
    (2U << 1U) | (1U << 5U),
    WG14_RESULT_PREFIX(result_status_flag_have_error_exception_moved_from) =
    (3U << 1U) | (1U << 5U),

    WG14_RESULT_PREFIX(
    result_status_flag_have_value_lost_consistency_moved_from) = (1U << 0U) |
                                                                 (1U << 3U) |
                                                                 (1U << 5U),
    WG14_RESULT_PREFIX(
    result_status_flag_have_error_lost_consistency_moved_from) = (1U << 1U) |
                                                                 (1U << 3U) |
                                                                 (1U << 5U),
    WG14_RESULT_PREFIX(
    result_status_flag_have_exception_lost_consistency_moved_from) =
    (2U << 1U) | (1U << 3U) | (1U << 5U),
    WG14_RESULT_PREFIX(
    result_status_flag_have_error_exception_lost_consistency_moved_from) =
    (3U << 1U) | (1U << 3U) | (1U << 5U),

    WG14_RESULT_PREFIX(result_status_flag_have_error_is_errno_moved_from) =
    (1U << 4U) | (1U << 5U),
    WG14_RESULT_PREFIX(
    result_status_flag_have_error_error_is_errno_moved_from) = (1U << 1U) |
                                                               (1U << 4U) |
                                                               (1U << 5U),
    WG14_RESULT_PREFIX(
    result_status_flag_have_error_exception_error_is_errno_moved_from) =
    (3U << 1U) | (1U << 4U) | (1U << 5U),

    WG14_RESULT_PREFIX(
    result_status_flag_have_error_lost_consistency_error_is_errno_moved_from) =
    (1U << 1U) | (1U << 3U) | (1U << 4U) | (1U << 5U),
    WG14_RESULT_PREFIX(
    result_status_flag_have_error_exception_lost_consistency_error_is_errno_moved_from) =
    (3U << 1U) | (1U << 3U) | (1U << 4U) | (1U << 5U),
  };

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#endif
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(                                                               \
disable : 4201)  // nonstandard extension used: nameless struct/union
#endif

  //! \brief The flags type within a Result
  union WG14_RESULT_PREFIX(result_flags)
  {
    // We use a union to provide backwards compatible ABI on C standards which
    // don't support enums with underlying type other than int
    enum WG14_RESULT_PREFIX(result_status_flags) status;
#if !(defined(SWIG) && __cplusplus)
    struct
    {
      uint16_t status_bits;          // status above, low bits
      uint16_t spare_storage_value;  // spare storage sixteen bits, high bits
    };
#endif
  };
#if __STDC_VERSION__ >= 201100L
  _Static_assert(sizeof(union WG14_RESULT_PREFIX(result_flags)) ==
                 sizeof(uint32_t),
                 "result_flags is not the size it is supposed to be!");
  _Static_assert(__alignof(union WG14_RESULT_PREFIX(result_flags)) <=
                 __alignof(uint32_t),
                 "result_flags does not have the alignment it is supposed to!");
#endif
#ifdef _MSC_VER
#pragma warning(pop)
#endif
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif

  //! \brief A Result for type `void`.
  typedef struct WG14_RESULT_PREFIX(result_with_void)
  {
    union WG14_RESULT_PREFIX(result_flags) _flags_;
    WG14_RESULT_PREFIX(status_code_system) error;
  } WG14_RESULT_PREFIX(result_with_void_t);
#if __STDC_VERSION__ >= 201100L
  _Static_assert(sizeof(WG14_RESULT_PREFIX(result_with_void_t)) ==
                 3 * sizeof(void *),
                 "Result is not the size it is supposed to be!");
  _Static_assert(__alignof(WG14_RESULT_PREFIX(result_with_void_t)) ==
                 __alignof(void *),
                 "Result does not have the alignment it is supposed to!");
#endif

  //! \brief Makes a successful Result with type `void`.
  WG14_RESULT_INLINE struct WG14_RESULT_PREFIX(result_with_void)
  WG14_RESULT_PREFIX(result_void_make_success)(void)
  {
    struct WG14_RESULT_PREFIX(result_with_void)
    ret = {{WG14_RESULT_PREFIX(result_status_flag_have_value)},
           WG14_RESULT_PREFIX(status_code_system_empty)};
    return ret;
  }
  //! \brief Makes an unsuccessful Result with type `void`.
  WG14_RESULT_INLINE struct WG14_RESULT_PREFIX(result_with_void)
  WG14_RESULT_PREFIX(result_void_make_failure)(
  WG14_RESULT_PREFIX(status_code_system) err)
  {
    struct WG14_RESULT_PREFIX(result_with_void)
    ret = {{WG14_RESULT_PREFIX(result_status_flag_have_error)}, err};
    return ret;
  }

//! \brief Declare a Result named `name` with successful type `T`
#define WG14_RESULT_DECLARE(T, name)                                           \
  struct WG14_RESULT_PREFIX(result_with_##name)                                \
  {                                                                            \
    T value;                                                                   \
    union WG14_RESULT_PREFIX(result_flags) _flags_;                            \
    WG14_RESULT_PREFIX(status_code_system) error;                              \
  };                                                                           \
  WG14_RESULT_INLINE struct WG14_RESULT_PREFIX(result_with_##name)             \
  WG14_RESULT_PREFIX(result_##name##_make_success)(T val)                      \
  {                                                                            \
    struct WG14_RESULT_PREFIX(result_with_##name)                              \
    ret = {val,                                                                \
           {WG14_RESULT_PREFIX(result_status_flag_have_value)},                \
           WG14_RESULT_PREFIX(status_code_system_empty)};                      \
    return ret;                                                                \
  }                                                                            \
  WG14_RESULT_INLINE struct WG14_RESULT_PREFIX(result_with_##name)             \
  WG14_RESULT_PREFIX(result_##name##_make_failure)(                            \
  WG14_RESULT_PREFIX(status_code_system) err)                                  \
  {                                                                            \
    struct WG14_RESULT_PREFIX(result_with_##name) ret;                         \
    memset(&ret, 0, sizeof(ret));                                              \
    ret.error = err;                                                           \
    ret._flags_.status = WG14_RESULT_PREFIX(result_status_flag_have_error);    \
    return ret;                                                                \
  }
//! \brief The type of a Result named `name` previously declared.
#define WG14_RESULT(name) struct WG14_RESULT_PREFIX(result_with_##name)
//! \brief Make a successful named Result previously declared.
#define WG14_RESULT_MAKE_SUCCESS(name, ...)                                    \
  WG14_RESULT_PREFIX(result_##name##_make_success)(__VA_ARGS__)
//! \brief Make a failed named Result previously declared.
#define WG14_RESULT_MAKE_FAILURE(name, ...)                                    \
  WG14_RESULT_PREFIX(result_##name##_make_failure)(__VA_ARGS__)
//! \brief True if a Result is successful (and contains a value if not `void`)
#define WG14_RESULT_HAS_VALUE(...)                                             \
  ((__VA_ARGS__)._flags_.status_bits &                                         \
   WG14_RESULT_PREFIX(result_status_flag_have_value))
//! \brief True if a Result is a failure (and contains an error)
#define WG14_RESULT_HAS_ERROR(...)                                             \
  ((__VA_ARGS__)._flags_.status_bits &                                         \
   WG14_RESULT_PREFIX(result_status_flag_have_error))

#ifdef __cplusplus
}
#endif

#endif
