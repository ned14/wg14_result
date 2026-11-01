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

#ifndef WG14_RESULT_STATUS_CODE_RESULT_H
#define WG14_RESULT_STATUS_CODE_RESULT_H

#include "status_code_system.h"

#include <stddef.h> /* for offsetof() */

#ifdef __cplusplus
extern "C"
{
#endif

  /*
     Whether this language mode can give the flags enum a fixed 16 bit underlying
     type is decided once in config.h as WG14_RESULT_SIZED_ENUMS_SUPPORTED, which
     is where the MSVC `__cplusplus` caveat is handled. What that means here:
     
       C++11 and later  - fixed underlying type (C++ had it first)
       C23 and later    - the same spelling in C
       everything else  - C++98, C89/C90, C11, C17: storage plus accessor
     
     In the sized-enum modes the enum itself is two bytes, so a Result's status
     word can hold the *enum typed* member and a debugger shows
     `result_status_flag_have_error` instead of `2`. In the other modes the enum
     is `int` sized (4 bytes, 4 byte aligned), so storing one inside a Result's
     status word would put that word at align_up(sizeof(R), 4) instead of the
     align_up(sizeof(R), 2) which `status_result<R>` uses and which the C ABI in
     outcome/experimental/result.h uses. Those modes therefore keep the two bytes
     as `unsigned short` and read them back through result_flags_get_status()
     below - same bytes, same layout, and C source still spells the enumerators
     with the enum's names. */

  /*
     Identical to
     https://github.com/ned14/outcome/blob/develop/include/outcome/detail/value_storage.hpp
     As a result, it does contain some values peculiar to C++, however that then
     shows correctly in the debugger for results which came from C++. As
     described there, it has been found the painful way that spelling out every
     possible configuration of status bits is the least worst approach for
     codegen. */
  enum WG14_RESULT_PREFIX(result_status_flags)
#if WG14_RESULT_SIZED_ENUMS_SUPPORTED
  : unsigned short
#endif
  {
    /*
       WARNING: These bits are not tracked by abi-dumper, but changing them will
       break ABI! bits 0-5 in use. */
    WG14_RESULT_PREFIX(result_status_flag_none) = 0,

    WG14_RESULT_PREFIX(result_status_flag_have_value) = (1U << 0U),
    WG14_RESULT_PREFIX(result_status_flag_have_error) = (1U << 1U),
    WG14_RESULT_PREFIX(result_status_flag_have_exception) = (2U << 1U),
    WG14_RESULT_PREFIX(result_status_flag_have_error_exception) = (3U << 1U),

    /*
       failed to complete a strong swap */
    WG14_RESULT_PREFIX(result_status_flag_have_lost_consistency) = (1U << 3U),

    WG14_RESULT_PREFIX(result_status_flag_have_value_lost_consistency) =
    (1U << 0U) | (1U << 3U),
    WG14_RESULT_PREFIX(result_status_flag_have_error_lost_consistency) =
    (1U << 1U) | (1U << 3U),
    WG14_RESULT_PREFIX(result_status_flag_have_exception_lost_consistency) =
    (2U << 1U) | (1U << 3U),
    WG14_RESULT_PREFIX(
    result_status_flag_have_error_exception_lost_consistency) = (3U << 1U) |
                                                                (1U << 3U),

    /*
       can errno be set from this error? */
    WG14_RESULT_PREFIX(result_status_flag_have_error_is_errno) = (1U << 4U),

    WG14_RESULT_PREFIX(result_status_flag_have_error_error_is_errno) =
    (1U << 1U) | (1U << 4U),
    WG14_RESULT_PREFIX(result_status_flag_have_error_exception_error_is_errno) =
    (3U << 1U) | (1U << 4U),

    WG14_RESULT_PREFIX(
    result_status_flag_have_error_lost_consistency_error_is_errno) =
    (1U << 1U) | (1U << 3U) | (1U << 4U),
    WG14_RESULT_PREFIX(
    result_status_flag_have_error_exception_lost_consistency_error_is_errno) =
    (3U << 1U) | (1U << 3U) | (1U << 4U),

    /*
       value has been moved from */
    WG14_RESULT_PREFIX(result_status_flag_have_moved_from) = (1U << 5U),

    WG14_RESULT_PREFIX(result_status_flag_have_value_moved_from) = (1U << 0U) |
                                                                   (1U << 5U),
    WG14_RESULT_PREFIX(result_status_flag_have_error_moved_from) = (1U << 1U) |
                                                                   (1U << 5U),
    WG14_RESULT_PREFIX(result_status_flag_have_exception_moved_from) =
    (2U << 1U) | (1U << 5U),
    WG14_RESULT_PREFIX(result_status_flag_have_error_exception_moved_from) =
    (3U << 1U) | (1U << 5U),

    WG14_RESULT_PREFIX(
    result_status_flag_have_value_lost_consistency_moved_from) = (1U << 0U) |
                                                                 (1U << 3U) |
                                                                 (1U << 5U),
    WG14_RESULT_PREFIX(
    result_status_flag_have_error_lost_consistency_moved_from) = (1U << 1U) |
                                                                 (1U << 3U) |
                                                                 (1U << 5U),
    WG14_RESULT_PREFIX(
    result_status_flag_have_exception_lost_consistency_moved_from) =
    (2U << 1U) | (1U << 3U) | (1U << 5U),
    WG14_RESULT_PREFIX(
    result_status_flag_have_error_exception_lost_consistency_moved_from) =
    (3U << 1U) | (1U << 3U) | (1U << 5U),

    WG14_RESULT_PREFIX(result_status_flag_have_error_is_errno_moved_from) =
    (1U << 4U) | (1U << 5U),
    WG14_RESULT_PREFIX(
    result_status_flag_have_error_error_is_errno_moved_from) = (1U << 1U) |
                                                               (1U << 4U) |
                                                               (1U << 5U),
    WG14_RESULT_PREFIX(
    result_status_flag_have_error_exception_error_is_errno_moved_from) =
    (3U << 1U) | (1U << 4U) | (1U << 5U),

    WG14_RESULT_PREFIX(
    result_status_flag_have_error_lost_consistency_error_is_errno_moved_from) =
    (1U << 1U) | (1U << 3U) | (1U << 4U) | (1U << 5U),
    WG14_RESULT_PREFIX(
    result_status_flag_have_error_exception_lost_consistency_error_is_errno_moved_from) =
    (3U << 1U) | (1U << 3U) | (1U << 4U) | (1U << 5U)
  };

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#endif
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(                                                               \
disable : 4201) /* nonstandard extension used: nameless struct/union */
#endif

  /*! \brief The flags type within a Result */
  /*
     
     This is the same 4 byte word as C++'s `status_bitfield_type`: two `uint16_t`
     halves, so aligned to 2 and *not* to 4. That alignment is ABI, not detail:
     a 4 byte member aligned to 4 lands the status word at
     align_up(sizeof(R), 4), which is where `status_result<R>` puts its own
     status word only when sizeof(R) % 4 is 0 or 3. For a `char`, a `short`, a
     `bool` (or any R of size 1, 2, 5, 6, ...) the two implementations then
     disagree about where the status word is, and a Result written by one and
     read by the other reads the wrong word (it comes back bitless).
     
     `status` is the low half of that word under the name (and, where the
     language can express it, the *type*) of the flags enum. In C23 and C++ the
     enum's underlying type is fixed to `unsigned short` above, so this member is
     two bytes of enum: the union stays 2 byte aligned and a debugger prints the
     enumerator's name. In C11 and C17 the enum cannot be given an underlying
     type, so an enum member here would be `int` sized and 4 byte aligned - the
     bug above - and the member is two bytes of `unsigned short` instead, which
     result_flags_get_status() reads back as the enum type. */
  union WG14_RESULT_PREFIX(result_flags)
  {
    struct
    {
      /*
         The two halves of the status word, in memory order. What is fixed by the
         design is the NUMERIC layout: flags in the low 16 bits of the 32 bit
         word, spare storage in the high 16. On a little endian machine the low
         half is the first two bytes; on a big endian one the first two bytes are
         the high half, so the members swap. The assertions below pin whichever
         order this machine needs, and test/layout_test.c re-derives the numeric
         layout through a memcpy so that neither endianness can drift. */
#if WG14_RESULT_BIG_ENDIAN
      uint16_t spare_storage_value;  /* spare storage sixteen bits, high bits */
      uint16_t status_bits;          /* status above, low bits */
#else
      uint16_t status_bits;          /* status above, low bits */
      uint16_t spare_storage_value;  /* spare storage sixteen bits, high bits */
#endif
    };
#if !(defined(SWIG) && __cplusplus)
#if WG14_RESULT_BIG_ENDIAN
    /*
       The same two bytes as `status_bits`, which are the LAST two bytes of the
       word here, so the member is reached through a padding half. */
    struct
    {
      uint16_t reserved_high_half;
#if WG14_RESULT_SIZED_ENUMS_SUPPORTED
      /*
         Two bytes *and* the enum type, which is what a debugger shows and what
         `_flags_.status = result_status_flag_...` assigns. */
      enum WG14_RESULT_PREFIX(result_status_flags) status;
#else
      /*
         The same two bytes, as plain storage, because a C11/C17 enum would be
         four. `_flags_.status = result_status_flag_...` still assigns (an enum
         converts to unsigned short) and result_flags_get_status() names it. */
      unsigned short status;
#endif
    };
#else
#if WG14_RESULT_SIZED_ENUMS_SUPPORTED
    /*
       Two bytes *and* the enum type, which is what a debugger shows and what
       `_flags_.status = result_status_flag_...` assigns. */
    enum WG14_RESULT_PREFIX(result_status_flags) status;
#else
    /*
       The same two bytes, as plain storage, because a C11/C17 enum would be
       four. `_flags_.status = result_status_flag_...` still assigns (an enum
       converts to unsigned short) and result_flags_get_status() names it. */
    unsigned short status;
#endif
#endif
#endif

#if defined(__cplusplus) && !defined(WG14_RESULT_DISABLE_CXX_EXTENSIONS)
    constexpr WG14_RESULT_PREFIX(result_flags)()
        : status{WG14_RESULT_PREFIX(result_status_flag_none)}
    {
    }
    constexpr /*implicit*/ WG14_RESULT_PREFIX(result_flags)(
    WG14_RESULT_PREFIX(result_status_flags) v)
        : status{v}
    {
    }
    constexpr WG14_RESULT_PREFIX(result_flags)(
    const WG14_RESULT_PREFIX(result_flags) &) = default;
    constexpr WG14_RESULT_PREFIX(result_flags)(
    WG14_RESULT_PREFIX(result_flags) &&) = default;
    WG14_RESULT_PREFIX(result_flags) &
    operator=(const WG14_RESULT_PREFIX(result_flags) &) = default;
    WG14_RESULT_PREFIX(result_flags) &
    operator=(WG14_RESULT_PREFIX(result_flags) &&) = default;
    ~WG14_RESULT_PREFIX(result_flags)() = default;

    constexpr bool have_value() const noexcept
    {
      return (static_cast<uint16_t>(status_bits) &
              static_cast<uint16_t>(
              WG14_RESULT_PREFIX(result_status_flag_have_value))) != 0;
    }
    constexpr bool have_error() const noexcept
    {
      return (static_cast<uint16_t>(status_bits) &
              static_cast<uint16_t>(
              WG14_RESULT_PREFIX(result_status_flag_have_error))) != 0;
    }
    constexpr bool have_exception() const noexcept
    {
      return (static_cast<uint16_t>(status_bits) &
              static_cast<uint16_t>(
              WG14_RESULT_PREFIX(result_status_flag_have_exception))) != 0;
    }
    constexpr bool have_lost_consistency() const noexcept
    {
      return (static_cast<uint16_t>(status_bits) &
              static_cast<uint16_t>(WG14_RESULT_PREFIX(
              result_status_flag_have_lost_consistency))) != 0;
    }
#endif
  };
  /*
     The status word is 4 bytes, aligned to 2, in every language mode. The size
     alone proves nothing: a union of an int's worth of bits is 4 bytes whether
     it is aligned to 2 or to 4, and the alignment is what decides where in a
     Result the status word sits (this is the check that would have caught the
     char/short/bool divergence). */
  WG14_RESULT_STATIC_ASSERT(
  (sizeof(union WG14_RESULT_PREFIX(result_flags)) == sizeof(uint32_t)),
  "result_flags is not the size it is supposed to be!")
  WG14_RESULT_STATIC_ASSERT(
  (__alignof(union WG14_RESULT_PREFIX(result_flags)) == __alignof(uint16_t)),
  "result_flags must be aligned like the two uint16_t it is made of!")
#if WG14_RESULT_BIG_ENDIAN
  /*
     The first two bytes of the word are the HIGH half on a big endian machine. */
  WG14_RESULT_STATIC_ASSERT(
  (offsetof(union WG14_RESULT_PREFIX(result_flags), spare_storage_value) == 0),
  "the spare storage is not in the first two bytes, where the high half is on a "
  "big endian machine!")
  WG14_RESULT_STATIC_ASSERT(
  (offsetof(union WG14_RESULT_PREFIX(result_flags), status_bits) == 2),
  "the status bits are not in the last two bytes, where the low half is on a big "
  "endian machine!")
#else
  WG14_RESULT_STATIC_ASSERT(
  (offsetof(union WG14_RESULT_PREFIX(result_flags), status_bits) == 0),
  "the status bits are not the low half of the status word!")
  WG14_RESULT_STATIC_ASSERT(
  (offsetof(union WG14_RESULT_PREFIX(result_flags), spare_storage_value) == 2),
  "the spare storage is not the high half of the status word!")
#endif

#if WG14_RESULT_SIZED_ENUMS_SUPPORTED
  /*
     The debugger-facing half of the same invariant. In C23 and C++ the flags
     enum has a fixed 16 bit underlying type, so it is two bytes and can be - and
     is - the declared type of the status word's low half. A build in which this
     fails is one where the enum is `int` sized, which would move the status word
     to align_up(sizeof(R), 4) and break the layout below. */
  WG14_RESULT_STATIC_ASSERT(
  (sizeof(enum WG14_RESULT_PREFIX(result_status_flags)) == sizeof(uint16_t)),
  "the flags enum is not 2 bytes, so a Result's status word cannot be typed "
  "with it")
  WG14_RESULT_STATIC_ASSERT(
  (__alignof(enum WG14_RESULT_PREFIX(result_status_flags)) ==
   __alignof(uint16_t)),
  "the flags enum is not aligned like a uint16_t!")
#endif
#if !(defined(SWIG) && __cplusplus)
#if !WG14_RESULT_BIG_ENDIAN
  /*
     `status` aliases `status_bits`, the low half. In the big endian order it is
     reached through a padding half, so it is not a union member there and its
     offset cannot be taken; the numeric check in test/layout_test.c proves the
     aliasing on both endiannesses, which is the property that matters. */
  WG14_RESULT_STATIC_ASSERT(
  (offsetof(union WG14_RESULT_PREFIX(result_flags), status) == 0),
  "the flags member is not the low half of the status word!")
#endif
  WG14_RESULT_STATIC_ASSERT(
  (sizeof(((union WG14_RESULT_PREFIX(result_flags) *) WG14_RESULT_NULLPTR)
           ->status) == sizeof(uint16_t)),
  "the flags member is not 2 bytes, so the status word is not 2 byte "
  "aligned")

  /*! \brief The low half of a Result's status word, as the flags enum. */
  /*
     
     In C23 and C++ `_flags_.status` already has the enum type, so this is just a
     second spelling. In C11 and C17 it is the only way to read the flags in
     named terms, because there the same two bytes are stored as `unsigned
     short` (an enum member would be four bytes, and would move the status word -
     see the top of this file). All four modes read the same two bytes. */
  WG14_RESULT_EXTERN_INLINE enum WG14_RESULT_PREFIX(result_status_flags)
  WG14_RESULT_PREFIX(result_flags_get_status)(
  const union WG14_RESULT_PREFIX(result_flags) *flags)
  {
    return (enum WG14_RESULT_PREFIX(result_status_flags)) flags->status;
  }
#endif

  /*
     The invariant every declared Result must hold, in every language mode, and
     which is what makes a WG14 Result interchangeable with `status_result<T>`:
     
       value    at offset 0
       _flags_  at align_up(sizeof(T), 2)   - status_result<T>'s status word
       error    at align_up(_flags_ + 4, alignof(status_code_system))
       alignof == max(alignof(T), alignof(flags), alignof(error))
       sizeof  == the status code followed by nothing but alignment padding
     
     The status word is at align_up(sizeof(T), 2) because both implementations
     keep it as two `uint16_t`; the status code after it is *not* at
     _flags_ + 4, because it is a `status_code_system` (two pointers) whose own
     alignment can require a gap - status_result<T> has exactly the same gap.
     
     Written as the rule rather than as literal numbers because reproduction from
     a formula is not proof: test/tests/cabi-layout.cpp in Outcome reads the
     offsets off `status_result<R>` itself for bool, char, short, int, long, long
     long, intptr_t, float, double, void pointer, 16, 24 and 512 byte structs, an
     alignas(16) struct and a non-trivially-copyable struct, and asserts them
     against this header's Result in every C++ mode; test/layout_test.c here
     asserts the rule above in C11, C17 and C23. */
#define WG14_RESULT_ASSERT_TAIL(sname)                                         \
  WG14_RESULT_STATIC_ASSERT(                                                   \
  (sizeof(struct WG14_RESULT_PREFIX(sname)) %                                  \
   __alignof(struct WG14_RESULT_PREFIX(sname)) == 0),                          \
  "the Result's size is not a multiple of its alignment")                    \
  WG14_RESULT_STATIC_ASSERT(                                                   \
  (sizeof(struct WG14_RESULT_PREFIX(sname)) >=                                 \
   offsetof(struct WG14_RESULT_PREFIX(sname), error) +                         \
   sizeof(WG14_RESULT_PREFIX(status_code_system))),                            \
  "the Result is smaller than its status word and status code")              \
  WG14_RESULT_STATIC_ASSERT(                                                   \
  (sizeof(struct WG14_RESULT_PREFIX(sname)) <                                  \
   offsetof(struct WG14_RESULT_PREFIX(sname), error) +                         \
   sizeof(WG14_RESULT_PREFIX(status_code_system)) +                            \
   __alignof(struct WG14_RESULT_PREFIX(sname))),                               \
  "the Result is padded beyond what its alignment requires")

#define WG14_RESULT_ASSERT_LAYOUT(T, sname)                                    \
  WG14_RESULT_STATIC_ASSERT(                                                   \
  (offsetof(struct WG14_RESULT_PREFIX(sname), value) == 0),                    \
  "the value is not at the start of the Result")                             \
  WG14_RESULT_STATIC_ASSERT(                                                   \
  (offsetof(struct WG14_RESULT_PREFIX(sname), _flags_) ==                      \
   (sizeof(T) + 1) / 2 * 2),                                                   \
  "the status word is not where status_result<T> keeps it")                  \
  WG14_RESULT_STATIC_ASSERT(                                                   \
  (offsetof(struct WG14_RESULT_PREFIX(sname), error) ==                        \
   (offsetof(struct WG14_RESULT_PREFIX(sname), _flags_) +                      \
    sizeof(union WG14_RESULT_PREFIX(result_flags)) +                           \
    __alignof(WG14_RESULT_PREFIX(status_code_system)) - 1) /                   \
       __alignof(WG14_RESULT_PREFIX(status_code_system)) *                     \
       __alignof(WG14_RESULT_PREFIX(status_code_system))),                     \
  "the status code is not where status_result<T> keeps it")                  \
  WG14_RESULT_STATIC_ASSERT(                                                   \
  (__alignof(struct WG14_RESULT_PREFIX(sname)) ==                              \
   (__alignof(T) > __alignof(WG14_RESULT_PREFIX(status_code_system))           \
    ? __alignof(T)                                                             \
    : __alignof(WG14_RESULT_PREFIX(status_code_system)))),                     \
  "the Result is not aligned like its most aligned member")                  \
  WG14_RESULT_ASSERT_TAIL(sname)
#ifdef _MSC_VER
#pragma warning(pop)
#endif
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif

  struct WG14_RESULT_PREFIX(result_with_void_s)
  {
    /* The void Result still has a value member, of the same size as the empty
       value storage `status_result<void>` uses: one byte. Without it the status
       word would sit at offset 0 here and at offset 2 in the C++ ABI, and the two
       would be the same size without being the same bytes. */
    char _value_[1];
    union WG14_RESULT_PREFIX(result_flags) _flags_;
    WG14_RESULT_PREFIX(status_code_system) error;
  };
  /*! \brief A Result for type `void`. */
#if defined(__cplusplus) && !defined(WG14_RESULT_DISABLE_CXX_EXTENSIONS)
  struct WG14_RESULT_PREFIX(result_with_void)
      : wg14_result::wg14_result_special_member_functions<WG14_RESULT_PREFIX(
        result_with_void_s)>
  {
    using wg14_result::wg14_result_special_member_functions<WG14_RESULT_PREFIX(
    result_with_void_s)>::wg14_result_special_member_functions;

    WG14_RESULT_CXX_CONSTEXPR20
    WG14_RESULT_PREFIX(result_with_void)() = default;
    WG14_RESULT_PREFIX(result_with_void)
    (const WG14_RESULT_PREFIX(result_with_void) &) = default;
    WG14_RESULT_PREFIX(result_with_void)
    (WG14_RESULT_PREFIX(result_with_void) &&) = default;
    WG14_RESULT_PREFIX(result_with_void) &
    operator=(const WG14_RESULT_PREFIX(result_with_void) &) = default;
    WG14_RESULT_PREFIX(result_with_void) &
    operator=(WG14_RESULT_PREFIX(result_with_void) &&) = default;
    ~WG14_RESULT_PREFIX(result_with_void)() = default;
  };
#else
typedef struct WG14_RESULT_PREFIX(result_with_void_s)
WG14_RESULT_PREFIX(result_with_void);
#endif
  /*
     The same invariant for the void Result: its one byte value storage at offset
     0, the status word at align_up(sizeof(value), 2) - offset 2, as
     `status_result<void>` has it - then alignment padding if the status code needs
     it, then the status code. Checked in every language mode, including C++, so
     that the C++ view of a void Result is the same bytes as the C one and as
     `status_result<void>`. */
  WG14_RESULT_STATIC_ASSERT(
  (offsetof(struct WG14_RESULT_PREFIX(result_with_void_s), _flags_) ==
   ((sizeof(((struct WG14_RESULT_PREFIX(result_with_void_s) *) 0)->_value_) + 1) /
    2 * 2)),
  "the status word of the void Result is not where status_result<void> keeps it!")
  WG14_RESULT_STATIC_ASSERT(
  (offsetof(struct WG14_RESULT_PREFIX(result_with_void_s), error) ==
   ((offsetof(struct WG14_RESULT_PREFIX(result_with_void_s), _flags_) +
     sizeof(union WG14_RESULT_PREFIX(result_flags)) +
     __alignof(WG14_RESULT_PREFIX(status_code_system)) - 1) /
    __alignof(WG14_RESULT_PREFIX(status_code_system)) *
    __alignof(WG14_RESULT_PREFIX(status_code_system)))),
  "the status code of the void Result is not where status_result<void> keeps "
  "it!")
  WG14_RESULT_STATIC_ASSERT(
  (sizeof(WG14_RESULT_PREFIX(result_with_void)) ==
   sizeof(struct WG14_RESULT_PREFIX(result_with_void_s))),
  "the C++ wrapper of the void Result has members outside its storage!")
  WG14_RESULT_ASSERT_TAIL(result_with_void_s)

  /*! \brief Makes a successful Result with type `void`. */
  WG14_RESULT_EXTERN_INLINE WG14_RESULT_PREFIX(result_with_void)
  WG14_RESULT_PREFIX(result_void_make_success)(void)
  {
    struct WG14_RESULT_PREFIX(result_with_void_s) ret1;
    WG14_RESULT_NOT_CXX_MEMSET(&ret1, 0, sizeof(ret1));
    ret1._flags_.status = WG14_RESULT_PREFIX(result_status_flag_have_value);
    ret1.error = WG14_RESULT_PREFIX(status_code_system_empty);
    const WG14_RESULT_PREFIX(result_with_void) ret2 =
    WG14_RESULT_STATUS_CODE_SPECIAL_MEMBER_FUNCTIONS_INITIALISER(ret1);
    return ret2;
  }
  /*! \brief Makes an unsuccessful Result with type `void`. */
  WG14_RESULT_EXTERN_INLINE WG14_RESULT_PREFIX(result_with_void)
  WG14_RESULT_PREFIX(result_void_make_failure)(
  WG14_RESULT_PREFIX(status_code_system) err)
  {
    struct WG14_RESULT_PREFIX(result_with_void_s) ret1;
    WG14_RESULT_NOT_CXX_MEMSET(&ret1, 0, sizeof(ret1));
    ret1._flags_.status = WG14_RESULT_PREFIX(result_status_flag_have_error);
    ret1.error = err;
    const WG14_RESULT_PREFIX(result_with_void) ret2 =
    WG14_RESULT_STATUS_CODE_SPECIAL_MEMBER_FUNCTIONS_INITIALISER(ret1);
    return ret2;
  }

  /*
     C++ gives a Result's value member a default member initialiser, so a
     default constructed `result_with_x_s` has a zeroed value - except at C++11,
     where a default member initialiser stops the struct being an aggregate (it
     becomes one again in C++14), and these storage structs are then initialised
     with braces in WG14_RESULT_DECLARE_IMPL2 and by the C spelling of the
     headers. The value is always passed explicitly by make_success() and is not
     read out of a failure, so C++11 loses only the zeroing. */
#if WG14_RESULT_CXX_VERSION >= 201103L && WG14_RESULT_CXX_VERSION < 201402L
#define WG14_RESULT_CXX_VALUE_NSDMI
#else
#define WG14_RESULT_CXX_VALUE_NSDMI {}
#endif

#define WG14_RESULT_DECLARE_IMPL1(T, name, ...)                                \
  struct WG14_RESULT_PREFIX(result_with_##name##_s)                            \
  {                                                                            \
    T value __VA_ARGS__;                                                       \
    union WG14_RESULT_PREFIX(result_flags) _flags_;                            \
    WG14_RESULT_PREFIX(status_code_system) error;                              \
  };                                                                           \
  WG14_RESULT_ASSERT_LAYOUT(T, result_with_##name##_s)
#define WG14_RESULT_DECLARE_IMPL2(T, name)                                     \
  WG14_RESULT_EXTERN_INLINE WG14_RESULT_PREFIX(result_with_##name)             \
  WG14_RESULT_PREFIX(result_##name##_make_success)(T val)                      \
  {                                                                            \
    struct WG14_RESULT_PREFIX(result_with_##name##_s) ret1;                    \
    /* Field by field rather than a positional brace initialiser: the two     \
    halves of the flags word are declared in endianness order, so a brace      \
    initialiser would set whichever member happens to come first instead of    \
    the flags. `status` always aliases `status_bits`. */                       \
    WG14_RESULT_NOT_CXX_MEMSET(&ret1, 0, sizeof(ret1));                        \
    ret1.value = val;                                                          \
    ret1._flags_.status = WG14_RESULT_PREFIX(result_status_flag_have_value);   \
    ret1.error = WG14_RESULT_PREFIX(status_code_system_empty);                 \
    WG14_RESULT_PREFIX(result_with_##name)                                     \
    ret2 = WG14_RESULT_STATUS_CODE_SPECIAL_MEMBER_FUNCTIONS_INITIALISER(ret1); \
    return ret2;                                                               \
  }                                                                            \
  WG14_RESULT_EXTERN_INLINE WG14_RESULT_PREFIX(result_with_##name)             \
  WG14_RESULT_PREFIX(result_##name##_make_failure)(                            \
  WG14_RESULT_PREFIX(status_code_system) err)                                  \
  {                                                                            \
    WG14_RESULT_PREFIX(result_with_##name) ret;                                \
    WG14_RESULT_NOT_CXX_MEMSET(&ret, 0, sizeof(ret));                          \
    ret.error = err;                                                           \
    ret._flags_.status = WG14_RESULT_PREFIX(result_status_flag_have_error);    \
    return ret;                                                                \
  }

#if defined(__cplusplus) && !defined(WG14_RESULT_DISABLE_CXX_EXTENSIONS)
/*! \brief Declare a Result named `name` with successful type `T` */
#define WG14_RESULT_DECLARE(T, name)                                           \
  WG14_RESULT_DECLARE_IMPL1(T, name, WG14_RESULT_CXX_VALUE_NSDMI)              \
  struct WG14_RESULT_PREFIX(result_with_##name)                                \
      : wg14_result::wg14_result_special_member_functions<WG14_RESULT_PREFIX(  \
        result_with_##name##_s)>                                               \
  {                                                                            \
    using _base =                                                              \
    wg14_result::wg14_result_special_member_functions<WG14_RESULT_PREFIX(      \
    result_with_##name##_s)>;                                                  \
    using _base::_base;                                                        \
                                                                               \
    using value_type = T;                                                      \
    using error_type = WG14_RESULT_PREFIX(status_code_system);                 \
                                                                               \
    WG14_RESULT_CXX_CONSTEXPR20                                                \
    WG14_RESULT_PREFIX(result_with_##name)() = default;                        \
    WG14_RESULT_PREFIX(result_with_##name)                                     \
    (const WG14_RESULT_PREFIX(result_with_##name) &) = default;                \
    WG14_RESULT_PREFIX(result_with_##name)                                     \
    (WG14_RESULT_PREFIX(result_with_##name) &&) = default;                     \
    WG14_RESULT_PREFIX(result_with_##name) &                                   \
    operator=(const WG14_RESULT_PREFIX(result_with_##name) &) = default;       \
    WG14_RESULT_PREFIX(result_with_##name) &                                   \
    operator=(WG14_RESULT_PREFIX(result_with_##name) &&) = default;            \
    ~WG14_RESULT_PREFIX(result_with_##name)() = default;                       \
  };                                                                           \
  WG14_RESULT_DECLARE_IMPL2(T, name)
#else
#define WG14_RESULT_DECLARE_IMPL(T, name)                                      \
  WG14_RESULT_DECLARE_IMPL1(T, name, )                                         \
  typedef struct WG14_RESULT_PREFIX(result_with_##name##_s)                    \
  WG14_RESULT_PREFIX(result_with_##name);                                      \
  WG14_RESULT_DECLARE_IMPL2(T, name)
/*! \brief Declare a Result named `name` with successful type `T` */
#define WG14_RESULT_DECLARE(T, name) WG14_RESULT_DECLARE_IMPL(T, name)
#endif

#define WG14_RESULT_IMPL(name) WG14_RESULT_PREFIX(result_with_##name)
/*! \brief The type of a Result named `name` previously declared. */
#define WG14_RESULT(name) WG14_RESULT_IMPL(name)
#define WG14_RESULT_MAKE_SUCCESS_IMPL(name, ...)                               \
  WG14_RESULT_PREFIX(result_##name##_make_success)(__VA_ARGS__)
/*! \brief Make a successful named Result previously declared. */
#define WG14_RESULT_MAKE_SUCCESS(name, ...)                                    \
  WG14_RESULT_MAKE_SUCCESS_IMPL(name, __VA_ARGS__)
#define WG14_RESULT_MAKE_FAILURE_IMPL(name, ...)                               \
  WG14_RESULT_PREFIX(result_##name##_make_failure)(__VA_ARGS__)
/*! \brief Make a failed named Result previously declared. */
#define WG14_RESULT_MAKE_FAILURE(name, ...)                                    \
  WG14_RESULT_MAKE_FAILURE_IMPL(name, __VA_ARGS__)
/*! \brief True if a Result is successful (and contains a value if not `void`) */
#define WG14_RESULT_HAS_VALUE(...)                                             \
  ((__VA_ARGS__)._flags_.status_bits &                                         \
   WG14_RESULT_PREFIX(result_status_flag_have_value))
/*! \brief True if a Result is a failure (and contains an error) */
#define WG14_RESULT_HAS_ERROR(...)                                             \
  ((__VA_ARGS__)._flags_.status_bits &                                         \
   WG14_RESULT_PREFIX(result_status_flag_have_error))

#ifdef __cplusplus
}
#if defined(__cplusplus) && !defined(WG14_RESULT_DISABLE_CXX_EXTENSIONS)
namespace wg14_result
{
  template <class T> struct WG14_RESULT_PREFIX(result_storage)
  {
    T value{};
    union WG14_RESULT_PREFIX(result_flags) _flags_;
    WG14_RESULT_PREFIX(status_code_system) error;

    using value_type = T;
    using error_type = WG14_RESULT_PREFIX(status_code_system);
  };
  template <> struct WG14_RESULT_PREFIX(result_storage)<void>
  {
    union WG14_RESULT_PREFIX(result_flags) _flags_;
    WG14_RESULT_PREFIX(status_code_system) error;

    using value_type = void;
    using error_type = WG14_RESULT_PREFIX(status_code_system);
  };
  template <class T>
  struct WG14_RESULT_PREFIX(result)
      : wg14_result::wg14_result_special_member_functions <
      WG14_RESULT_PREFIX(result_storage) < T >>
  {
    using wg14_result::wg14_result_special_member_functions <
    WG14_RESULT_PREFIX(result_storage) < T >>
    ::wg14_result_special_member_functions;

    WG14_RESULT_CXX_CONSTEXPR20
    WG14_RESULT_PREFIX(result)() = default;
    constexpr WG14_RESULT_PREFIX(result)(const WG14_RESULT_PREFIX(result) &) =
    default;
    constexpr WG14_RESULT_PREFIX(result)(WG14_RESULT_PREFIX(result) &&) =
    default;
    WG14_RESULT_PREFIX(result) &
    operator=(const WG14_RESULT_PREFIX(result) &) = default;
    WG14_RESULT_PREFIX(result) &
    operator=(WG14_RESULT_PREFIX(result) &&) = default;
    ~WG14_RESULT_PREFIX(result)() = default;
  };
} /* namespace wg14_result */
#endif
#endif

#endif
