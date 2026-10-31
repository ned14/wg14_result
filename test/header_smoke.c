/* The WG14 Result headers must still parse as C89.
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

/* A pre-C11 C compilation of the header set.
   ---------------------------------------------------------------------------

   The disposition of a Result's status word is a property of the C and C++
   standards, not of a compiler: a fixed underlying type for the flags enum
   exists in C23 (and in C++11 and later) and nowhere else, so C89/C90, C11 and
   C17 must be able to compile the header with the enum held in plain
   `unsigned short` storage, and result.h derives the declaration form from the
   language version to arrange exactly that.

   The header carries no static assertion in this mode, because C89 cannot
   express one, so the value of this file is that it *compiles* and that the
   bytes it produces read back correctly: if the language-version derivation ever
   sent C89 down the sized-enum path (`enum x : unsigned short` is a syntax error
   there) or dropped the accessor, this would not build, and if the enum became
   the storage here the flag bits would not read back.

   It is deliberately written in C89: no `//` comments, no mixed declarations
   and statements, no `long long`.
*/

#include "wg14_result/result.h"

#include <stddef.h>

WG14_RESULT_DECLARE(char, c)
WG14_RESULT_DECLARE(short, s)
WG14_RESULT_DECLARE(long, l)

int main(void)
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
  if(!WG14_RESULT_HAS_VALUE(WG14_RESULT_MAKE_SUCCESS(s, (short) -2)))
  {
    return 3;
  }
  /* The two bytes of the status word read back as the enumerators. */
  if(result_flags_get_status(&ok._flags_) !=
     WG14_RESULT_PREFIX(result_status_flag_have_value))
  {
    return 4;
  }
  if(result_flags_get_status(&bad._flags_) !=
     WG14_RESULT_PREFIX(result_status_flag_have_error))
  {
    return 5;
  }
  /* Four bytes, of which the low two are the flags: this is the number which
     decides where in a Result the status word sits, and it cannot be an assert
     in this mode. */
  if(sizeof(union WG14_RESULT_PREFIX(result_flags)) != 4)
  {
    return 6;
  }
  if(offsetof(WG14_RESULT(l), _flags_) != 8)
  {
    return 7;
  }
  return 0;
}
