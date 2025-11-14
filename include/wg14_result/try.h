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

#ifndef WG14_RESULT_STATUS_CODE_TRY_H
#define WG14_RESULT_STATUS_CODE_TRY_H

#include "config.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /* You should ensure that this tracks closely
  https://github.com/ned14/outcome/blob/develop/include/outcome/detail/try.h
  in both directions!
  */

#if !defined(__clang__) && defined(__GNUC__) && __GNUC__ >= 8
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wparentheses"
#endif

#define OUTCOME_TRY_GLUE2(x, y) x##y
#define OUTCOME_TRY_GLUE(x, y) OUTCOME_TRY_GLUE2(x, y)
#define OUTCOME_TRY_UNIQUE_NAME                                                \
  OUTCOME_TRY_GLUE(_outcome_try_unique_name_temporary, __COUNTER__)

#define OUTCOME_TRY_RETURN_ARG_COUNT(_1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_,   \
                                     count, ...)                               \
  count
#define OUTCOME_TRY_EXPAND_ARGS(args) OUTCOME_TRY_RETURN_ARG_COUNT args
#define OUTCOME_TRY_COUNT_ARGS_MAX8(...)                                       \
  OUTCOME_TRY_EXPAND_ARGS((__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1, 0))
#define OUTCOME_TRY_OVERLOAD_MACRO2(name, count) name##count
#define OUTCOME_TRY_OVERLOAD_MACRO1(name, count)                               \
  OUTCOME_TRY_OVERLOAD_MACRO2(name, count)
#define OUTCOME_TRY_OVERLOAD_MACRO(name, count)                                \
  OUTCOME_TRY_OVERLOAD_MACRO1(name, count)
#define OUTCOME_TRY_OVERLOAD_GLUE(x, y) x y
#define OUTCOME_TRY_CALL_OVERLOAD(name, ...)                                   \
  OUTCOME_TRY_OVERLOAD_GLUE(                                                   \
  OUTCOME_TRY_OVERLOAD_MACRO(name, OUTCOME_TRY_COUNT_ARGS_MAX8(__VA_ARGS__)),  \
  (__VA_ARGS__))

#define _OUTCOME_TRY_RETURN_ARG_COUNT(_1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_,  \
                                      count, ...)                              \
  count
#define _OUTCOME_TRY_EXPAND_ARGS(args) _OUTCOME_TRY_RETURN_ARG_COUNT args
#define _OUTCOME_TRY_COUNT_ARGS_MAX8(...)                                      \
  _OUTCOME_TRY_EXPAND_ARGS((__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1, 0))
#define _OUTCOME_TRY_OVERLOAD_MACRO2(name, count) name##count
#define _OUTCOME_TRY_OVERLOAD_MACRO1(name, count)                              \
  _OUTCOME_TRY_OVERLOAD_MACRO2(name, count)
#define _OUTCOME_TRY_OVERLOAD_MACRO(name, count)                               \
  _OUTCOME_TRY_OVERLOAD_MACRO1(name, count)
#define _OUTCOME_TRY_OVERLOAD_GLUE(x, y) x y
#define _OUTCOME_TRY_CALL_OVERLOAD(name, ...)                                  \
  _OUTCOME_TRY_OVERLOAD_GLUE(_OUTCOME_TRY_OVERLOAD_MACRO(                      \
                             name, _OUTCOME_TRY_COUNT_ARGS_MAX8(__VA_ARGS__)), \
                             (__VA_ARGS__))

#if !defined(OUTCOME_TRY_LIKELY_IF) && defined(__has_cpp_attribute)
#if __has_cpp_attribute(likely) && (__cplusplus >= 202000L || _HAS_CXX20)
#define OUTCOME_TRY_LIKELY_IF(...) if(__VA_ARGS__) [[likely]]
#endif
#endif
#ifndef OUTCOME_TRY_LIKELY_IF
#if defined(__clang__) || defined(__GNUC__)
#define OUTCOME_TRY_LIKELY_IF(...) if(__builtin_expect(!!(__VA_ARGS__), 1))
#else
#define OUTCOME_TRY_LIKELY_IF(...) if(__VA_ARGS__)
#endif
#endif

