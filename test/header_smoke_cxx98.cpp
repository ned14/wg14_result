/* The WG14 Result headers must still parse as C++98.
(C) 2024 - 2026 Niall Douglas <http://www.nedproductions.biz/>
File Created: Nov 2026


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

/* A C++98 compilation of the header set.
   ---------------------------------------------------------------------------

   config.h says that C++98 is supported: a C++ compiler older than C++11 gets an
   #error telling you to define WG14_RESULT_DISABLE_CXX_EXTENSIONS "to make this
   a pure C library". C++98 is therefore a mode these headers claim, and this
   file is what compiles them in it.

   What C++98 must get is the same disposition as C89, C11 and C17: no fixed
   underlying type for the flags enum (`enum x : unsigned short` is a C++11
   spelling), so the two bytes of the status word are `unsigned short` storage
   read back through result_flags_get_status(). That is why the declaration form
   comes from the language version, WG14_RESULT_SIZED_ENUMS_SUPPORTED in
   config.h, and not from a bare `#ifdef __cplusplus`, which C++98 cannot
   compile.

   No static assertion is possible at file scope in C++98 (result.h compiles the
   assertion away there rather than claiming one it cannot make), so this file's
   job is to compile and to show that the bytes still classify correctly. It is
   compiled with WG14_RESULT_DISABLE_CXX_EXTENSIONS, as config.h requires.
*/

#ifndef WG14_RESULT_DISABLE_CXX_EXTENSIONS
#define WG14_RESULT_DISABLE_CXX_EXTENSIONS 1
#endif

#include "wg14_result/result.h"

#include <cstddef>

WG14_RESULT_DECLARE(char, c)
WG14_RESULT_DECLARE(short, s)
WG14_RESULT_DECLARE(long, l)

int main()
{
  WG14_RESULT(l) ok = WG14_RESULT_MAKE_SUCCESS(l, 42L);
  WG14_RESULT(c) bad = WG14_RESULT_MAKE_FAILURE(
  c, WG14_RESULT_PREFIX(status_code_system_empty));

  if(!WG14_RESULT_HAS_VALUE(ok))
  {
    return 1;
  }
  if(!WG14_RESULT_HAS_ERROR(bad))
  {
    return 2;
  }
  /* The two bytes of the status word read back as the enumerators. */
  if(result_flags_get_status(&ok._flags_) !=
     WG14_RESULT_PREFIX(result_status_flag_have_value))
  {
    return 3;
  }
  if(result_flags_get_status(&bad._flags_) !=
     WG14_RESULT_PREFIX(result_status_flag_have_error))
  {
    return 4;
  }
  /* Four bytes, of which the low two are the flags - the number which decides
     where in a Result the status word sits, and which cannot be an assert in
     this mode. */
  if(sizeof(union WG14_RESULT_PREFIX(result_flags)) != 4)
  {
    return 5;
  }
  if(offsetof(WG14_RESULT(l), _flags_) != 8)
  {
    return 6;
  }
  return 0;
}
