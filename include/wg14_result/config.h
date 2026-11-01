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


#ifndef WG14_RESULT_CONFIG_H
#define WG14_RESULT_CONFIG_H

/* The C standard in use, as a number, with MSVC's C23 announcement discounted.
   MSVC accepts `C_STANDARD 23`, reports __STDC_VERSION__ as 202311L, and then has
   neither `nullptr`, `constexpr`, `typeof` nor a fixed enum underlying type in C
   (it reports "error C2065: 'nullptr': undeclared identifier", "expected '(' to
   follow 'constexpr'" and "syntax error: ':'" against the C23 spelling). Every
   C23 test in these headers therefore goes through this value rather than through
   __STDC_VERSION__ directly, and MSVC takes the C17 arms. */
#ifndef WG14_RESULT_C_VERSION
#if defined(__STDC_VERSION__)
#if __STDC_VERSION__ >= 202311L && defined(_MSC_VER)
#define WG14_RESULT_C_VERSION 201710L
#else
#define WG14_RESULT_C_VERSION __STDC_VERSION__
#endif
#else
#define WG14_RESULT_C_VERSION 0
#endif
#endif


#if defined(__cplusplus)
#if __cplusplus < 201103L && _MSVC_LANG < 201103L &&                           \
!defined(WG14_RESULT_DISABLE_CXX_EXTENSIONS) && !defined(SWIG)
#error                                                                         \
"If being used from C++, a minimum of C++ 11 is required. If you really need to use C++ 98, define WG14_RESULT_DISABLE_CXX_EXTENSIONS to make this a pure C library."
#endif
#include <version>  /* some toolchains need this to set the version macros correctly */
#endif

/* Can this language mode spell a fixed enum underlying type, i.e. `enum X :
   unsigned short`? C++11 and C23 can; C++98, C89, C11 and C17 cannot, and there
   the spelling is a hard error rather than a warning.

   `__cplusplus` alone is not enough to answer this: MSVC reports 199711L whatever
   /std: option is in use, unless /Zc:__cplusplus is passed - and nothing in this
   project passes it. `_MSVC_LANG` always reports the standard actually in use, so
   it is consulted as well. Without it every default MSVC build silently took the
   portable branch and lost the enum the debugger is supposed to show, even at
   /std:c++17.

   The C test is the standard's own value, 202311L. This is a reference
   implementation for a proposal, so it asks the compiler what the standard says
   it is rather than what it can parse: gcc 14 accepts the C23 spelling under
   -std=c23 while reporting __STDC_VERSION__ as 202000L, so it takes the portable
   branch until it reports C23 honestly. That is the conformant answer, and the
   way to opt in on such a compiler is to define this macro to 1.

   Define this to 0 to force the portable storage form, or to 1 to force the
   sized enum in a mode that cannot spell it (gcc and clang accept it there as an
   extension and warn rather than error, unless -pedantic-errors is in force). */
#ifndef WG14_RESULT_SIZED_ENUMS_SUPPORTED
#if(defined(__cplusplus) && __cplusplus >= 201103L) ||                         \
(defined(_MSVC_LANG) && _MSVC_LANG >= 201103L) ||                             \
(defined(__STDC_VERSION__) && WG14_RESULT_C_VERSION >= 202311L)
#define WG14_RESULT_SIZED_ENUMS_SUPPORTED 1
#else
#define WG14_RESULT_SIZED_ENUMS_SUPPORTED 0
#endif
#endif

/* The same question for a compile time assertion. C++ has the `static_assert`
   keyword from C++11; C has the `_Static_assert` keyword from C11 (the plain
   `static_assert` spelling is only a macro that C11 asks <assert.h> to provide,
   so it is not a keyword there and these headers never use it - they do not
   include <assert.h>). Neither spelling exists in C++98 or C89, where the layout
   checks in result.h compile away and the invariant rests on construction and on
   the C tests. Derived for the same reason and in the same way as above.
   Which way round the bytes of a multi byte integer go. The Result's status word
   is a 32 bit word whose low 16 bits are the flags and whose high 16 bits are
   spare storage, and a union of two uint16_t only has that numeric layout on a
   little endian machine: on a big endian one the first two bytes are the high
   half of the word, so the members are declared the other way round (see
   result.h). Fail here rather than lay a Result out backwards on a machine
   nobody checked; the runtime check in test/layout_test.c pins the numeric
   layout either way. */
