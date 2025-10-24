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

#ifdef __cplusplus
}
#endif

#endif