#ifdef __cplusplus
#define OUTCOME_TRYV2_UNIQUE_STORAGE_AUTO(...) auto
#else
#define OUTCOME_TRYV2_UNIQUE_STORAGE_AUTO(...) __typeof__(__VA_ARGS__)
#endif

#define OUTCOME_TRYV2_UNIQUE_STORAGE_UNPACK(...) __VA_ARGS__
#define OUTCOME_TRYV2_UNIQUE_STORAGE_DEDUCE3(unique, ...)                      \
  OUTCOME_TRYV2_UNIQUE_STORAGE_AUTO(__VA_ARGS__) unique = (__VA_ARGS__)
#define OUTCOME_TRYV2_UNIQUE_STORAGE_DEDUCE2(x) x
#define OUTCOME_TRYV2_UNIQUE_STORAGE_DEDUCE(unique, x, ...)                    \
  OUTCOME_TRYV2_UNIQUE_STORAGE_DEDUCE2(                                        \
  OUTCOME_TRYV2_UNIQUE_STORAGE_DEDUCE3(unique, __VA_ARGS__))
#define OUTCOME_TRYV2_UNIQUE_STORAGE_SPECIFIED3(unique, x, y, ...)             \
  x unique = (__VA_ARGS__)
#define OUTCOME_TRYV2_UNIQUE_STORAGE_SPECIFIED2(x) x
#define OUTCOME_TRYV2_UNIQUE_STORAGE_SPECIFIED(unique, ...)                    \
  OUTCOME_TRYV2_UNIQUE_STORAGE_SPECIFIED2(                                     \
  OUTCOME_TRYV2_UNIQUE_STORAGE_SPECIFIED3(unique, __VA_ARGS__))
#define OUTCOME_TRYV2_UNIQUE_STORAGE1(...) OUTCOME_TRYV2_UNIQUE_STORAGE_DEDUCE
#define OUTCOME_TRYV2_UNIQUE_STORAGE2(...)                                     \
  OUTCOME_TRYV2_UNIQUE_STORAGE_SPECIFIED
#define OUTCOME_TRYV2_UNIQUE_STORAGE(unique, spec, ...)                        \
  _OUTCOME_TRY_CALL_OVERLOAD(OUTCOME_TRYV2_UNIQUE_STORAGE,                     \
                             OUTCOME_TRYV2_UNIQUE_STORAGE_UNPACK spec)         \
  (unique, OUTCOME_TRYV2_UNIQUE_STORAGE_UNPACK spec, __VA_ARGS__)

#define OUTCOME_TRY2_VAR_SECOND2(x, var) var
#define OUTCOME_TRY2_VAR_SECOND3(x, y, ...) x y
#define OUTCOME_TRY2_VAR(spec)                                                 \
  _OUTCOME_TRY_CALL_OVERLOAD(OUTCOME_TRY2_VAR_SECOND,                          \
                             OUTCOME_TRYV2_UNIQUE_STORAGE_UNPACK spec, spec)

#if !defined(__clang__) && defined(__GNUC__) && __GNUC__ >= 8
#pragma GCC diagnostic pop
#endif


  /* You should ensure that this tracks closely
  https://github.com/ned14/outcome/blob/develop/include/outcome/experimental/result.h
  in both directions!
  */

#define WG14_RESULT_TRY_IMPLV(unique, retstmt, cleanup, spec, ...)             \
  OUTCOME_TRYV2_UNIQUE_STORAGE(unique, spec, __VA_ARGS__);                     \
  OUTCOME_TRY_LIKELY_IF(WG14_RESULT_HAS_VALUE(unique));                        \
  else                                                                         \
  {                                                                            \
    cleanup;                                                                   \
    retstmt;                                                                   \
  }
#define WG14_RESULT_TRY_IMPLA(unique, retstmt, cleanup, var, ...)              \
  WG14_RESULT_TRY_IMPLV(unique, retstmt, cleanup, var, __VA_ARGS__)            \
  OUTCOME_TRY2_VAR(var) = unique.value

