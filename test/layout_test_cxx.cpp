/* The WG14 Result layout matrix, compiled as C++ at C++11.
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

/* The C++ half of wg14_result's own layout test, at C++11.
   ---------------------------------------------------------------------------

   layout_test.c is compiled as C11, C17 and C23 because those are the modes
   whose flags *storage* differs; this file compiles the same matrix as C++ at
   C++11, which is what CMakeLists.txt asks the sources for via `cxx_std_11` and
   what the other C++ builds do not reach, since they compile at whatever
   standard the compiler defaults to. At C++11 the defaulted assignment operators
   are not constexpr and the storage structs' default member initialisers keep
   them aggregates, so the declaration paths differ from C++14 and later.

   What is asserted here, per shape, is the same invariant layout_test.c asserts
   and which Outcome's test/tests/cabi-layout.cpp asserts against
   `status_result<R>` itself:

     value    at offset 0
     _flags_  at align_up(sizeof(T), 2)   - the status word, two uint16_t
     error    at align_up(_flags_ + 4, alignof(status_code_system))
     alignof  == max(alignof(T), alignof(status_code_system))
     sizeof   == align_up(error + sizeof(status_code_system), alignof(result))

   and, the part a C test cannot express and this change is about, that the
   flags enum really is two bytes and really is the declared type of the status
   word's low half - i.e. that a debugger shows `result_status_flag_have_error`
   rather than `2`. The declaration form is derived from the language version in
   result.h; a C++11-or-later compilation is one of the modes which gets the
   sized enum, and these static_asserts fail to compile if it ever stops being
   so. (C11/C17 get the same bytes with `unsigned short` storage and
   result_flags_get_status() names them; C++98 and C89 get no static assert at
   all, because the language cannot express one - see result.h.)
*/

#include "wg14_result/result.h"

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <type_traits>

/* The R matrix. Everything is trivially copyable and padding-free, so the shape
   is the only variable. `alignas(16)` is C++11, which is what this file is
   built at. */
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
struct alignas(16) lt_a16_s
{
  char v[16];
};

WG14_RESULT_DECLARE(bool, b)
WG14_RESULT_DECLARE(char, c)
WG14_RESULT_DECLARE(short, s)
WG14_RESULT_DECLARE(int, i)
WG14_RESULT_DECLARE(long, l)
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
   Result type, plus a runtime check that both halves of the status word are
   where they belong and that the flags read back as the enumerators. */
