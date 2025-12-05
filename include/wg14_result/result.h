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

#if defined(__cplusplus) && !defined(WG14_RESULT_DISABLE_CXX_EXTENSIONS)
    constexpr WG14_RESULT_PREFIX(result_flags)()
        : status{WG14_RESULT_PREFIX(result_status_flag_none)}
    {
    }
    constexpr /*implicit*/ WG14_RESULT_PREFIX(result_flags)(
    WG14_RESULT_PREFIX(result_status_flags) v)
        : status{v}
    {
    }
    constexpr WG14_RESULT_PREFIX(result_flags)(
    const WG14_RESULT_PREFIX(result_flags) &) = default;
    constexpr WG14_RESULT_PREFIX(result_flags)(
    WG14_RESULT_PREFIX(result_flags) &&) = default;
    constexpr WG14_RESULT_PREFIX(result_flags) &
    operator=(const WG14_RESULT_PREFIX(result_flags) &) = default;
    constexpr WG14_RESULT_PREFIX(result_flags) &
    operator=(WG14_RESULT_PREFIX(result_flags) &&) = default;
    WG14_RESULT_CXX_CONSTEXPR20 ~WG14_RESULT_PREFIX(result_flags)() = default;

    constexpr bool have_value() const noexcept
    {
      return (static_cast<uint16_t>(status_bits) &
              static_cast<uint16_t>(
              WG14_RESULT_PREFIX(result_status_flag_have_value))) != 0;
    }
    constexpr bool have_error() const noexcept
    {
      return (static_cast<uint16_t>(status_bits) &
              static_cast<uint16_t>(
              WG14_RESULT_PREFIX(result_status_flag_have_error))) != 0;
    }
    constexpr bool have_exception() const noexcept
    {
      return (static_cast<uint16_t>(status_bits) &
              static_cast<uint16_t>(
              WG14_RESULT_PREFIX(result_status_flag_have_exception))) != 0;
    }
    constexpr bool have_lost_consistency() const noexcept
    {
      return (static_cast<uint16_t>(status_bits) &
              static_cast<uint16_t>(WG14_RESULT_PREFIX(
              result_status_flag_have_lost_consistency))) != 0;
    }
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

  struct WG14_RESULT_PREFIX(result_with_void_s)
  {
    union WG14_RESULT_PREFIX(result_flags) _flags_;
    WG14_RESULT_PREFIX(status_code_system) error;
  };
  //! \brief A Result for type `void`.
#if defined(__cplusplus) && !defined(WG14_RESULT_DISABLE_CXX_EXTENSIONS)
  struct WG14_RESULT_PREFIX(result_with_void)
      : wg14_result::wg14_result_special_member_functions<WG14_RESULT_PREFIX(
        result_with_void_s)>
  {
    using wg14_result::wg14_result_special_member_functions<WG14_RESULT_PREFIX(
    result_with_void_s)>::wg14_result_special_member_functions;

    WG14_RESULT_CXX_CONSTEXPR20
    WG14_RESULT_PREFIX(result_with_void)() = default;
    WG14_RESULT_PREFIX(result_with_void)
    (const WG14_RESULT_PREFIX(result_with_void) &) = default;
    WG14_RESULT_PREFIX(result_with_void)
    (WG14_RESULT_PREFIX(result_with_void) &&) = default;
    WG14_RESULT_PREFIX(result_with_void) &
    operator=(const WG14_RESULT_PREFIX(result_with_void) &) = default;
    WG14_RESULT_PREFIX(result_with_void) &
    operator=(WG14_RESULT_PREFIX(result_with_void) &&) = default;
    WG14_RESULT_CXX_CONSTEXPR20 ~WG14_RESULT_PREFIX(result_with_void)() =
    default;
  };
#else
typedef struct WG14_RESULT_PREFIX(result_with_void_s)
WG14_RESULT_PREFIX(result_with_void);
#endif
#if __STDC_VERSION__ >= 201100L
  _Static_assert(sizeof(WG14_RESULT_PREFIX(result_with_void)) ==
                 3 * sizeof(void *),
                 "Result is not the size it is supposed to be!");
  _Static_assert(__alignof(WG14_RESULT_PREFIX(result_with_void)) ==
                 __alignof(void *),
                 "Result does not have the alignment it is supposed to!");
