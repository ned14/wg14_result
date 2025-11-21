/* Proposed WG14 Result
(C) 2024 - 2026 Niall Douglas <http://www.nedproductions.biz/>
File Created: Oct 2025


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

#ifndef WG14_RESULT_STATUS_CODE_H
#define WG14_RESULT_STATUS_CODE_H

#include "config.h"
#include "status_code_domain.h"

#ifdef __cplusplus
extern "C"
{
#endif
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996)  // use strerror_s instead
#endif

//! \brief The type of a status code with named payload. You must call
//! `STATUS_CODE_WITH_PAYLOAD_DECLARE(T, name)` first.
#define STATUS_CODE_WITH_PAYLOAD(name)                                         \
  WG14_RESULT_PREFIX(status_code_with_payload_##name)
#define STATUS_CODE_WITH_PAYLOAD_DECLARE_IMPL1(T, name)                        \
  struct WG14_RESULT_PREFIX(status_code_with_payload_##name##_s)               \
  {                                                                            \
    WG14_RESULT_PREFIX(status_code_untyped) base;                              \
    T value
#define STATUS_CODE_WITH_PAYLOAD_DECLARE_IMPL2(T, name)                        \
  WG14_RESULT_INLINE WG14_RESULT_PREFIX(status_code_with_payload_##name)       \
  WG14_RESULT_PREFIX(status_code_with_payload_##name##_make)(T val)            \
  {                                                                            \
    WG14_RESULT_PREFIX(status_code_with_payload_##name)                        \
    ret = {WG14_RESULT_STATUS_CODE_SPECIAL_MEMBER_FUNCTIONS_INITIALISER(       \
    {&name##_domain}, val)};                                                   \
    return ret;                                                                \
  }                                                                            \
  WG14_RESULT_INLINE WG14_RESULT_PREFIX(status_code_with_payload_##name)       \
  WG14_RESULT_PREFIX(status_code_with_payload_##name##_make_empty)(void)       \
  {                                                                            \
    WG14_RESULT_PREFIX(status_code_with_payload_##name) ret;                   \
    WG14_RESULT_NOT_CXX_MEMSET(&ret, 0, sizeof(ret));                          \
    return ret;                                                                \
  }
  /* \brief Declare a status code with payload of type `T` named `name`.

  Before you do this there needs to be a `static constexpr` variable
  declared called `name_domain`, otherwise the code emitted by this macro
  won't compile.
  */
