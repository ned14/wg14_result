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
#endif

  //! \brief Retrieve the domain for `status_code_posix`
  WG14_RESULT_EXTERN
  WG14_RESULT_SINGLETON WG14_RESULT_PREFIX(status_code_domain) *
  WG14_RESULT_PREFIX(status_code_posix_domain)(void);

  //! \brief A POSIX status code
  typedef struct WG14_RESULT_PREFIX(status_code_posix_s)
  {
    WG14_RESULT_PREFIX(status_code_untyped) base;
    int value;
  } WG14_RESULT_PREFIX(status_code_posix);
#if __STDC_VERSION__ >= 201100L
  _Static_assert(sizeof(WG14_RESULT_PREFIX(status_code_posix)) <=
                 sizeof(WG14_RESULT_PREFIX(status_code_system)),
                 "A POSIX status code is too large for status_code_system!");
  _Static_assert(
  __alignof(WG14_RESULT_PREFIX(status_code_posix)) <=
  __alignof(WG14_RESULT_PREFIX(status_code_system)),
  "A POSIX status code is too over aligned for status_code_system!");
#endif

  //! \brief Make a `status_code_posix`
  WG14_RESULT_INLINE WG14_RESULT_PREFIX(status_code_posix)
  WG14_RESULT_PREFIX(status_code_posix_make)(int val)
  {
    const WG14_RESULT_PREFIX(status_code_posix)
    ret = {{WG14_RESULT_PREFIX(status_code_posix_domain)()}, val};
    return ret;
  }

#ifdef __cplusplus
}
#endif

#if WG14_RESULT_ENABLE_HEADER_ONLY
#include "../../src/wg14_result/status_code_posix.c"
#endif

#endif
