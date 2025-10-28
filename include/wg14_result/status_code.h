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

#define STATUS_CODE_WITH_PAYLOAD(T)                                            \
  struct WG14_RESULT_PREFIX(status_code_with_payload_##T)
#define STATUS_CODE_WITH_PAYLOAD_DECLARE(T, I)                                 \
  struct WG14_RESULT_PREFIX(status_code_with_payload_##I)                      \
  {                                                                            \
    WG14_RESULT_PREFIX(status_code_untyped) base;                              \
    T value;                                                                   \
  };                                                                           \
  WG14_RESULT_INLINE struct WG14_RESULT_PREFIX(status_code_with_payload_##I)   \
  WG14_RESULT_PREFIX(status_code_with_payload_##I##_make)(T val)               \
  {                                                                            \
    struct WG14_RESULT_PREFIX(status_code_with_payload_##I)                    \
    ret = {{&I##_domain}, val};                                                \
    return ret;                                                                \
  }
#define STATUS_CODE_WITH_PAYLOAD_MAKE(T)                                       \
  WG14_RESULT_PREFIX(status_code_with_payload_##T##_make)

  //! \brief True if the status code is empty (implementation)
  WG14_RESULT_INLINE bool WG14_RESULT_PREFIX(status_code_is_empty)(
  const WG14_RESULT_PREFIX(status_code_untyped) * r)
  {
    return r->domain == WG14_RESULT_NULLPTR;
  }

  //! \brief True if the status code is a success (implementation)
  WG14_RESULT_INLINE bool WG14_RESULT_PREFIX(status_code_is_success)(
  const WG14_RESULT_PREFIX(status_code_untyped) * r)
  {
    return (r->domain != WG14_RESULT_NULLPTR) ? !r->domain->vptr->failure(r) :
                                                false;
  }

  //! \brief True if the status code is a failure (implementation)
  WG14_RESULT_INLINE bool WG14_RESULT_PREFIX(status_code_is_failure)(
  const WG14_RESULT_PREFIX(status_code_untyped) * r)
  {
    return (r->domain != WG14_RESULT_NULLPTR) ? r->domain->vptr->failure(r) :
                                                false;
  }

  //! \brief Retrieves the message of the status code (implementation). Make
  //! sure you call `status_code_domain_string_ref_destroy()` when you are done
  //! with the returned string reference.
  WG14_RESULT_INLINE WG14_RESULT_PREFIX(status_code_domain_string_ref)
  WG14_RESULT_PREFIX(status_code_message)(
  const WG14_RESULT_PREFIX(status_code_untyped) * r)
  {
    if(r->domain != WG14_RESULT_NULLPTR)
    {
      return r->domain->vptr->message(r);
    }
    return WG14_RESULT_PREFIX(status_code_domain_string_ref_from_static_string)(
    "(empty)");
  }

  //! \brief True if the status codes are strictly semantically equivalent
  //! (implementation). `primary`'s domain's equivalence function decides if
  //! `secondary` is semantically equivalent to it. Note that for most status
  //! codes, if the codes have the same domain, a pure value equality is
  //! performed. This operation may not be transitive.
  WG14_RESULT_INLINE bool WG14_RESULT_PREFIX(status_code_strictly_equivalent)(
  const WG14_RESULT_PREFIX(status_code_untyped) * primary,
  const WG14_RESULT_PREFIX(status_code_untyped) * secondary)
  {
    if(primary->domain != WG14_RESULT_NULLPTR &&
       secondary->domain != WG14_RESULT_NULLPTR)
    {
      return primary->domain->vptr->equivalent(primary, secondary);
    }
    // If we are both empty, we are equivalent
    if(WG14_RESULT_NULLPTR == primary->domain &&
       WG14_RESULT_NULLPTR == secondary->domain)
    {
      return true;  // NOLINT
    }
    // Otherwise not equivalent
    return false;
  }


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

#ifdef __cplusplus
}
#endif

#endif