#ifndef WG14_RESULT_BIG_ENDIAN
#if defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__)
#define WG14_RESULT_BIG_ENDIAN (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#elif defined(__BYTE_ORDER) && defined(__BIG_ENDIAN)
#define WG14_RESULT_BIG_ENDIAN (__BYTE_ORDER == __BIG_ENDIAN)
#elif defined(_MSC_VER) || defined(_WIN32)
/* Every Windows target is little endian, and MSVC defines none of the above. */
#define WG14_RESULT_BIG_ENDIAN 0
#elif defined(SWIG)
/* SWIG parses the headers with its own preprocessor, which defines none of the
   above. The wrapper it generates re-includes these headers, so the compiler
   that builds it decides the endianness then; what SWIG sees only has to let the
   parse through. */
#define WG14_RESULT_BIG_ENDIAN 0
#else
#error                                                                         \
"cannot determine endianness: define WG14_RESULT_BIG_ENDIAN to 0 (little) or 1 (big)"
#endif
#endif

#ifndef WG14_RESULT_STATIC_ASSERT_SUPPORTED
#if(defined(__cplusplus) && __cplusplus >= 201103L) ||                         \
(defined(_MSVC_LANG) && _MSVC_LANG >= 201103L) ||                             \
(defined(__STDC_VERSION__) && WG14_RESULT_C_VERSION >= 201112L)
#define WG14_RESULT_STATIC_ASSERT_SUPPORTED 1
#else
#define WG14_RESULT_STATIC_ASSERT_SUPPORTED 0
#endif
#endif

/*! \brief A compile time assertion, in whichever spelling this language has.
    Carries its own semicolon, as assert() does, so that a use is written without
    one and the modes which cannot express an assertion expand to nothing at
    all. */
/*
   One spelling for the layout checks in result.h, which are written once for
   every language family (and so that a predicate containing `offsetof(struct x,
   y)` - which has a comma in it - stays one macro argument, hence the extra
   bracketing at each use). In C++98 and C89/C90 it expands to nothing: those
   modes cannot express an assertion at file scope, so the layout invariant is
   covered there by construction (the same declarations with the same natural
   alignment, the same two byte `unsigned short` status storage) and by
   test/layout_test.c compiled as C11/C17/C23 against the same header, while
   test/header_smoke.c and test/header_smoke_cxx98.cpp prove the headers still
   parse in those modes. */
#ifndef WG14_RESULT_STATIC_ASSERT
#if WG14_RESULT_STATIC_ASSERT_SUPPORTED
#if defined(__cplusplus)
#define WG14_RESULT_STATIC_ASSERT(pred, msg) static_assert(pred, msg);
#else
#define WG14_RESULT_STATIC_ASSERT(pred, msg) _Static_assert(pred, msg);
#endif
#else
#define WG14_RESULT_STATIC_ASSERT(pred, msg)
#endif
#endif

/* The C++ standard in use, as a number: 199711L, 201103L, 201402L, ... and 0 when
   this is not C++ at all. MSVC reports 199711L in `__cplusplus` whatever /std:
   option is in use unless /Zc:__cplusplus is passed, and reports the real value in
   `_MSVC_LANG`, so that is consulted when `__cplusplus` is not truthful. */
#ifndef WG14_RESULT_CXX_VERSION
#if defined(__cplusplus) && __cplusplus >= 201103L
#define WG14_RESULT_CXX_VERSION __cplusplus
#elif defined(__cplusplus) && defined(_MSVC_LANG)
#define WG14_RESULT_CXX_VERSION _MSVC_LANG
#elif defined(__cplusplus)
#define WG14_RESULT_CXX_VERSION __cplusplus
#else
#define WG14_RESULT_CXX_VERSION 0
#endif
#endif