#if defined(__cplusplus) && !defined(WG14_RESULT_DISABLE_CXX_EXTENSIONS)
#define STATUS_CODE_WITH_PAYLOAD_DECLARE(T, name)                              \
  STATUS_CODE_WITH_PAYLOAD_DECLARE_IMPL1(T, name);                             \
  }                                                                            \
  ;                                                                            \
  struct WG14_RESULT_PREFIX(status_code_with_payload_##name)                   \
      : wg14_result::WG14_RESULT_PREFIX(status_code_special_member_functions)< \
        WG14_RESULT_PREFIX(status_code_with_payload_##name##_s)>               \
  {                                                                            \
    using wg14_result::WG14_RESULT_PREFIX(                                     \
    status_code_special_member_functions)<                                     \
    WG14_RESULT_PREFIX(status_code_with_payload_##name##_s)>::                 \
    WG14_RESULT_PREFIX(status_code_special_member_functions);                  \
  };                                                                           \
  STATUS_CODE_WITH_PAYLOAD_DECLARE_IMPL2(T, name)
#else
#define STATUS_CODE_WITH_PAYLOAD_DECLARE(T, name)                              \
  STATUS_CODE_WITH_PAYLOAD_DECLARE_IMPL1(T, name);                             \
  }                                                                            \
  ;                                                                            \
  typedef struct WG14_RESULT_PREFIX(status_code_with_payload_##name##_s)       \
  WG14_RESULT_PREFIX(status_code_with_payload_##name);                         \
  STATUS_CODE_WITH_PAYLOAD_DECLARE_IMPL2(T, name)
#endif

//! \brief Make a named status code previously declared.
#define STATUS_CODE_WITH_PAYLOAD_MAKE(name, ...)                               \
  WG14_RESULT_PREFIX(status_code_with_payload_##name##_make)(__VA_ARGS__)

//! \brief Make an empty named status code previously declared.
#define STATUS_CODE_WITH_PAYLOAD_MAKE_EMPTY(name)                              \
  WG14_RESULT_PREFIX(status_code_with_payload_##name##_make_empty)()


#ifndef WG14_RESULT_DISABLE_CONVENIENCE_MACROS
//! \brief True if the status code is empty (convenience macro)
#define status_code_is_empty(...)                                              \
  WG14_RESULT_PREFIX(status_code_is_empty)(&(__VA_ARGS__).base)
//! \brief True if the status code is a success (convenience macro)
#define status_code_is_success(...)                                            \
  WG14_RESULT_PREFIX(status_code_is_success)(&(__VA_ARGS__).base)
//! \brief True if the status code is a failure (convenience macro)
#define status_code_is_failure(...)                                            \
  WG14_RESULT_PREFIX(status_code_is_failure)(&(__VA_ARGS__).base)
//! \brief Retrieves the message of the status code (convenience macro). Make
//! sure you call `status_code_domain_string_ref_destroy()` when you are done
//! with the returned string reference.
#define status_code_message(...)                                               \
  WG14_RESULT_PREFIX(status_code_message)(&(__VA_ARGS__).base)
//! \brief True if the status codes are strictly semantically equivalent
//! (convenience macro). `primary`'s domain's equivalence function decides if
//! `secondary` is semantically equivalent to it. Note that for most status
//! codes, if the codes have the same domain, a pure value equality is
//! performed. This operation may not be transitive.
#define status_code_strictly_equivalent(primary, secondary)                    \
  WG14_RESULT_PREFIX(status_code_strictly_equivalent)                          \
  (&(primary).base, &(secondary).base)
//! \brief Destroys the status code (type aware)
#define status_code_destroy(...)                                               \
  {                                                                            \
    typedef WG14_RESULT_TYPEOF_UNQUAL(*(__VA_ARGS__))                          \
    WG14_RESULT_PREFIX(_code_type_);                                           \
    WG14_RESULT_PREFIX(_code_type_) * WG14_RESULT_PREFIX(_code_ptr_) =         \
    (WG14_RESULT_PREFIX(_code_type_) *) (&(*(__VA_ARGS__)));                   \
    if(WG14_RESULT_PREFIX(_code_ptr_)->base.domain != WG14_RESULT_NULLPTR &&   \
       WG14_RESULT_PREFIX(_code_ptr_)->base.domain->vptr->erased_destroy !=    \
       WG14_RESULT_NULLPTR)                                                    \
    {                                                                          \
      typedef WG14_RESULT_TYPEOF(WG14_RESULT_PREFIX(_code_ptr_)->value)        \
      WG14_RESULT_PREFIX(_value_type_);                                        \
      const WG14_RESULT_PREFIX(status_code_domain_payload_info_t)              \
      WG14_RESULT_PREFIX(_info_) =                                             \
      STATUS_CODE_DOMAIN_PAYLOAD_INFO_INIT(WG14_RESULT_PREFIX(_value_type_));  \
      WG14_RESULT_VTABLE_INVOKE_API(                                           \
      WG14_RESULT_PREFIX(_code_ptr_)->base.domain, erased_destroy,             \
      &WG14_RESULT_PREFIX(_code_ptr_)->base, WG14_RESULT_PREFIX(_info_));      \
      WG14_RESULT_PREFIX(_code_ptr_)->base.domain = WG14_RESULT_NULLPTR;       \
    }                                                                          \
  }
#if !defined(_MSC_VER) || defined(__clang__)
#ifdef __GCC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
//! \brief Clones the status code (type aware)
#define status_code_clone(dest, src)                                           \
  ({                                                                           \
    typedef WG14_RESULT_TYPEOF(*(dest)) WG14_RESULT_PREFIX(_dest_code_type_);  \
    typedef WG14_RESULT_TYPEOF(*(src)) WG14_RESULT_PREFIX(_src_code_type_);    \
    WG14_RESULT_PREFIX(_dest_code_type_) *                                     \
    WG14_RESULT_PREFIX(_dest_code_ptr_) =                                      \
    (WG14_RESULT_PREFIX(_dest_code_type_) *) (&(*(dest)));                     \
    WG14_RESULT_PREFIX(_src_code_type_) * WG14_RESULT_PREFIX(_src_code_ptr_) = \
    (WG14_RESULT_PREFIX(_src_code_type_) *) (&(*(src)));                       \
    typedef WG14_RESULT_TYPEOF(WG14_RESULT_PREFIX(_dest_code_ptr_)->value)     \
    WG14_RESULT_PREFIX(_dest_value_type_);                                     \
    status_code_destroy(_dest_code_ptr_);                                      \
    int WG14_RESULT_PREFIX(ret) = 0;                                           \
    if(WG14_RESULT_PREFIX(_src_code_ptr_)->base.domain != WG14_RESULT_NULLPTR) \
    {                                                                          \
      const WG14_RESULT_PREFIX(status_code_domain_payload_info_t)              \
      WG14_RESULT_PREFIX(_info_) = STATUS_CODE_DOMAIN_PAYLOAD_INFO_INIT(       \
      WG14_RESULT_PREFIX(_dest_value_type_));                                  \
      WG14_RESULT_PREFIX(ret) = WG14_RESULT_VTABLE_INVOKE_API(                 \
      WG14_RESULT_PREFIX(_src_code_ptr_)->base.domain, erased_copy,            \
      &WG14_RESULT_PREFIX(_dest_code_ptr_)->base,                              \
      &WG14_RESULT_PREFIX(_src_code_ptr_)->base, WG14_RESULT_PREFIX(_info_));  \
    }                                                                          \
    WG14_RESULT_PREFIX(ret);                                                   \
  })
#ifdef __GCC__
#pragma GCC diagnostic pop
#endif
#endif
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#ifdef __cplusplus
}
#endif

#endif
