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

#if WG14_RESULT_ENABLE_HEADER_ONLY || WG14_RESULT_SOURCE
#if defined(__cplusplus)
#if __cplusplus < 202000L && !_HAS_CXX20
#include <version>  // some toolchains need this to set the version macros correctly
#endif
#if __cplusplus < 202000L && !_HAS_CXX20
#error                                                                         \
"If being compiled as C++, the source code requires a minimum of a C++ 20 compiler for decent C11 support."
#endif
#elif __STDC_VERSION__ < 201100L
#error                                                                         \
"The source code requires a minimum of a C11 compiler for atomics support."
#endif
#endif

#ifndef WG14_RESULT_PREFIX
#define WG14_RESULT_PREFIX(x) x
#endif

#ifndef WG14_RESULT_NULLPTR
#if __STDC_VERSION__ >= 202300L
#define WG14_RESULT_NULLPTR nullptr
#else
#define WG14_RESULT_NULLPTR NULL
#endif
#endif

#ifndef WG14_RESULT_CONSTEXPR
#if __STDC_VERSION__ >= 202300L && (!defined(__clang__) || __clang_major >= 19)
#define WG14_RESULT_CONSTEXPR constexpr
#else
#define WG14_RESULT_CONSTEXPR
#endif
#endif

#ifndef WG14_RESULT_INLINE
#if __STDC_VERSION__ >= 201100L
#define WG14_RESULT_INLINE static inline
#else
#define WG14_RESULT_INLINE static
#endif
#endif

#ifndef WG14_RESULT_TYPEOF
#if __STDC_VERSION__ >= 202300L
#define WG14_RESULT_TYPEOF(...) typeof(__VA_ARGS__)
#else
#define WG14_RESULT_TYPEOF(...) __typeof__(__VA_ARGS__)
#endif
#endif

#ifndef WG14_RESULT_TYPEOF_UNQUAL
#if __STDC_VERSION__ >= 202300L
#define WG14_RESULT_TYPEOF_UNQUAL(...) typeof_unqual(__VA_ARGS__)
#elif(defined(__GNUC__) && !defined(__clang__) && __GNUC__ < 14) ||            \
(defined(__clang__) && __clang_major__ < 19)
// Not much else we can do here
#define WG14_RESULT_TYPEOF_UNQUAL(...) __typeof__(__VA_ARGS__)
#elif defined(_MSC_VER) && defined(__cplusplus)
// MSVC doesn't appear to provide __typeof_unqual__ if we are in C++
#include <type_traits>
#define WG14_RESULT_TYPEOF_UNQUAL(...)                                         \
  std::remove_cv<decltype(__VA_ARGS__)>::type
#else
#define WG14_RESULT_TYPEOF_UNQUAL(...) __typeof_unqual__(__VA_ARGS__)
#endif
#endif

#ifndef WG14_RESULT_CONSTEXPR_OR_CONST
#if __STDC_VERSION__ >= 202300L && (!defined(__clang__) || __clang_major >= 19)
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
#elif __STDC_VERSION__ >= 202300L
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
WG14_RESULT_INLINE unsigned
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
#elif __STDC_VERSION__ >= 202300L && !defined(__clang__)
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

#ifdef __cplusplus
extern "C"
{
#endif


#ifdef __cplusplus
}
#endif

#endif