/* Whether this language mode has `constexpr` at all: C++11 and later, or C23. */
#ifndef WG14_RESULT_CONSTEXPR_AVAILABLE
#if (defined(__cplusplus) && WG14_RESULT_CXX_VERSION >= 201103L) ||            \
(defined(__STDC_VERSION__) && WG14_RESULT_C_VERSION >= 202311L)
#define WG14_RESULT_CONSTEXPR_AVAILABLE 1
#else
#define WG14_RESULT_CONSTEXPR_AVAILABLE 0
#endif
#endif

/* `constexpr` on a C++ constructor where the language has it, and nothing in
   C++98, where the same constructors are written in their plain C++98 spelling. */
#ifndef WG14_RESULT_CXX_CONSTEXPR
#if defined(__cplusplus) && WG14_RESULT_CONSTEXPR_AVAILABLE
#define WG14_RESULT_CXX_CONSTEXPR constexpr
#else
#define WG14_RESULT_CXX_CONSTEXPR
#endif
#endif

/* The same for a member a C++11 compiler cannot make constexpr: a constructor with
   a `std::initializer_list` parameter, because that type is not a literal type
   until C++14 (AppleClang rejects it outright, gcc tolerates it). */
#ifndef WG14_RESULT_CXX_CONSTEXPR14
#if defined(__cplusplus) && WG14_RESULT_CXX_VERSION >= 201402L
#define WG14_RESULT_CXX_CONSTEXPR14 constexpr
#else
#define WG14_RESULT_CXX_CONSTEXPR14
#endif
#endif

#if WG14_RESULT_ENABLE_HEADER_ONLY || WG14_RESULT_SOURCE
#if defined(__cplusplus)
#if !((defined(__cplusplus) && __cplusplus >= 202002L) ||               \
(defined(_MSVC_LANG) && _MSVC_LANG >= 202002L) ||                      \
(defined(_HAS_CXX20) && _HAS_CXX20))
#error                                                                         \
"If being compiled as C++, the source code requires a minimum of a C++ 20 compiler for decent C11 support."
#endif
#elif WG14_RESULT_C_VERSION < 201112L
#error                                                                         \
"The source code requires a minimum of a C11 compiler for atomics support."
#endif
#endif

#ifndef WG14_RESULT_PREFIX
#define WG14_RESULT_PREFIX(x) x
#endif

/* Whether the C23 keywords these headers use - `nullptr`, `constexpr`, `typeof` -
   are actually available. MSVC announces a C23 mode (C_STANDARD 23 is accepted and
   __STDC_VERSION__ is 202311L) but its C front end has neither keyword: it reports
   `error C2065: 'nullptr': undeclared identifier` and "expected '(' to follow
   'constexpr'". Asking for the keywords and being told yes is not the same as
   having them, so MSVC's answer is discounted and the portable arms are used
   there. */
#ifndef WG14_RESULT_C23_KEYWORDS
#if WG14_RESULT_C_VERSION >= 202311L
#define WG14_RESULT_C23_KEYWORDS 1
#else
#define WG14_RESULT_C23_KEYWORDS 0
#endif
#endif

#ifndef WG14_RESULT_NULLPTR
#if WG14_RESULT_C23_KEYWORDS
#define WG14_RESULT_NULLPTR nullptr
#else
#define WG14_RESULT_NULLPTR NULL
#endif
#endif

#ifndef WG14_RESULT_CONSTEXPR
#if WG14_RESULT_C23_KEYWORDS && (!defined(__clang__) || __clang_major >= 19)
#define WG14_RESULT_CONSTEXPR constexpr
#else
#define WG14_RESULT_CONSTEXPR
#endif
#endif

#ifndef WG14_RESULT_TYPEOF
#if defined(__cplusplus) && WG14_RESULT_CXX_VERSION >= 201103L
/* C++ spells it decltype, and MSVC has neither typeof nor __typeof__ in any mode -
   the tests are compiled as C++ there, which is how this was found. decltype of a
   dereference is a reference where typeof of it is a value, so the reference is
   removed: every use is a typedef for declaring pointers to it. */
#include <type_traits>
#define WG14_RESULT_TYPEOF(...)                                                \
  typename std::remove_reference<decltype(__VA_ARGS__)>::type
