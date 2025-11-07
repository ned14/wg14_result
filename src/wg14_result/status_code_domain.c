// Remember that this source file can be included into every TU, including from
// C++
#include "../../include/wg14_result/status_code_domain.h"

#include <string.h>

// Some toolchains get upset if <stdatomic.h> is included in C++
#ifdef __cplusplus
#include <atomic>
#define WG14_RESULT_ATOMIC_PREFIX std::
#define WG14_RESULT_ATOMIC_UINT atomic_uint
#else
#include <stdatomic.h>
#define WG14_RESULT_ATOMIC_PREFIX
#define WG14_RESULT_ATOMIC_UINT atomic_uint
#endif

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4200)  // zero-sized array in struct/union
#endif
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

  struct atomic_refcounted_string
  {
    WG14_RESULT_ATOMIC_PREFIX WG14_RESULT_ATOMIC_UINT count;
    char msg[];
  };

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#ifdef __cplusplus
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wclass-memaccess"
#endif
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4190)  // has C linkage, but returns type which is
                                 // incompatible with C
#endif
#endif

  static int atomic_refcounted_string_thunk(
  const WG14_RESULT_PREFIX(status_code_domain_string_ref_thunk_args) * args)
  {
    switch(args->op)
    {
    case WG14_RESULT_PREFIX(status_code_domain_string_ref_thunk_op_copy):
    {
      memcpy(args->dest, args->src, sizeof(*args->src));
      struct atomic_refcounted_string *p =
      (struct atomic_refcounted_string *) args->src->state[0];
      WG14_RESULT_ATOMIC_PREFIX
      atomic_fetch_add_explicit(&p->count, 1,
                                WG14_RESULT_ATOMIC_PREFIX memory_order_relaxed);
      return 0;
    }
    case WG14_RESULT_PREFIX(status_code_domain_string_ref_thunk_op_move):
    {
      memcpy(args->dest, args->src, sizeof(*args->dest));
      memset(args->src, 0, sizeof(*args->src));
      return 0;
    }
    case WG14_RESULT_PREFIX(status_code_domain_string_ref_thunk_op_destruct):
    {
      struct atomic_refcounted_string *p =
      (struct atomic_refcounted_string *) args->dest->state[0];
      if(1 == WG14_RESULT_ATOMIC_PREFIX atomic_fetch_sub_explicit(
              &p->count, 1, WG14_RESULT_ATOMIC_PREFIX memory_order_relaxed))
      {
        free(p);
      }
      memset(args->dest, 0, sizeof(*args->dest));
      return 0;
    }
    }
    return 0;
  }

  WG14_RESULT_PREFIX(status_code_domain_string_ref)
  WG14_RESULT_PREFIX(
  status_code_domain_string_ref_atomic_refcounted_from_buffer)
  (const char *s, size_t len)
  {
    struct atomic_refcounted_string *p =
    (struct atomic_refcounted_string *) malloc(
    sizeof(struct atomic_refcounted_string) + len + 1);
    if(p == WG14_RESULT_NULLPTR)
    {
      return WG14_RESULT_PREFIX(
      status_code_domain_string_ref_from_static_string)(
      "failed to get message from system");
    }
    WG14_RESULT_ATOMIC_PREFIX atomic_store_explicit(
    &p->count, 1, WG14_RESULT_ATOMIC_PREFIX memory_order_release);
    memcpy(p->msg, s, len);
    p->msg[len] = 0;
    const WG14_RESULT_PREFIX(status_code_domain_string_ref)
    ret = {p->msg,
           p->msg + len,
           {p, WG14_RESULT_NULLPTR, WG14_RESULT_NULLPTR},
           atomic_refcounted_string_thunk};
    return ret;
  }


  static inline unsigned long long parse_hex_byte(char c)
  {
    return ('0' <= c && c <= '9') ?
           (unsigned long long) (c - '0') :
           ('a' <= c && c <= 'f') ?
           (unsigned long long) (10 + c - 'a') :
           ('A' <= c && c <= 'F') ?
           (unsigned long long) (10 + c - 'A') :
           WG14_RESULT_ABORTF("Invalid character in UUID");
  }
  static inline unsigned long long parse_uuid2(const char *s)
  {
    return ((parse_hex_byte(s[0]) << 0) | (parse_hex_byte(s[1]) << 4) |
            (parse_hex_byte(s[2]) << 8) | (parse_hex_byte(s[3]) << 12) |
            (parse_hex_byte(s[4]) << 16) | (parse_hex_byte(s[5]) << 20) |
            (parse_hex_byte(s[6]) << 24) | (parse_hex_byte(s[7]) << 28) |
            (parse_hex_byte(s[9]) << 32) | (parse_hex_byte(s[10]) << 36) |
            (parse_hex_byte(s[11]) << 40) | (parse_hex_byte(s[12]) << 44) |
            (parse_hex_byte(s[14]) << 48) | (parse_hex_byte(s[15]) << 52) |
            (parse_hex_byte(s[16]) << 56) | (parse_hex_byte(s[17]) << 60))  //
           ^                                                                //
           ((parse_hex_byte(s[19]) << 0) | (parse_hex_byte(s[20]) << 4) |
            (parse_hex_byte(s[21]) << 8) | (parse_hex_byte(s[22]) << 12) |
            (parse_hex_byte(s[24]) << 16) | (parse_hex_byte(s[25]) << 20) |
            (parse_hex_byte(s[26]) << 24) | (parse_hex_byte(s[27]) << 28) |
            (parse_hex_byte(s[28]) << 32) | (parse_hex_byte(s[29]) << 36) |
            (parse_hex_byte(s[30]) << 40) | (parse_hex_byte(s[31]) << 44) |
            (parse_hex_byte(s[32]) << 48) | (parse_hex_byte(s[33]) << 52) |
            (parse_hex_byte(s[34]) << 56) | (parse_hex_byte(s[35]) << 60));
  }

  WG14_RESULT_PREFIX(status_code_domain_unique_id_type)
  WG14_RESULT_PREFIX(status_code_domain_unique_id_from_uuid)(const char *uuid)
  {
    const size_t uuidlen = strlen(uuid);
    return (uuidlen == 36) ?
           parse_uuid2(uuid) :
           ((uuidlen == 38) ?
            parse_uuid2(uuid + 1) :
            WG14_RESULT_ABORTF("UUID does not have correct length"));
  }


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#ifdef _MSC_VER
#pragma warning(pop)
#endif
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif
#endif
