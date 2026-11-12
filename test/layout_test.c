/* The WG14 Result layout matrix, compiled as C.
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

/* This is the C half of the layout test. Outcome's test/tests/cabi-layout.cpp
proves, in C++, that a WG14 Result has the same sizeof, alignof and member
offsets as `status_result<R>` for every shape it declares, and that
`status_result<R>` itself obeys the rule this file asserts:

    value    at offset 0
    _flags_  at align_up(sizeof(T), 2)   - the status word, two uint16_t
    error    at align_up(_flags_ + 4, _Alignof(status_code_system))
    alignof  == max(_Alignof(T), _Alignof(status_code_system))
    sizeof   == align_up(error + sizeof(status_code_system), alignof(result))

cabi-layout.cpp can only speak C++, and status_result cannot be named from C at
all, so this file restates that rule and checks it in C11, C17 and C23: with the
C++ side asserting that status_result follows the same rule, a Result built by
one implementation and read by the other is the same bytes in every language
mode. It also pins the flags word itself: C11 and C17 cannot give an enum a fixed
underlying type, so an `int` sized enum in the status word would put that word at
align_up(sizeof(T), 4) in those two modes, a divergence that only shows itself
for sizeof(T) % 4 in {1, 2}.
*/

#include "wg14_result/result.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* The R matrix: the shapes which decide the layout, from a 1 byte char (where
   the status word moves if the flags word is 4 byte aligned) to a 16 byte
   alignment. */
struct lt_two16_s
{
  intptr_t a, b;
};
struct lt_s24_s
{
  int v[6];
};
struct lt_s5_s
{
  char v[5];
};
struct lt_s6_s
{
  char v[6];
};
struct lt_a16_s
{
  _Alignas(16) char v[16];
};

WG14_RESULT_DECLARE(_Bool, b)
WG14_RESULT_DECLARE(char, c)
WG14_RESULT_DECLARE(short, s)
WG14_RESULT_DECLARE(int, i)
WG14_RESULT_DECLARE(long, l)
WG14_RESULT_DECLARE(long long, ll)
WG14_RESULT_DECLARE(intptr_t, p)
WG14_RESULT_DECLARE(float, f)
WG14_RESULT_DECLARE(double, d)
WG14_RESULT_DECLARE(void *, vp)
WG14_RESULT_DECLARE(struct lt_two16_s, two16)
WG14_RESULT_DECLARE(struct lt_s24_s, s24)
WG14_RESULT_DECLARE(struct lt_s5_s, s5)
WG14_RESULT_DECLARE(struct lt_s6_s, s6)
WG14_RESULT_DECLARE(struct lt_a16_s, a16)

#define LT_ALIGN_UP(n, a) (((n) + (a) -1) / (a) * (a))

/* One row of the matrix: the rule above, spelled out against this header's own
   Result type, plus a runtime check that the two halves of the status word read
   back through the flags enum (which in C11/C17 means through
   result_flags_get_status()). */