#elif WG14_RESULT_C23_KEYWORDS
#define WG14_RESULT_TYPEOF(...) typeof(__VA_ARGS__)
#else
#define WG14_RESULT_TYPEOF(...) __typeof__(__VA_ARGS__)
#endif
#endif

#ifndef WG14_RESULT_TYPEOF_UNQUAL
#if WG14_RESULT_C_VERSION >= 202311L
#define WG14_RESULT_TYPEOF_UNQUAL(...) typeof_unqual(__VA_ARGS__)
#elif(defined(__GNUC__) && !defined(__clang__) && __GNUC__ < 14) ||            \
(defined(__clang__) && __clang_major__ < 19)
/* Not much else we can do here */
#define WG14_RESULT_TYPEOF_UNQUAL(...) __typeof__(__VA_ARGS__)
#elif defined(__cplusplus) && __cplusplus < 201103L
/* C++98 has no <type_traits> (and libstdc++'s errors out if asked), and it has
   no decltype either, so __typeof__ is the only spelling available. Only the
   C-only part of these headers is usable from C++98 - config.h requires
   WG14_RESULT_DISABLE_CXX_EXTENSIONS there - but the macro still has to exist
   and parse. */
#define WG14_RESULT_TYPEOF_UNQUAL(...) __typeof__(__VA_ARGS__)
#elif defined(__cplusplus)
/* C++ compilers don't appear to provide __typeof_unqual__ if the language
   standard is old enough */
#include <type_traits>
#define WG14_RESULT_TYPEOF_UNQUAL(...)                                         \
  std::remove_cv<std::remove_reference<decltype(__VA_ARGS__)>::type>::type
#else
#define WG14_RESULT_TYPEOF_UNQUAL(...) __typeof_unqual__(__VA_ARGS__)
#endif
#endif

#ifndef WG14_RESULT_CONSTEXPR_OR_CONST
#if WG14_RESULT_C_VERSION >= 202311L && (!defined(__clang__) || __clang_major >= 19)
#define WG14_RESULT_CONSTEXPR_OR_CONST constexpr
#else
#define WG14_RESULT_CONSTEXPR_OR_CONST const
#endif
#endif

#ifndef WG14_RESULT_DEFAULT_VISIBILITY
#ifdef _WIN32
#define WG14_RESULT_DEFAULT_VISIBILITY
#else
#define WG14_RESULT_DEFAULT_VISIBILITY __attribute__((visibility("default")))
#endif
#endif

#ifndef WG14_RESULT_EXTERN
#if WG14_RESULT_SOURCE
#ifdef _WIN32
#define WG14_RESULT_EXTERN extern __declspec(dllexport)
#else
#define WG14_RESULT_EXTERN extern __attribute__((visibility("default")))
#endif
#elif WG14_RESULT_ENABLE_HEADER_ONLY
#define WG14_RESULT_EXTERN extern inline
#else
#define WG14_RESULT_EXTERN extern
#endif
#endif

#ifndef WG14_RESULT_PURE_INLINE
#if WG14_RESULT_C_VERSION >= 201112L || defined(SWIG)
#define WG14_RESULT_PURE_INLINE static inline
#else
#define WG14_RESULT_PURE_INLINE static __inline
#endif
#endif

#ifndef WG14_RESULT_EXTERN_INLINE
#if WG14_RESULT_SOURCE_INLINES
/* We need inlined functions to export if building the library */
#define WG14_RESULT_EXTERN_INLINE WG14_RESULT_EXTERN
#else
/* Other, pure inline */
#define WG14_RESULT_EXTERN_INLINE WG14_RESULT_PURE_INLINE
#endif
#endif


#ifndef WG14_RESULT_SINGLETON
#if defined(__GNUC__) || defined(__clang__)
#define WG14_RESULT_SINGLETON __attribute__((const))
#else
#define WG14_RESULT_SINGLETON
#endif
#endif

#ifndef WG14_RESULT_STATIC_ASSERT_WITHIN_EXPR
#if defined(__cplusplus)
#define WG14_RESULT_STATIC_ASSERT_WITHIN_EXPR(pred, msg, ...)                  \
  ((void) [] { static_assert((pred), msg); }, (__VA_ARGS__))
