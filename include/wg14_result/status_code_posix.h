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

#ifndef WG14_RESULT_STATUS_CODE_POSIX_H
#define WG14_RESULT_STATUS_CODE_POSIX_H

#include "status_code_system.h"

#ifdef __cplusplus
extern "C"
{
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4190)  /* C linkage */
#pragma warning(disable : 4996)  /* use strerror_s instead */
#endif
#endif

  /*! \brief Retrieve the domain for `status_code_posix` */
  WG14_RESULT_EXTERN
  WG14_RESULT_SINGLETON WG14_RESULT_PREFIX(status_code_domain) *
  WG14_RESULT_PREFIX(status_code_posix_domain)(void);

  struct WG14_RESULT_PREFIX(status_code_posix_s)
  {
    WG14_RESULT_PREFIX(status_code_untyped) base;
    int value
#if defined(__cplusplus) && !defined(WG14_RESULT_DISABLE_CXX_EXTENSIONS)
    {}
#endif
    ;
/* A default member initialiser keeps these storage structs from being aggregates
   in C++11, so there the brace initialisers the headers use need constructors.
   Before C++11 and from C++14 onwards they are aggregates and need none - in
   C++98 declaring one would take aggregate initialisation away, which is exactly
   what the headers rely on there. */
#if defined(__cplusplus) && WG14_RESULT_CXX_VERSION >= 201103L &&              \
WG14_RESULT_CXX_VERSION < 201402L && !defined(SWIG)
    /* C++11 only, see status_code_system.h: the NSDMI above keeps this from
       being an aggregate until C++14, so the brace initialisers need one
       constructor to spell, and a defaulted one so the NSDMI still applies.

       Hidden from SWIG, which parses at a language version where they exist and
       would then emit constructors that the wrapper's own compilation (C++14 or
       later) cannot see. */
    WG14_RESULT_CXX_CONSTEXPR WG14_RESULT_PREFIX(status_code_posix_s)() = default;
    WG14_RESULT_CXX_CONSTEXPR WG14_RESULT_PREFIX(status_code_posix_s)(
    const WG14_RESULT_PREFIX(status_code_untyped) & b, int v)
        : base(b)
        , value(v)
    {
    }
#endif
  };

  /*! \brief A POSIX status code */
#if defined(__cplusplus) && !defined(WG14_RESULT_DISABLE_CXX_EXTENSIONS)
  struct WG14_RESULT_PREFIX(status_code_posix)
      : wg14_result::WG14_RESULT_PREFIX(status_code_special_member_functions)<
        struct WG14_RESULT_PREFIX(status_code_posix_s)>
  {
    using wg14_result::WG14_RESULT_PREFIX(status_code_special_member_functions)<
    struct WG14_RESULT_PREFIX(status_code_posix_s)>::
    WG14_RESULT_PREFIX(status_code_special_member_functions);
  };
#else
typedef struct WG14_RESULT_PREFIX(status_code_posix_s)
WG14_RESULT_PREFIX(status_code_posix);
#endif

#if WG14_RESULT_C_VERSION >= 201112L
  _Static_assert(sizeof(WG14_RESULT_PREFIX(status_code_posix)) <=
                 sizeof(WG14_RESULT_PREFIX(status_code_system)),
                 "A POSIX status code is too large for status_code_system!");
  _Static_assert(
  __alignof(WG14_RESULT_PREFIX(status_code_posix)) <=
  __alignof(WG14_RESULT_PREFIX(status_code_system)),
  "A POSIX status code is too over aligned for status_code_system!");
#endif

  /*! \brief Make a `status_code_posix` */
  WG14_RESULT_EXTERN_INLINE WG14_RESULT_PREFIX(status_code_posix)
  WG14_RESULT_PREFIX(status_code_posix_make)(int val)
  {
    const WG14_RESULT_PREFIX(status_code_posix)
    ret = {WG14_RESULT_STATUS_CODE_SPECIAL_MEMBER_FUNCTIONS_INITIALISER(
    {WG14_RESULT_PREFIX(status_code_posix_domain)()}, val)};
    return ret;
  }

#ifdef __cplusplus
#ifdef _MSC_VER
#pragma warning(pop)
#endif
}
#endif

#if WG14_RESULT_ENABLE_HEADER_ONLY
#include "../../src/wg14_result/status_code_posix.c"
#endif

#endif
