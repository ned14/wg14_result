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

#ifndef WG14_RESULT_STATUS_CODE_DOMAIN_H
#define WG14_RESULT_STATUS_CODE_DOMAIN_H

#include "config.h"

#include <assert.h>
#include <stdbool.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

  //! \brief Type of a unique id of a domain
  typedef unsigned long long
  WG14_RESULT_PREFIX(status_code_domain_unique_id_type);

  //! \brief Type of a string ref of a domain
  typedef struct WG14_RESULT_PREFIX(status_code_domain_string_ref)
  WG14_RESULT_PREFIX(status_code_domain_string_ref);

  //! \brief Type of a string ref thunk op
  enum WG14_RESULT_PREFIX(status_code_domain_string_ref_thunk_op)
  {
    WG14_RESULT_PREFIX(status_code_domain_string_ref_thunk_op_copy),
    WG14_RESULT_PREFIX(status_code_domain_string_ref_thunk_op_move),
    WG14_RESULT_PREFIX(status_code_domain_string_ref_thunk_op_destruct),
  };

  //! \brief Type of a string ref thunk function
  typedef bool (*status_code_domain_string_ref_thunk_spec)(
  WG14_RESULT_PREFIX(status_code_domain_string_ref) * dest,
  WG14_RESULT_PREFIX(status_code_domain_string_ref) * src,
  enum WG14_RESULT_PREFIX(status_code_domain_string_ref_thunk_op) op);

  struct WG14_RESULT_PREFIX(status_code_domain_string_ref)
  {
    const char *c_str, *end;
    void *state[3];
    const status_code_domain_string_ref_thunk_spec thunk;
  };

  //! \brief Attempt to copy a string ref
  WG14_RESULT_INLINE bool
  WG14_RESULT_PREFIX(status_code_domain_string_ref_copy)(
  WG14_RESULT_PREFIX(status_code_domain_string_ref) * dest,
  const WG14_RESULT_PREFIX(status_code_domain_string_ref) * src)
  {
    if(src->thunk != WG14_RESULT_NULLPTR)
    {
      return src->thunk(
      dest, (WG14_RESULT_PREFIX(status_code_domain_string_ref) *) src,
      WG14_RESULT_PREFIX(status_code_domain_string_ref_thunk_op_copy));
    }
    memcpy(dest, src, sizeof(*dest));
    return true;
  }

  //! \brief Attempt to move a string ref
  WG14_RESULT_INLINE bool
  WG14_RESULT_PREFIX(status_code_domain_string_ref_move)(
  WG14_RESULT_PREFIX(status_code_domain_string_ref) * dest,
  WG14_RESULT_PREFIX(status_code_domain_string_ref) * src)
  {
    if(src->thunk != WG14_RESULT_NULLPTR)
    {
      return src->thunk(
      dest, src,
      WG14_RESULT_PREFIX(status_code_domain_string_ref_thunk_op_move));
    }
    memcpy(dest, src, sizeof(*dest));
    memset(src, 0, sizeof(*src));
    return true;
  }

  //! \brief Destroy a string ref
  WG14_RESULT_INLINE bool
  WG14_RESULT_PREFIX(status_code_domain_string_ref_destroy)(
  WG14_RESULT_PREFIX(status_code_domain_string_ref) * src)
  {
    if(src->thunk != WG14_RESULT_NULLPTR)
    {
      return src->thunk(
      src, WG14_RESULT_NULLPTR,
      WG14_RESULT_PREFIX(status_code_domain_string_ref_thunk_op_destruct));
    }
    memset(src, 0, sizeof(*src));
    return true;
  }

  //! \brief Make a string ref which refers to a null terminated static string
  //! slice
  WG14_RESULT_INLINE WG14_RESULT_PREFIX(status_code_domain_string_ref)
  WG14_RESULT_PREFIX(status_code_domain_string_ref_from_static_string_slice)(
  const char *s, size_t len)
  {
    assert(s[len] == 0);
    const WG14_RESULT_PREFIX(status_code_domain_string_ref)
    ret = {s,
           s + len,
           {WG14_RESULT_NULLPTR, WG14_RESULT_NULLPTR, WG14_RESULT_NULLPTR},
           WG14_RESULT_NULLPTR};
    return ret;
  }

  //! \brief Make a string ref which refers to a null terminated static string
  WG14_RESULT_INLINE WG14_RESULT_PREFIX(status_code_domain_string_ref)
  WG14_RESULT_PREFIX(status_code_domain_string_ref_from_static_string)(
  const char *s)
  {
    return WG14_RESULT_PREFIX(
    status_code_domain_string_ref_from_static_string_slice)(s, strlen(s));
  }

  //! \brief Make an atomic refcounted string ref which tracks living copies
  //! using an atomic reference count, freeing the internally stored buffer on
  //! final destroy. The input range is copied and does not need to be null
  //! terminated. Failure to malloc causes a static string ref to be returned
  //! with a string like "failed to get message from system".
  WG14_RESULT_EXTERN WG14_RESULT_PREFIX(status_code_domain_string_ref)
  WG14_RESULT_PREFIX(
  status_code_domain_string_ref_atomic_refcounted_from_buffer)(const char *s,
                                                               size_t len);

  //! \brief Make an atomic refcounted string ref which copies the input null
  //! terminated static string
  WG14_RESULT_INLINE WG14_RESULT_PREFIX(status_code_domain_string_ref)
  WG14_RESULT_PREFIX(
  status_code_domain_string_ref_atomic_refcounted_from_string)(const char *s)
  {
    return WG14_RESULT_PREFIX(
    status_code_domain_string_ref_atomic_refcounted_from_buffer)(s, strlen(s));
  }


  /**************************************************************************/


  //! \brief The functions defined by a status code domain, kept ABI compatible
  //! with a C++ vtable
  typedef const struct WG14_RESULT_PREFIX(status_code_domain_s)
  WG14_RESULT_PREFIX(status_code_domain);

  //! \brief Type of a payload info of a domain
  typedef struct WG14_RESULT_PREFIX(status_code_domain_payload_info)
  {
    size_t payload_size;  //!< The payload size in bytes
    size_t
    total_size;  //!< The total status code size in bytes (includes domain pointer and mixins state)
    size_t total_alignment;  //!< The total status code alignment in bytes
  } WG14_RESULT_PREFIX(status_code_domain_payload_info_t);

  //! \brief Type of an untyped status code
  typedef struct WG14_RESULT_PREFIX(status_code_untyped)
  {
    WG14_RESULT_PREFIX(status_code_domain) * domain;
  } WG14_RESULT_PREFIX(status_code_untyped);

  //! \brief Type of a generic status code
  typedef struct WG14_RESULT_PREFIX(status_code_generic_s)
  WG14_RESULT_PREFIX(status_code_generic);

  //! \brief The functions defined by a status code domain, kept ABI compatible
  //! with a C++ vtable
  typedef const struct WG14_RESULT_PREFIX(status_code_domain_vtable_s)
  {
    //! Name of this category.
    WG14_RESULT_PREFIX(status_code_domain_string_ref) (*const name)(void);
    //! Information about this domain's payload
    WG14_RESULT_PREFIX(status_code_domain_payload_info_t)
    (*const payload_info)(void);
    //! True if code means failure.
    bool (*const failure)(const WG14_RESULT_PREFIX(status_code_untyped) * code);
    //! True if code is (potentially non-transitively) equivalent to another
    //! code in another domain.
    bool (*const equivalent)(const WG14_RESULT_PREFIX(status_code_untyped) *
                             code1,
                             const WG14_RESULT_PREFIX(status_code_untyped) *
                             code2);
    //! Returns the generic code closest to this code, if any.
    WG14_RESULT_PREFIX(status_code_generic)
    (*const generic_code)(const WG14_RESULT_PREFIX(status_code_untyped) * code);
    //! Return a reference to a string textually representing a code.
    WG14_RESULT_PREFIX(status_code_domain_string_ref)
    (*const message)(const WG14_RESULT_PREFIX(status_code_untyped) * code);
    //! ABI compatibility slot for throwing a code as a C++ exception, do not
    //! call this even from C++ (use the C++ implementation instead).
    void (*const reserved_slot_for_cxx_throw_exception)(
    const WG14_RESULT_PREFIX(status_code_untyped) * code);
    //! For a `status_code<erased<T>>` only, copy from `src` to `dst`. Default
    //! implementation uses `memcpy()`. You should return false here if your
    //! payload is not trivially copyable or would not fit.
    bool (*const erased_copy)(
    WG14_RESULT_PREFIX(status_code_untyped) * dst,
    const WG14_RESULT_PREFIX(status_code_untyped) * src,
    WG14_RESULT_PREFIX(status_code_domain_payload_info_t) dstinfo);
    //! For a `status_code<erased<T>>` only, destroy the erased value type.
    //! Default implementation does nothing.
    void (*const erased_destroy)(
    WG14_RESULT_PREFIX(status_code_untyped) * code,
    WG14_RESULT_PREFIX(status_code_domain_payload_info_t) info);
  } WG14_RESULT_PREFIX(status_code_domain_vtable);

  //! \brief The functions defined by a status code domain, kept ABI compatible
  //! with a C++ vtable
  struct WG14_RESULT_PREFIX(status_code_domain_s)
  {
    //! C++ ABI compatible vptr
    WG14_RESULT_PREFIX(status_code_domain_vtable) *const vptr;
    //! The unique id used to identify identical category instances.
    const WG14_RESULT_PREFIX(status_code_domain_unique_id_type) id;
  };

  //! \brief The default implementation for
  //! `status_code_domain_vtable.erased_copy()`.
  WG14_RESULT_INLINE bool WG14_RESULT_PREFIX(default_erased_copy_impl)(
  WG14_RESULT_PREFIX(status_code_untyped) * dst,
  const WG14_RESULT_PREFIX(status_code_untyped) * src,
  WG14_RESULT_PREFIX(status_code_domain_payload_info_t) dstinfo)
  {
    // Note that dst may not have its domain set
    const WG14_RESULT_PREFIX(status_code_domain_payload_info_t) srcinfo =
    src->domain->vptr->payload_info();
    if(dstinfo.total_size < srcinfo.total_size)
    {
      return false;
    }
    const size_t tocopy = (dstinfo.total_size > srcinfo.total_size) ?
                          srcinfo.total_size :
                          dstinfo.total_size;
    memcpy(&dst, &src, tocopy);
    return true;
  }

  //! \brief The default implementation for
  //! `status_code_domain_vtable.erased_destroy()`.
  WG14_RESULT_INLINE void WG14_RESULT_PREFIX(default_erased_destroy_impl)(
  WG14_RESULT_PREFIX(status_code_untyped) * code,
  WG14_RESULT_PREFIX(status_code_domain_payload_info_t) info)
  {
    (void) code;
    (void) info;
  }


  //! \brief Parse a uuid input string to yield a status code domain unique id
  WG14_RESULT_EXTERN WG14_RESULT_PREFIX(status_code_domain_unique_id_type)
  WG14_RESULT_PREFIX(status_code_domain_unique_id_from_uuid)(const char *uuid);

  // Alas the trick of (msg, 1 / 0) doesn't work in C23 constexpr :(