#endif

  //! \brief Makes a successful Result with type `void`.
  WG14_RESULT_EXTERN_INLINE WG14_RESULT_PREFIX(result_with_void)
  WG14_RESULT_PREFIX(result_void_make_success)(void)
  {
    const struct WG14_RESULT_PREFIX(result_with_void_s)
    ret1 = {{WG14_RESULT_PREFIX(result_status_flag_have_value)},
            WG14_RESULT_PREFIX(status_code_system_empty)};
    const WG14_RESULT_PREFIX(result_with_void) ret2 =
    WG14_RESULT_STATUS_CODE_SPECIAL_MEMBER_FUNCTIONS_INITIALISER(ret1);
    return ret2;
  }
  //! \brief Makes an unsuccessful Result with type `void`.
  WG14_RESULT_EXTERN_INLINE WG14_RESULT_PREFIX(result_with_void)
  WG14_RESULT_PREFIX(result_void_make_failure)(
  WG14_RESULT_PREFIX(status_code_system) err)
  {
    const struct WG14_RESULT_PREFIX(result_with_void_s)
    ret1 = {{WG14_RESULT_PREFIX(result_status_flag_have_error)}, err};
    const WG14_RESULT_PREFIX(result_with_void) ret2 =
    WG14_RESULT_STATUS_CODE_SPECIAL_MEMBER_FUNCTIONS_INITIALISER(ret1);
    return ret2;
  }