#define WG14_RESULT_TRY_IMPL_RETURN(unique, ident)                             \
  WG14_RESULT(ident) unique##_f;                                               \
  unique##_f._flags_ = (unique)._flags_;                                       \
  unique##_f.error = (unique).error;                                           \
  return unique##_f

#define WG14_RESULT_TRY_WITH_SPEC_AND_CLEANUP_AND_NEW_RETURN_TYPE(             \
unique, spec, ident, cleanup, ...)                                             \
  WG14_RESULT_TRY_IMPLA(unique, WG14_RESULT_TRY_IMPL_RETURN(unique, ident),    \
                        cleanup, spec, __VA_ARGS__)
#define WG14_RESULT_TRY_WITH_SPEC_AND_CLEANUP(unique, spec, cleanup, ...)      \
  WG14_RESULT_TRY_IMPLA(unique, return unique, cleanup, spec, __VA_ARGS__)
#define WG14_RESULT_TRY_WITH_CLEANUP(unique, cleanup, ...)                     \
  WG14_RESULT_TRY_IMPLV(unique, return unique, cleanup, deduce, __VA_ARGS__)
#define WG14_RESULT_TRY_SAME_RETURN_TYPE(unique, ...)                          \
  WG14_RESULT_TRY_IMPLV(unique, return unique, , deduce, __VA_ARGS__)

#define WG14_RESULT_TRY_INVOKE_TRY8(a, b, c, d, e, f, g, h)                    \
  WG14_RESULT_TRY_WITH_SPEC_AND_CLEANUP_AND_NEW_RETURN_TYPE(                   \
  OUTCOME_TRY_UNIQUE_NAME, a, b, c, d, e, f, g, h)
#define WG14_RESULT_TRY_INVOKE_TRY7(a, b, c, d, e, f, g)                       \
  WG14_RESULT_TRY_WITH_SPEC_AND_CLEANUP_AND_NEW_RETURN_TYPE(                   \
  OUTCOME_TRY_UNIQUE_NAME, a, b, c, d, e, f, g)
#define WG14_RESULT_TRY_INVOKE_TRY6(a, b, c, d, e, f)                          \
  WG14_RESULT_TRY_WITH_SPEC_AND_CLEANUP_AND_NEW_RETURN_TYPE(                   \
  OUTCOME_TRY_UNIQUE_NAME, a, b, c, d, e, f)
#define WG14_RESULT_TRY_INVOKE_TRY5(a, b, c, d, e)                             \
  WG14_RESULT_TRY_WITH_SPEC_AND_CLEANUP_AND_NEW_RETURN_TYPE(                   \
  OUTCOME_TRY_UNIQUE_NAME, a, b, c, d, e)
#define WG14_RESULT_TRY_INVOKE_TRY4(a, b, c, d)                                \
  WG14_RESULT_TRY_WITH_SPEC_AND_CLEANUP_AND_NEW_RETURN_TYPE(                   \
  OUTCOME_TRY_UNIQUE_NAME, a, b, c, d)
#define WG14_RESULT_TRY_INVOKE_TRY3(a, b, c)                                   \
  WG14_RESULT_TRY_WITH_SPEC_AND_CLEANUP(OUTCOME_TRY_UNIQUE_NAME, a, b, c)
#define WG14_RESULT_TRY_INVOKE_TRY2(a, b)                                      \
  WG14_RESULT_TRY_WITH_CLEANUP(OUTCOME_TRY_UNIQUE_NAME, a, b)
#define WG14_RESULT_TRY_INVOKE_TRY1(expr)                                      \
  WG14_RESULT_TRY_SAME_RETURN_TYPE(OUTCOME_TRY_UNIQUE_NAME, expr)

#define WG14_RESULT_TRY(...)                                                   \
  OUTCOME_TRY_CALL_OVERLOAD(WG14_RESULT_TRY_INVOKE_TRY, __VA_ARGS__)


#ifdef __cplusplus
}
#endif

#endif