#ifdef __cplusplus
#define STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_ABORT(msg) (msg, 1 / 0)
#else
#define STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_ABORT(msg) (0)
#endif

#define STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_PARSE_HEX_BYTE(c)               \
  (('0' <= (c) && (c) <= '9') ? (unsigned long long) ((c) - '0') :             \
   ('a' <= (c) && (c) <= 'f') ? (unsigned long long) (10 + (c) - 'a') :        \
   ('A' <= (c) && (c) <= 'F') ? (unsigned long long) (10 + (c) - 'A') :        \
                                STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_ABORT(  \
                                "Invalid character in UUID"))

//! \brief Generate a constexpr expression which yields a status code domain
//! unique id
#define STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID(uuid)                           \
  (((STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_PARSE_HEX_BYTE(uuid[0]) ^          \
     STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_PARSE_HEX_BYTE(uuid[19]))          \
    << 0) |                                                                    \
   ((STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_PARSE_HEX_BYTE(uuid[1]) ^          \
     STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_PARSE_HEX_BYTE(uuid[20]))          \
    << 4) |                                                                    \
   ((STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_PARSE_HEX_BYTE(uuid[2]) ^          \
     STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_PARSE_HEX_BYTE(uuid[21]))          \
    << 8) |                                                                    \
   ((STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_PARSE_HEX_BYTE(uuid[3]) ^          \
     STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_PARSE_HEX_BYTE(uuid[22]))          \
    << 12) |                                                                   \
   ((STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_PARSE_HEX_BYTE(uuid[4]) ^          \
     STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_PARSE_HEX_BYTE(uuid[24]))          \
    << 16) |                                                                   \
   ((STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_PARSE_HEX_BYTE(uuid[5]) ^          \
     STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_PARSE_HEX_BYTE(uuid[25]))          \
    << 20) |                                                                   \
   ((STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_PARSE_HEX_BYTE(uuid[6]) ^          \
     STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_PARSE_HEX_BYTE(uuid[26]))          \
    << 24) |                                                                   \
   ((STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_PARSE_HEX_BYTE(uuid[7]) ^          \
     STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_PARSE_HEX_BYTE(uuid[27]))          \
    << 28) |                                                                   \
   ((STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_PARSE_HEX_BYTE(uuid[9]) ^          \
     STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_PARSE_HEX_BYTE(uuid[28]))          \
    << 32) |                                                                   \
   ((STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_PARSE_HEX_BYTE(uuid[10]) ^         \
     STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_PARSE_HEX_BYTE(uuid[29]))          \
    << 36) |                                                                   \
   ((STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_PARSE_HEX_BYTE(uuid[11]) ^         \
     STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_PARSE_HEX_BYTE(uuid[30]))          \
    << 40) |                                                                   \
   ((STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_PARSE_HEX_BYTE(uuid[12]) ^         \
     STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_PARSE_HEX_BYTE(uuid[31]))          \
    << 44) |                                                                   \
   ((STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_PARSE_HEX_BYTE(uuid[14]) ^         \
     STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_PARSE_HEX_BYTE(uuid[32]))          \
    << 48) |                                                                   \
   ((STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_PARSE_HEX_BYTE(uuid[15]) ^         \
     STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_PARSE_HEX_BYTE(uuid[33]))          \
    << 52) |                                                                   \
   ((STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_PARSE_HEX_BYTE(uuid[16]) ^         \
     STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_PARSE_HEX_BYTE(uuid[34]))          \
    << 56) |                                                                   \
   ((STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_PARSE_HEX_BYTE(uuid[17]) ^         \
     STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID_PARSE_HEX_BYTE(uuid[35]))          \
    << 60))

#ifdef __cplusplus
}
#endif

#endif