#define LT_ROW(name, T, VALUE)                                                 \
  static_assert(offsetof(WG14_RESULT(name), value) == 0,                       \
                #name ": the value is not at offset 0");                       \
  static_assert(offsetof(WG14_RESULT(name), _flags_) ==                        \
                LT_ALIGN_UP(sizeof(T), 2),                                     \
                #name ": the status word is not at align_up(sizeof(R), 2)");   \
  static_assert(offsetof(WG14_RESULT(name), error) ==                          \
                LT_ALIGN_UP(offsetof(WG14_RESULT(name), _flags_) +             \
                                sizeof(union WG14_RESULT_PREFIX(               \
                                result_flags)),                                \
                            alignof(WG14_RESULT_PREFIX(                        \
                            status_code_system))),                             \
                #name ": the status code is not where status_result<R> keeps " \
                "it");                                                         \
  static_assert(sizeof(WG14_RESULT(name)) ==                                   \
                LT_ALIGN_UP(offsetof(WG14_RESULT(name), error) +               \
                                sizeof(WG14_RESULT_PREFIX(                     \
                                status_code_system)),                          \
                            alignof(WG14_RESULT(name))),                       \
                #name ": the Result is not the size status_result<R> is");     \
  static_assert(alignof(WG14_RESULT(name)) ==                                  \
                (alignof(T) > alignof(WG14_RESULT_PREFIX(                      \
                                 status_code_system))                          \
                     ? alignof(T)                                              \
                     : alignof(WG14_RESULT_PREFIX(                             \
                     status_code_system))),                                    \
                #name ": the Result is not aligned like its most aligned "     \
                "member");                                                     \
  static int lt_row_##name()                                                   \
  {                                                                            \
    const WG14_RESULT(name) succ = WG14_RESULT_MAKE_SUCCESS(name, VALUE);       \
    const WG14_RESULT(name) fail =                                             \
    WG14_RESULT_MAKE_FAILURE(name, WG14_RESULT_PREFIX(status_code_system_empty));\
    std::printf("%-6s %3zu %2zu %2zu %2zu %2zu\n", #name,                      \
                sizeof(WG14_RESULT(name)), alignof(WG14_RESULT(name)),         \
                offsetof(WG14_RESULT(name), value),                            \
                offsetof(WG14_RESULT(name), _flags_),                          \
                offsetof(WG14_RESULT(name), error));                           \
    if(!succ.has_value())                                                      \
    {                                                                          \
      std::printf("FAILED %s: a success does not read back as have_value\n",   \
                  #name);                                                      \
      return 1;                                                                \
    }                                                                          \
    if(!fail.has_error())                                                      \
    {                                                                          \
      std::printf("FAILED %s: a failure does not read back as have_error\n",   \
                  #name);                                                      \
      return 1;                                                                \
    }                                                                          \
    if(result_flags_get_status(&succ._flags_) !=                               \
       WG14_RESULT_PREFIX(result_status_flag_have_value))                      \
    {                                                                          \
      std::printf("FAILED %s: the flags word does not read back as the "       \
                  "have_value enumerator (%u)\n",                              \
                  #name,                                                       \
                  (unsigned) result_flags_get_status(&succ._flags_));          \
      return 1;                                                                \
    }                                                                          \
    if(result_flags_get_status(&fail._flags_) !=                               \
       WG14_RESULT_PREFIX(result_status_flag_have_error))                      \
    {                                                                          \
      std::printf("FAILED %s: the flags word does not read back as the "       \
                  "have_error enumerator (%u)\n",                              \
                  #name,                                                       \
                  (unsigned) result_flags_get_status(&fail._flags_));          \
      return 1;                                                                \
    }                                                                          \
    return 0;                                                                  \
  }

LT_ROW(b, bool, true)
LT_ROW(c, char, (char) -1)
LT_ROW(s, short, (short) -12345)
LT_ROW(i, int, 0x7fedcba9)
LT_ROW(l, long, 0x0102030405060708L)
LT_ROW(p, intptr_t, (intptr_t) 0x0102030405060708LL)
LT_ROW(f, float, 3.5f)
LT_ROW(d, double, -2.25)
LT_ROW(vp, void *, (void *) 0x1234)
LT_ROW(two16, struct lt_two16_s, (lt_two16_s{1, 2}))
LT_ROW(s24, struct lt_s24_s, (lt_s24_s{{0, 1, 2, 3, 4, 5}}))
LT_ROW(s5, struct lt_s5_s, (lt_s5_s{{1, 2, 3, 4, 5}}))
LT_ROW(s6, struct lt_s6_s, (lt_s6_s{{1, 2, 3, 4, 5, 6}}))
LT_ROW(a16, struct lt_a16_s, (lt_a16_s{{7, 8}}))

/* The status word: four bytes, two byte aligned, its low half two bytes of
   flags. In every C++ mode that half is declared as the flags enum, and the enum
   has a fixed 16 bit underlying type - which is the layout *and* the debugger
   requirement. `decltype` of an unparenthesised member access yields the
   declared type, so the last assert is exactly "the status field is the enum". */
static_assert(sizeof(union WG14_RESULT_PREFIX(result_flags)) == 4,
              "the flags word is not 4 bytes");
static_assert(alignof(union WG14_RESULT_PREFIX(result_flags)) == 2,
              "the flags word is not 2 byte aligned, so the status word sits in "
              "the wrong place");
/* The member order follows the endianness (see result.h): the low half of the
   word is the first two bytes on a little endian machine and the last two on a
   big endian one. `status` aliases `status_bits`, which the numeric check in
   lt_numeric_layout() proves, because in the big endian order it is reached
   through a padding half and so cannot be offsetof'd as a union member. */
#if WG14_RESULT_BIG_ENDIAN
static_assert(offsetof(union WG14_RESULT_PREFIX(result_flags),
                       spare_storage_value) == 0,
              "the spare storage is not in the high half of the flags word");
static_assert(offsetof(union WG14_RESULT_PREFIX(result_flags), status_bits) == 2,
              "the status bits are not in the low half of the flags word");
#else
static_assert(offsetof(union WG14_RESULT_PREFIX(result_flags), status_bits) == 0,
              "the status bits are not the low half of the flags word");
static_assert(offsetof(union WG14_RESULT_PREFIX(result_flags),
                       spare_storage_value) == 2,
              "the spare storage is not the high half of the flags word");
#endif
static_assert(sizeof(((union WG14_RESULT_PREFIX(result_flags) *) nullptr)->status) == 2,
              "the flags member is not 2 bytes");
static_assert(sizeof(enum WG14_RESULT_PREFIX(result_status_flags)) == 2,
              "the flags enum is not 2 bytes, so the status word cannot be typed "
              "with it and a debugger shows the raw bits");
static_assert(alignof(enum WG14_RESULT_PREFIX(result_status_flags)) == 2,
              "the flags enum is not 2 byte aligned");
static_assert(
std::is_same<decltype(((union WG14_RESULT_PREFIX(result_flags) *) nullptr)
                        ->status),
             enum WG14_RESULT_PREFIX(result_status_flags)>::value,
"the status word's low half is not declared as the flags enum, so a debugger "
"prints a number rather than the enumerator");

/* The endianness half of the invariant, as in the C half of this test: the low 16
   bits of the 32 bit status word are the flags and the high 16 are the spare
   storage, on either endianness, so the members are declared in whichever order
   puts them there and reading the word back as an integer must give
   0xABCD0123. */
static int lt_numeric_layout()
{
  union WG14_RESULT_PREFIX(result_flags) f;
  uint32_t word;
  f.status_bits = 0x0123U;
  f.spare_storage_value = 0xABCDU;
  std::memcpy(&word, &f, sizeof(word));
  std::printf("status word as a uint32 = 0x%08X (expected 0xABCD0123%s)\n",
              (unsigned) word, (word == 0xABCD0123U) ? "" : " MISMATCH");
  return (word == 0xABCD0123U) ? 0 : 1;
}

int main()
{
  int failures = 0;
  std::printf("sizeof(enum result_status_flags)=%zu alignof=%zu (the declared "
              "type of the status word's low half)\n",
              sizeof(enum WG14_RESULT_PREFIX(result_status_flags)),
              alignof(enum WG14_RESULT_PREFIX(result_status_flags)));
  std::printf("%-6s %3s %2s %2s %2s %2s\n", "R", "sz", "al", "val", "flg",
              "err");
  failures += lt_row_b();
  failures += lt_row_c();
  failures += lt_row_s();
  failures += lt_row_i();
  failures += lt_row_l();
  failures += lt_row_p();
  failures += lt_row_f();
  failures += lt_row_d();
  failures += lt_row_vp();
  failures += lt_row_two16();
  failures += lt_row_s24();
  failures += lt_row_s5();
  failures += lt_row_s6();
  failures += lt_row_a16();
  failures += lt_numeric_layout();
  if(failures != 0)
  {
    std::printf("%d matrix rows FAILED\n", failures);
    return 1;
  }
  std::printf("all matrix rows passed\n");
  return 0;
}
