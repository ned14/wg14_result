// Remember that this source file can be included into every TU, including from
// C++
#include "../../include/wg14_result/status_code_posix.h"

#include "../../include/wg14_result/status_code_generic.h"

#include <errno.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

  static int WG14_RESULT_VTABLE_API(
  status_code_posix_domain_vtable_name,
  struct WG14_RESULT_PREFIX(status_code_domain_vtable_name_args) * args);
  static void WG14_RESULT_VTABLE_API(
  status_code_posix_domain_vtable_payload_info,
  struct WG14_RESULT_PREFIX(status_code_domain_vtable_payload_info_args) *
  args);
  static bool
  WG14_RESULT_VTABLE_API(status_code_posix_domain_vtable_failure,
                         const WG14_RESULT_PREFIX(status_code_untyped) * code);
  static bool
  WG14_RESULT_VTABLE_API(status_code_posix_domain_vtable_equivalent,
                         const WG14_RESULT_PREFIX(status_code_untyped) * code1,
                         const WG14_RESULT_PREFIX(status_code_untyped) * code2);
  static void WG14_RESULT_VTABLE_API(
  status_code_posix_domain_vtable_generic_code,
  struct WG14_RESULT_PREFIX(status_code_domain_vtable_generic_code_args) *
  args);
  static int WG14_RESULT_VTABLE_API(
  status_code_posix_domain_vtable_message,
  struct WG14_RESULT_PREFIX(status_code_domain_vtable_message_args) * args);

  static WG14_RESULT_CONSTEXPR WG14_RESULT_PREFIX(status_code_domain_vtable)
  status_code_posix_domain_vtable = {
  .name = status_code_posix_domain_vtable_name,
  .payload_info = status_code_posix_domain_vtable_payload_info,
  .failure = status_code_posix_domain_vtable_failure,
  .equivalent = status_code_posix_domain_vtable_equivalent,
  .generic_code = status_code_posix_domain_vtable_generic_code,
  .message = status_code_posix_domain_vtable_message,
  .reserved_slot_for_cxx_throw_exception = WG14_RESULT_NULLPTR,
  .erased_copy = WG14_RESULT_PREFIX(default_erased_copy_impl),
  .erased_destroy = WG14_RESULT_PREFIX(default_erased_destroy_impl)  //
  };

  WG14_RESULT_PREFIX(status_code_domain) *
  WG14_RESULT_PREFIX(status_code_posix_domain)(void)
  {
    static WG14_RESULT_CONSTEXPR WG14_RESULT_PREFIX(status_code_domain)
    ret = {&status_code_posix_domain_vtable,
           0xa59a56fe5f310933 /* same as for C++ impl */};
    return &ret;
  }

  static int WG14_RESULT_VTABLE_API(
  status_code_posix_domain_vtable_name,
  struct WG14_RESULT_PREFIX(status_code_domain_vtable_name_args) * args)
  {
    args->ret = WG14_RESULT_PREFIX(
    status_code_domain_string_ref_from_static_string)("posix domain");
    return 0;
  }

  static void WG14_RESULT_VTABLE_API(
  status_code_posix_domain_vtable_payload_info,
  struct WG14_RESULT_PREFIX(status_code_domain_vtable_payload_info_args) * args)
  {
    const WG14_RESULT_PREFIX(status_code_domain_payload_info_t) ret =
    STATUS_CODE_DOMAIN_PAYLOAD_INFO_INIT(int);
    args->ret = ret;
  }

  static bool
  WG14_RESULT_VTABLE_API(status_code_posix_domain_vtable_failure,
                         const WG14_RESULT_PREFIX(status_code_untyped) * code)
  {
    assert(code->domain->id ==
           WG14_RESULT_PREFIX(status_code_posix_domain)()->id);
    const WG14_RESULT_PREFIX(status_code_posix) *c =
    (const WG14_RESULT_PREFIX(status_code_posix) *) code;
    return c->value != 0;  // NOLINT
  }

  static bool
  WG14_RESULT_VTABLE_API(status_code_posix_domain_vtable_equivalent,
                         const WG14_RESULT_PREFIX(status_code_untyped) * code1,
                         const WG14_RESULT_PREFIX(status_code_untyped) * code2)
  {
    assert(code1->domain->id ==
           WG14_RESULT_PREFIX(status_code_posix_domain)()->id);
    const WG14_RESULT_PREFIX(status_code_posix) *c1 =
    (const WG14_RESULT_PREFIX(status_code_posix) *) code1;
    if(code2->domain->id == code1->domain->id)
    {
      const WG14_RESULT_PREFIX(status_code_posix) *c2 =
      (const WG14_RESULT_PREFIX(status_code_posix) *) code2;
      return c1->value == c2->value;
    }
    if(code2->domain->id ==
       WG14_RESULT_PREFIX(status_code_generic_domain)()->id)
    {
      const WG14_RESULT_PREFIX(status_code_generic) *c2 =
      (WG14_RESULT_PREFIX(status_code_generic) *) code2;  // NOLINT
      if((int) c2->value == c1->value)
      {
        return true;
      }
    }
    return false;
  }

  static void WG14_RESULT_VTABLE_API(
  status_code_posix_domain_vtable_generic_code,
  struct WG14_RESULT_PREFIX(status_code_domain_vtable_generic_code_args) * args)
  {
    assert(args->code->domain->id ==
           WG14_RESULT_PREFIX(status_code_posix_domain)()->id);
    const WG14_RESULT_PREFIX(status_code_posix) *c =
    (const WG14_RESULT_PREFIX(status_code_posix) *) args->code;
    args->ret = WG14_RESULT_PREFIX(status_code_generic_make)(
    (enum WG14_RESULT_PREFIX(status_code_errc)) c->value);
  }

  static int WG14_RESULT_VTABLE_API(
  status_code_posix_domain_vtable_message,
  struct WG14_RESULT_PREFIX(status_code_domain_vtable_message_args) * args)
  {
    assert(args->code->domain->id ==
           WG14_RESULT_PREFIX(status_code_posix_domain)()->id);
    const int c =
    ((const WG14_RESULT_PREFIX(status_code_posix) *) args->code)->value;

    char buffer[1024] = "";
    errno = 0;
#ifdef _WIN32
    strerror_s(buffer, sizeof(buffer), c);
#else
  strerror_r(c, buffer, sizeof(buffer));
#endif
    const int errcode = errno;
    args->ret = WG14_RESULT_PREFIX(
    status_code_domain_string_ref_atomic_refcounted_from_string)(buffer);
    return errcode;
  }

#ifdef __cplusplus
}
#endif