#define WG14_RESULT_DECLARE_IMPL1(T, name, ...)                                \
  struct WG14_RESULT_PREFIX(result_with_##name##_s)                            \
  {                                                                            \
    T value __VA_ARGS__;                                                       \
    union WG14_RESULT_PREFIX(result_flags) _flags_;                            \
    WG14_RESULT_PREFIX(status_code_system) error;                              \
  };
#define WG14_RESULT_DECLARE_IMPL2(T, name)                                     \
  WG14_RESULT_EXTERN_INLINE WG14_RESULT_PREFIX(result_with_##name)             \
  WG14_RESULT_PREFIX(result_##name##_make_success)(T val)                      \
  {                                                                            \
    struct WG14_RESULT_PREFIX(result_with_##name##_s)                          \
    ret1 = {val,                                                               \
            {WG14_RESULT_PREFIX(result_status_flag_have_value)},               \
            WG14_RESULT_PREFIX(status_code_system_empty)};                     \
    WG14_RESULT_PREFIX(result_with_##name)                                     \
    ret2 = WG14_RESULT_STATUS_CODE_SPECIAL_MEMBER_FUNCTIONS_INITIALISER(ret1); \
    return ret2;                                                               \
  }                                                                            \
  WG14_RESULT_EXTERN_INLINE WG14_RESULT_PREFIX(result_with_##name)             \
  WG14_RESULT_PREFIX(result_##name##_make_failure)(                            \
  WG14_RESULT_PREFIX(status_code_system) err)                                  \
  {                                                                            \
    WG14_RESULT_PREFIX(result_with_##name) ret;                                \
    WG14_RESULT_NOT_CXX_MEMSET(&ret, 0, sizeof(ret));                          \
    ret.error = err;                                                           \
    ret._flags_.status = WG14_RESULT_PREFIX(result_status_flag_have_error);    \
    return ret;                                                                \
  }

#if defined(__cplusplus) && !defined(WG14_RESULT_DISABLE_CXX_EXTENSIONS)
//! \brief Declare a Result named `name` with successful type `T`
#define WG14_RESULT_DECLARE(T, name)                                           \
  WG14_RESULT_DECLARE_IMPL1(T, name, {})                                       \
  struct WG14_RESULT_PREFIX(result_with_##name)                                \
      : wg14_result::wg14_result_special_member_functions<WG14_RESULT_PREFIX(  \
        result_with_##name##_s)>                                               \
  {                                                                            \
    using _base =                                                              \
    wg14_result::wg14_result_special_member_functions<WG14_RESULT_PREFIX(      \
    result_with_##name##_s)>;                                                  \
    using _base::_base;                                                        \
                                                                               \
    using value_type = T;                                                      \
    using error_type = WG14_RESULT_PREFIX(status_code_system);                 \
                                                                               \
    WG14_RESULT_CXX_CONSTEXPR20                                                \
    WG14_RESULT_PREFIX(result_with_##name)() = default;                        \
    WG14_RESULT_PREFIX(result_with_##name)                                     \
    (const WG14_RESULT_PREFIX(result_with_##name) &) = default;                \
    WG14_RESULT_PREFIX(result_with_##name)                                     \
    (WG14_RESULT_PREFIX(result_with_##name) &&) = default;                     \
    WG14_RESULT_PREFIX(result_with_##name) &                                   \
    operator=(const WG14_RESULT_PREFIX(result_with_##name) &) = default;       \
    WG14_RESULT_PREFIX(result_with_##name) &                                   \
    operator=(WG14_RESULT_PREFIX(result_with_##name) &&) = default;            \
    WG14_RESULT_CXX_CONSTEXPR20 ~WG14_RESULT_PREFIX(result_with_##name)() =    \
    default;                                                                   \
  };                                                                           \
  WG14_RESULT_DECLARE_IMPL2(T, name)
#else
#define WG14_RESULT_DECLARE_IMPL(T, name)                                      \
  WG14_RESULT_DECLARE_IMPL1(T, name, )                                         \
  typedef struct WG14_RESULT_PREFIX(result_with_##name##_s)                    \
  WG14_RESULT_PREFIX(result_with_##name);                                      \
  WG14_RESULT_DECLARE_IMPL2(T, name)
//! \brief Declare a Result named `name` with successful type `T`
#define WG14_RESULT_DECLARE(T, name) WG14_RESULT_DECLARE_IMPL(T, name)
#endif

#define WG14_RESULT_IMPL(name) WG14_RESULT_PREFIX(result_with_##name)
//! \brief The type of a Result named `name` previously declared.
#define WG14_RESULT(name) WG14_RESULT_IMPL(name)
#define WG14_RESULT_MAKE_SUCCESS_IMPL(name, ...)                               \
  WG14_RESULT_PREFIX(result_##name##_make_success)(__VA_ARGS__)
//! \brief Make a successful named Result previously declared.
#define WG14_RESULT_MAKE_SUCCESS(name, ...)                                    \
  WG14_RESULT_MAKE_SUCCESS_IMPL(name, __VA_ARGS__)
#define WG14_RESULT_MAKE_FAILURE_IMPL(name, ...)                               \
  WG14_RESULT_PREFIX(result_##name##_make_failure)(__VA_ARGS__)
//! \brief Make a failed named Result previously declared.
#define WG14_RESULT_MAKE_FAILURE(name, ...)                                    \
  WG14_RESULT_MAKE_FAILURE_IMPL(name, __VA_ARGS__)
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
#if defined(__cplusplus) && !defined(WG14_RESULT_DISABLE_CXX_EXTENSIONS)
namespace wg14_result
{
  template <class T> struct WG14_RESULT_PREFIX(result_storage)
  {
    T value{};
    union WG14_RESULT_PREFIX(result_flags) _flags_;
    WG14_RESULT_PREFIX(status_code_system) error;

    using value_type = T;
    using error_type = WG14_RESULT_PREFIX(status_code_system);
  };
  template <> struct WG14_RESULT_PREFIX(result_storage)<void>
  {
    union WG14_RESULT_PREFIX(result_flags) _flags_;
    WG14_RESULT_PREFIX(status_code_system) error;

    using value_type = void;
    using error_type = WG14_RESULT_PREFIX(status_code_system);
  };
  template <class T>
  struct WG14_RESULT_PREFIX(result)
      : wg14_result::wg14_result_special_member_functions <
      WG14_RESULT_PREFIX(result_storage) < T >>
  {
    using wg14_result::wg14_result_special_member_functions <
    WG14_RESULT_PREFIX(result_storage) < T >>
    ::wg14_result_special_member_functions;

    WG14_RESULT_CXX_CONSTEXPR20
    WG14_RESULT_PREFIX(result)() = default;
    constexpr WG14_RESULT_PREFIX(result)(const WG14_RESULT_PREFIX(result) &) =
    default;
    constexpr WG14_RESULT_PREFIX(result)(WG14_RESULT_PREFIX(result) &&) =
    default;
    constexpr WG14_RESULT_PREFIX(result) &
    operator=(const WG14_RESULT_PREFIX(result) &) = default;
    constexpr WG14_RESULT_PREFIX(result) &
    operator=(WG14_RESULT_PREFIX(result) &&) = default;
    WG14_RESULT_CXX_CONSTEXPR20 ~WG14_RESULT_PREFIX(result)() = default;
  };
}  // namespace wg14_result
#endif
#endif

#endif