#elif WG14_RESULT_C_VERSION >= 202311L
#define WG14_RESULT_STATIC_ASSERT_WITHIN_EXPR(pred, msg, ...)                  \
  ((__VA_ARGS__) + 0 * sizeof(struct {                                         \
                     static_assert((pred), msg);                               \
                     int x;                                                    \
                   }))
#else
#define WG14_RESULT_STATIC_ASSERT_WITHIN_EXPR(pred, msg, ...)                  \
  ((__VA_ARGS__) + 0 * sizeof(struct {                                         \
                     _Static_assert((pred), msg);                              \
                     int x;                                                    \
                   }))
#endif
#endif


#ifndef WG14_RESULT_STDERR_PRINTF
#include <stdio.h>
#define WG14_RESULT_STDERR_PRINTF(...) fprintf(stderr, __VA_ARGS__)
#endif

#ifndef WG14_RESULT_ABORTF
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
WG14_RESULT_EXTERN_INLINE unsigned
#if !defined(_MSC_VER) && !defined(SWIG)
__attribute__((noreturn))
#endif
WG14_RESULT_PREFIX(abortf_impl)(const char *msg, ...)
{
  va_list args;
  va_start(args, msg);
  vfprintf(stderr, msg, args);
  abort();
#ifndef _MSC_VER
  va_end(args);
#endif
}
#if defined(_MSC_VER) && !defined(__clang__)
#define WG14_RESULT_ABORTF(X, ...)                                             \
  WG14_RESULT_PREFIX(abortf_impl)("FATAL: " X "\n", ##__VA_ARGS__)
#elif WG14_RESULT_C_VERSION >= 202311L && !defined(__clang__)
#define WG14_RESULT_ABORTF(X, ...)                                             \
  WG14_RESULT_PREFIX(abortf_impl)("FATAL: " X "\n" __VA_OPT(, ) __VA_ARGS__)
#else
#define WG14_RESULT_ABORTF_HAS_COMMA(...)                                      \
  WG14_RESULT_ABORTF_HAS_COMMA_16__(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
                                    1, 1, 1, 1, 0, 0)
#define WG14_RESULT_ABORTF_HAS_COMMA_16__(                                     \
_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, ...)     \
  _15
#define WG14_RESULT_ABORTF_CAT_(X, Y) X##Y
#define WG14_RESULT_ABORTF_CAT(X, Y) WG14_RESULT_ABORTF_CAT_(X, Y)
#define WG14_RESULT_ABORTF_INVOKE0(X)                                          \
  WG14_RESULT_PREFIX(abortf_impl)("FATAL: " X "\n")
#define WG14_RESULT_ABORTF_INVOKE1(X, ...)                                     \
  WG14_RESULT_PREFIX(abortf_impl)("FATAL: " X "\n", __VA_ARGS__)

#define WG14_RESULT_ABORTF(...)                                                \
  WG14_RESULT_ABORTF_CAT(WG14_RESULT_ABORTF_INVOKE,                            \
                         WG14_RESULT_ABORTF_HAS_COMMA(__VA_ARGS__))            \
  (__VA_ARGS__)
#endif
#endif

#ifndef WG14_RESULT_CXX_CONSTEXPR20
#if(defined(__cplusplus) && __cplusplus >= 202002L) ||                  \
(defined(_MSVC_LANG) && _MSVC_LANG >= 202002L) ||                      \
(defined(_HAS_CXX20) && _HAS_CXX20)
#define WG14_RESULT_CXX_CONSTEXPR20 constexpr
#else
#define WG14_RESULT_CXX_CONSTEXPR20
#endif
#endif

#ifndef WG14_RESULT_NOT_CXX_MEMSET
#if defined(__cplusplus) && !defined(WG14_RESULT_DISABLE_CXX_EXTENSIONS)
#define WG14_RESULT_NOT_CXX_MEMSET(mem, val, len)
#else
#define WG14_RESULT_NOT_CXX_MEMSET(mem, val, len) memset((mem), (val), (len))
#endif
#endif

#ifdef __cplusplus
extern "C"
{
#endif


#ifdef __cplusplus
}
#endif

#endif