#define LT_ROW(name, T, VALUE)                                                 \
  _Static_assert(offsetof(WG14_RESULT(name), value) == 0,                      \
                 #name ": the value is not at offset 0");                      \
  _Static_assert(offsetof(WG14_RESULT(name), _flags_) ==                       \
                 LT_ALIGN_UP(sizeof(T), 2),                                    \
                 #name ": the status word is not at align_up(sizeof(R), 2)");  \
  _Static_assert(offsetof(WG14_RESULT(name), error) ==                         \
                 LT_ALIGN_UP(offsetof(WG14_RESULT(name), _flags_) +            \
                                 sizeof(union WG14_RESULT_PREFIX(result_flags)),\
                             _Alignof(WG14_RESULT_PREFIX(                      \
                             status_code_system))),                            \
                 #name ": the status code is not where status_result<R> keeps "\
                 "it");                                                        \
  _Static_assert(sizeof(WG14_RESULT(name)) ==                                  \
                 LT_ALIGN_UP(offsetof(WG14_RESULT(name), error) +              \
                                 sizeof(WG14_RESULT_PREFIX(                    \
                                 status_code_system)),                         \
                             _Alignof(WG14_RESULT(name))),                     \
                 #name ": the Result is not the size status_result<R> is");    \
  _Static_assert(_Alignof(WG14_RESULT(name)) ==                                \
                 (_Alignof(T) > _Alignof(WG14_RESULT_PREFIX(                   \
                                 status_code_system))                          \
                      ? _Alignof(T)                                            \
                      : _Alignof(WG14_RESULT_PREFIX(                           \
                      status_code_system))),                                   \
                 #name ": the Result is not aligned like its most aligned "\
                 "member");                                                    \
  static int lt_row_##name(void)                                               \
  {                                                                            \
    const WG14_RESULT(name) succ = WG14_RESULT_MAKE_SUCCESS(name, VALUE);       \
    const WG14_RESULT(name) fail =                                             \
    WG14_RESULT_MAKE_FAILURE(name, WG14_RESULT_PREFIX(status_code_system_empty));\
    printf("%-6s %3zu %2zu %2zu %2zu %2zu\n", #name,                           \
           sizeof(WG14_RESULT(name)), _Alignof(WG14_RESULT(name)),             \
           offsetof(WG14_RESULT(name), value),                                 \
           offsetof(WG14_RESULT(name), _flags_),                               \
           offsetof(WG14_RESULT(name), error));                                \
    if(!WG14_RESULT_HAS_VALUE(succ))                                           \
    {                                                                          \
      printf("FAILED %s: a success does not read back as have_value\n", #name);\
      return 1;                                                                \
    }                                                                          \
    if(!WG14_RESULT_HAS_ERROR(fail))                                           \
    {                                                                          \
      printf("FAILED %s: a failure does not read back as have_error\n", #name);\
      return 1;                                                                \
    }                                                                          \
    if(result_flags_get_status(&succ._flags_) !=                               \
       WG14_RESULT_PREFIX(result_status_flag_have_value))                      \
    {                                                                          \
      printf("FAILED %s: the flags word does not read back as the have_value "\
             "enumerator (%u)\n",                                              \
             #name, (unsigned) result_flags_get_status(&succ._flags_));        \
      return 1;                                                                \
    }                                                                          \
    if(result_flags_get_status(&fail._flags_) !=                               \
       WG14_RESULT_PREFIX(result_status_flag_have_error))                      \
    {                                                                          \
      printf("FAILED %s: the flags word does not read back as the have_error "\
             "enumerator (%u)\n",                                              \
             #name, (unsigned) result_flags_get_status(&fail._flags_));        \
      return 1;                                                                \
    }                                                                          \
    return 0;                                                                  \
  }

LT_ROW(b, _Bool, 1)
LT_ROW(c, char, (char) -1)
LT_ROW(s, short, (short) -12345)
LT_ROW(i, int, 0x7fedcba9)
LT_ROW(l, long, 0x0102030405060708L)
LT_ROW(ll, long long, 0x0102030405060708LL)
LT_ROW(p, intptr_t, (intptr_t) 0x0102030405060708LL)
LT_ROW(f, float, 3.5f)
LT_ROW(d, double, -2.25)
LT_ROW(vp, void *, (void *) 0x1234)
LT_ROW(two16, struct lt_two16_s, ((struct lt_two16_s){1, 2}))
LT_ROW(s24, struct lt_s24_s, ((struct lt_s24_s){{0, 1, 2, 3, 4, 5}}))
LT_ROW(s5, struct lt_s5_s, ((struct lt_s5_s){{1, 2, 3, 4, 5}}))
LT_ROW(s6, struct lt_s6_s, ((struct lt_s6_s){{1, 2, 3, 4, 5, 6}}))
LT_ROW(a16, struct lt_a16_s, ((struct lt_a16_s){{7, 8}}))

/* The void Result, which has no value of its own but still has the one byte of
   value storage the C++ ABI has, so its status word is at align_up(1, 2) and not
   at 0. This is the row the matrix was missing: without it the void Result could
   differ from `status_result<void>` byte for byte and every other row would still
   pass. */
_Static_assert(offsetof(struct WG14_RESULT_PREFIX(result_with_void_s), _value_) == 0,
               "the void Result's value storage is not at offset 0");
_Static_assert(sizeof(((struct WG14_RESULT_PREFIX(result_with_void_s) *) 0)->_value_) ==
                   1,
               "the void Result's value storage is not one byte, so its status "
               "word is not where status_result<void> keeps it");
_Static_assert(offsetof(struct WG14_RESULT_PREFIX(result_with_void_s), _flags_) ==
                   LT_ALIGN_UP(
                   sizeof(((struct WG14_RESULT_PREFIX(result_with_void_s) *) 0)
                              ->_value_),
                   2),
               "the void Result's status word is not at align_up(sizeof(value), 2)");
_Static_assert(offsetof(struct WG14_RESULT_PREFIX(result_with_void_s), error) ==
                   LT_ALIGN_UP(
                   offsetof(struct WG14_RESULT_PREFIX(result_with_void_s), _flags_) +
                       sizeof(union WG14_RESULT_PREFIX(result_flags)),
                   _Alignof(WG14_RESULT_PREFIX(status_code_system))),
               "the void Result's status code is not where status_result<void> "
               "keeps it");

/* The flags word itself, in every language mode: four bytes, two byte aligned,
   whose low half is two bytes of flags whether that half is typed as the enum
   (C23 and C++) or as plain `unsigned short` (C11 and C17). */
_Static_assert(sizeof(union WG14_RESULT_PREFIX(result_flags)) == 4,
               "the flags word is not 4 bytes");
_Static_assert(_Alignof(union WG14_RESULT_PREFIX(result_flags)) == 2,
               "the flags word is not 2 byte aligned, so the status word sits "
               "in the wrong place");
/* The member order follows the endianness (see result.h): the low half of the
   word is the first two bytes on a little endian machine and the last two on a
   big endian one. */
#if WG14_RESULT_BIG_ENDIAN
_Static_assert(offsetof(union WG14_RESULT_PREFIX(result_flags),
                        spare_storage_value) == 0,
               "the spare storage is not in the high half of the flags word");
_Static_assert(offsetof(union WG14_RESULT_PREFIX(result_flags), status_bits) == 2,
               "the status bits are not in the low half of the flags word");
/* `status` is reached through a padding half in this order, so its offset is
   not a member of the union itself and cannot be taken with offsetof; that it
   aliases `status_bits` is what the numeric check below proves. */
#else
_Static_assert(offsetof(union WG14_RESULT_PREFIX(result_flags), status_bits) ==
               0,
               "the status bits are not the low half of the flags word");
_Static_assert(offsetof(union WG14_RESULT_PREFIX(result_flags),
                        spare_storage_value) == 2,
               "the spare storage is not the high half of the flags word");
/* As above: `status` aliases `status_bits` at offset 0, proved numerically by
   lt_numeric_layout() rather than by offsetof of a different member. */
#endif
_Static_assert(sizeof(((union WG14_RESULT_PREFIX(result_flags) *) 0)->status) ==
               2,
               "the flags member is not 2 bytes");

/* The endianness half of the same invariant, which no size or offset can
   express: the 32 bit word's LOW 16 bits are the flags and its HIGH 16 bits are
   the spare storage, on either endianness. result_flags declares its two uint16_t
   members in whichever order puts them there, so reading the word back as an
   integer has to produce 0xABCD0123 regardless of the machine. Compile time
   assertions cannot see this (offsetof says nothing about byte significance and
   the value is not a constant expression), so it is checked here and in the C++
   half of this test. */
static int lt_numeric_layout(void)
{
  union WG14_RESULT_PREFIX(result_flags) f;
  uint32_t word;
  f.status_bits = 0x0123U;
  f.spare_storage_value = 0xABCDU;
  memcpy(&word, &f, sizeof(word));
  printf("status word as a uint32 = 0x%08X (expected 0xABCD0123%s)\n",
         (unsigned) word,
         (word == 0xABCD0123U) ? "" : " MISMATCH");
  return (word == 0xABCD0123U) ? 0 : 1;
}

int main(void)
{
  int failures = 0;
  printf("sizeof(enum result_status_flags)=%zu __alignof=%zu (the storage is "
         "2 bytes in every mode)\n",
         sizeof(enum WG14_RESULT_PREFIX(result_status_flags)),
         _Alignof(enum WG14_RESULT_PREFIX(result_status_flags)));
  printf("%-6s %3s %2s %2s %2s %2s\n", "R", "sz", "al", "val", "flg", "err");
  failures += lt_row_b();
  failures += lt_row_c();
  failures += lt_row_s();
  failures += lt_row_i();
  failures += lt_row_l();
  failures += lt_row_ll();
  failures += lt_row_p();
  failures += lt_row_f();
  failures += lt_row_d();
  failures += lt_row_vp();
  failures += lt_row_two16();
  failures += lt_row_s24();
  failures += lt_row_s5();
  failures += lt_row_s6();
  failures += lt_row_a16();
  /* The void Result: value 0, status word 2, status code 8 on LP64 and LLP64
     alike, because the one byte of value storage is what shifts the word. */
  printf("%-6s %3zu %2zu %2zu %2zu %2zu\n", "void",
         sizeof(struct WG14_RESULT_PREFIX(result_with_void_s)),
         _Alignof(struct WG14_RESULT_PREFIX(result_with_void_s)),
         offsetof(struct WG14_RESULT_PREFIX(result_with_void_s), _value_),
         offsetof(struct WG14_RESULT_PREFIX(result_with_void_s), _flags_),
         offsetof(struct WG14_RESULT_PREFIX(result_with_void_s), error));
  if(offsetof(struct WG14_RESULT_PREFIX(result_with_void_s), _flags_) != 2)
  {
    printf("FAILED void: the status word is not at offset 2\n");
    failures++;
  }
  failures += lt_numeric_layout();
  if(failures != 0)
  {
    printf("%d matrix rows FAILED\n", failures);
    return 1;
  }
  printf("all matrix rows passed\n");
  return 0;
}
