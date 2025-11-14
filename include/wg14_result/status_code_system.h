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

#ifndef WG14_RESULT_STATUS_CODE_SYSTEM_H
#define WG14_RESULT_STATUS_CODE_SYSTEM_H

#include "status_code_generic.h"

#include <stdint.h>  // for intptr_t

#ifdef __cplusplus
extern "C"
{
#endif

  //! \brief A `status_code` guaranteed to be able to hold any system error code
  //! in full. The payload is always an `intptr_t`.
  typedef struct WG14_RESULT_PREFIX(status_code_system_s)
  {
    WG14_RESULT_PREFIX(status_code_untyped) base;
    intptr_t value;
  } WG14_RESULT_PREFIX(status_code_system);

  //| \brief An empty `status_code_system`
  static WG14_RESULT_CONSTEXPR_OR_CONST WG14_RESULT_PREFIX(status_code_system)
  WG14_RESULT_PREFIX(status_code_system_empty) = {{WG14_RESULT_NULLPTR}, 0};

  //! \brief Make a system code from an untyped input status code, this will
  //! fail if the input status cannot fit into a system code or if alignment
  //! requirements are not met.
  WG14_RESULT_INLINE WG14_RESULT_PREFIX(status_code_system)
  WG14_RESULT_PREFIX(status_code_system_make)(
  const WG14_RESULT_PREFIX(status_code_untyped) * src,
  WG14_RESULT_PREFIX(status_code_domain_payload_info_t) dstinfo)
  {
    WG14_RESULT_PREFIX(status_code_system)
    ret;
    if(dstinfo.total_size > sizeof(ret))
    {
      WG14_RESULT_ABORTF(
      "status_code_with_payload_system_make cannot make a system code sized "
      "%zu from an input status code sized %zu.",
      sizeof(ret), dstinfo.total_size);
    }
    if(dstinfo.total_alignment > __alignof(ret))
    {
      WG14_RESULT_ABORTF(
      "status_code_with_payload_system_make cannot make a system code aligned "
      "%zu from an input status code aligned %zu.",
      __alignof(ret), dstinfo.total_alignment);
    }
    memcpy(&ret, src, dstinfo.total_size);
    return ret;
  }

  //! \brief Make a system code from an `enum status_code_errc`
  WG14_RESULT_INLINE WG14_RESULT_PREFIX(status_code_system)
  WG14_RESULT_PREFIX(status_code_system_make_from_errc)(
  enum WG14_RESULT_PREFIX(status_code_errc) errc)
  {
    const union
    {
      WG14_RESULT_PREFIX(status_code_generic) gen;
      WG14_RESULT_PREFIX(status_code_system) sys;
    } ret = {WG14_RESULT_PREFIX(status_code_generic_make)(errc)};
    return ret.sys;
  }

#define STATUS_CODE_SYSTEM_MAKE_IMPL2(src, payload_size, total_size,           \
                                      total_alignment)                         \
  WG14_RESULT_PREFIX(status_code_system_make)                                  \
  (WG14_RESULT_STATIC_ASSERT_WITHIN_EXPR(                                      \
   (total_size) <= sizeof(WG14_RESULT_PREFIX(status_code_system)),             \
   "cannot make a system code from a larger sized input",                      \
   WG14_RESULT_STATIC_ASSERT_WITHIN_EXPR(                                      \
   (total_alignment) <= __alignof(WG14_RESULT_PREFIX(status_code_system)),     \
   "cannot make a system code from a more over aligned input", (src))),        \
   WG14_RESULT_PREFIX(status_code_domain_payload_info_make)(                   \
   (payload_size), (total_size), (total_alignment)))
#define STATUS_CODE_SYSTEM_MAKE_IMPL(src, ...)                                 \
  STATUS_CODE_SYSTEM_MAKE_IMPL2((src), __VA_ARGS__)

#ifndef WG14_RESULT_DISABLE_CONVENIENCE_MACROS
  //! \brief Make a system code from a typed input status code, this will
  //! fail if the input status cannot fit into a system code or if alignment
  //! requirements are not met.
#define status_code_system_make(...)                                           \
  STATUS_CODE_SYSTEM_MAKE_IMPL(                                                \
  &(__VA_ARGS__).base, STATUS_CODE_DOMAIN_PAYLOAD_INFO_INIT_INNARDS(           \
                       WG14_RESULT_TYPEOF_UNQUAL((__VA_ARGS__).value)))
#endif

#ifdef __cplusplus
}
#endif

#endif
