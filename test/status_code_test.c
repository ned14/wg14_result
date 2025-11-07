#include "test_common.h"

#include "wg14_result/status_code.h"
#include "wg14_result/status_code_generic.h"

#if WG14_RESULT_HAVE_POSIX_SUPPORT
#include "wg14_result/status_code_posix.h"
#endif

#ifdef _MSC_VER
#include <crtdbg.h>
#endif

#ifdef __clang__
#pragma clang diagnostic ignored                                               \
"-Wgnu-statement-expression-from-macro-expansion"
#endif
#ifdef __GCC__
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

/* The unit test is heavily borrowed from
 * https://github.com/ned14/status-code/blob/master/test/main.cpp
 */

// An error coding with multiple success values
enum Code
{
  Code_success1,
  Code_goaway,
  Code_success2,
  Code_error2
};

// "Full fat" custom status code domain
static int WG14_RESULT_VTABLE_API(
Code_domain_vtable_name,
struct WG14_RESULT_PREFIX(status_code_domain_vtable_name_args) * args);
static void WG14_RESULT_VTABLE_API(
Code_domain_vtable_payload_info,
struct WG14_RESULT_PREFIX(status_code_domain_vtable_payload_info_args) * args);
static bool
WG14_RESULT_VTABLE_API(Code_domain_vtable_failure,
                       const WG14_RESULT_PREFIX(status_code_untyped) * code);
static bool
WG14_RESULT_VTABLE_API(Code_domain_vtable_equivalent,
                       const WG14_RESULT_PREFIX(status_code_untyped) * code1,
                       const WG14_RESULT_PREFIX(status_code_untyped) * code2);
static void WG14_RESULT_VTABLE_API(
Code_domain_vtable_generic_code,
struct WG14_RESULT_PREFIX(status_code_domain_vtable_generic_code_args) * args);
static int WG14_RESULT_VTABLE_API(
Code_domain_vtable_message,
struct WG14_RESULT_PREFIX(status_code_domain_vtable_message_args) * args);

static WG14_RESULT_CONSTEXPR
WG14_RESULT_PREFIX(status_code_domain_vtable) Code_domain_vtable = {
.name = Code_domain_vtable_name,
.payload_info = Code_domain_vtable_payload_info,
.failure = Code_domain_vtable_failure,
.equivalent = Code_domain_vtable_equivalent,
.generic_code = Code_domain_vtable_generic_code,
.message = Code_domain_vtable_message,
.reserved_slot_for_cxx_throw_exception = WG14_RESULT_NULLPTR,
.erased_copy = WG14_RESULT_PREFIX(default_erased_copy_impl),
.erased_destroy = WG14_RESULT_PREFIX(default_erased_destroy_impl)  //
};

static WG14_RESULT_CONSTEXPR WG14_RESULT_PREFIX(status_code_domain)
Code_domain = {.vptr = &Code_domain_vtable,
#ifdef _MSC_VER
               .id = 7886328514083815424
#else
               .id = STATUS_CODE_DOMAIN_UNIQUE_ID_FROM_UUID(
               "430f1201-94fc-06c7-430f-120194111111")
#endif
};
STATUS_CODE_WITH_PAYLOAD_DECLARE(enum Code, Code)
typedef STATUS_CODE_WITH_PAYLOAD(Code) StatusCode;

static int WG14_RESULT_VTABLE_API(
Code_domain_vtable_name,
struct WG14_RESULT_PREFIX(status_code_domain_vtable_name_args) * args)
{
  args->ret = WG14_RESULT_PREFIX(
  status_code_domain_string_ref_from_static_string)("Code_category_impl");
  return 0;
}

static void WG14_RESULT_VTABLE_API(
Code_domain_vtable_payload_info,
struct WG14_RESULT_PREFIX(status_code_domain_vtable_payload_info_args) * args)
{
  const WG14_RESULT_PREFIX(status_code_domain_payload_info_t) ret =
  STATUS_CODE_DOMAIN_PAYLOAD_INFO_INIT(enum Code);
  args->ret = ret;
}

static bool
WG14_RESULT_VTABLE_API(Code_domain_vtable_failure,
                       const WG14_RESULT_PREFIX(status_code_untyped) * code)
{
  assert(code->domain->id == Code_domain.id);
  const StatusCode *c = (const StatusCode *) code;
  return ((int) c->value & 1) != 0;  // NOLINT
}

static bool
WG14_RESULT_VTABLE_API(Code_domain_vtable_equivalent,
                       const WG14_RESULT_PREFIX(status_code_untyped) * code1,
                       const WG14_RESULT_PREFIX(status_code_untyped) * code2)
{
  assert(code1->domain->id == Code_domain.id);
  const StatusCode *c1 = (const StatusCode *) code1;
  if(code2->domain->id == code1->domain->id)
  {
    const StatusCode *c2 = (const StatusCode *) code2;
    return c1->value == c2->value;
  }
  // If the other category is generic
  if(code2->domain->id == WG14_RESULT_PREFIX(status_code_generic_domain)()->id)
  {
    const WG14_RESULT_PREFIX(status_code_generic) *c2 =
    (const WG14_RESULT_PREFIX(status_code_generic) *) code2;  // NOLINT
    switch(c1->value)
    {
    case Code_success1:
    case Code_success2:
      return c2->value == WG14_RESULT_PREFIX(status_code_errc_success);
    case Code_goaway:
      switch(c2->value)
      {
      case WG14_RESULT_PREFIX(status_code_errc_permission_denied):
      case WG14_RESULT_PREFIX(status_code_errc_operation_not_permitted):
        return true;
      default:
        return false;
      }
    case Code_error2:
      return false;
    }
  }
  return false;
}

static void WG14_RESULT_VTABLE_API(
Code_domain_vtable_generic_code,
struct WG14_RESULT_PREFIX(status_code_domain_vtable_generic_code_args) * args)
{
  assert(args->code->domain->id == Code_domain.id);
  const StatusCode *c = (const StatusCode *) args->code;
  switch(c->value)
  {
  case Code_success1:
  case Code_success2:
    args->ret = WG14_RESULT_PREFIX(status_code_generic_make)(
    WG14_RESULT_PREFIX(status_code_errc_success));
    return;
  case Code_goaway:
    args->ret = WG14_RESULT_PREFIX(status_code_generic_make)(
    WG14_RESULT_PREFIX(status_code_errc_permission_denied));
    return;
  case Code_error2:
    break;
  }
  const WG14_RESULT_PREFIX(status_code_generic)
  ret = {{WG14_RESULT_NULLPTR}, WG14_RESULT_PREFIX(status_code_errc_success)};
  args->ret = ret;
}

static int WG14_RESULT_VTABLE_API(
Code_domain_vtable_message,
struct WG14_RESULT_PREFIX(status_code_domain_vtable_message_args) * args)
{
  assert(args->code->domain->id == Code_domain.id);
  const StatusCode *c = (const StatusCode *) args->code;
  switch(c->value)
  {
  case Code_success1:
    args->ret = WG14_RESULT_PREFIX(
    status_code_domain_string_ref_from_static_string)("success1");
    return 0;
  case Code_goaway:
    args->ret = WG14_RESULT_PREFIX(
    status_code_domain_string_ref_from_static_string)("goaway");
    return 0;
  case Code_success2:
    args->ret = WG14_RESULT_PREFIX(
    status_code_domain_string_ref_from_static_string)("success2");
    return 0;
  case Code_error2:
    args->ret = WG14_RESULT_PREFIX(
    status_code_domain_string_ref_from_static_string)("error2");
    return 0;
  }
  WG14_RESULT_PREFIX(status_code_domain_string_ref) ret;
  memset(&ret, 0, sizeof(ret));
  args->ret = ret;
  return 0;
}


int main(void)
{
  int ret = 0;
#ifdef _MSC_VER
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  WG14_RESULT_CONSTEXPR_OR_CONST WG14_RESULT_PREFIX(status_code_generic)
  empty1 = {{WG14_RESULT_NULLPTR},
            WG14_RESULT_PREFIX(status_code_errc_success)},
  success1 = WG14_RESULT_PREFIX(status_code_generic_make)(
  WG14_RESULT_PREFIX(status_code_errc_success)),
  failure1 = WG14_RESULT_PREFIX(status_code_generic_make)(
  WG14_RESULT_PREFIX(status_code_errc_permission_denied));
  CHECK(status_code_is_empty(empty1));
  CHECK(!status_code_is_empty(success1));
  CHECK(!status_code_is_empty(failure1));
  CHECK(status_code_is_success(success1));
  CHECK(status_code_is_failure(failure1));
  printf("generic_code empty has value %d (%s) is success %d is failure %d\n",
         empty1.value, status_code_message(empty1).c_str,
         status_code_is_success(empty1), status_code_is_failure(empty1));
  printf("generic_code success has value %d (%s) is success %d is failure %d\n",
         success1.value, status_code_message(success1).c_str,
         status_code_is_success(success1), status_code_is_failure(success1));
  printf("generic_code failure has value %d (%s) is success %d is failure %d\n",
         failure1.value, status_code_message(failure1).c_str,
         status_code_is_success(failure1), status_code_is_failure(failure1));

  WG14_RESULT_CONSTEXPR_OR_CONST StatusCode
  empty2 = {{WG14_RESULT_NULLPTR}, Code_success1},
  success2 = STATUS_CODE_WITH_PAYLOAD_MAKE(Code)(Code_success1),
  failure2 = STATUS_CODE_WITH_PAYLOAD_MAKE(Code)(Code_goaway);
  CHECK(status_code_is_success(success2));
  CHECK(status_code_is_failure(failure2));
  printf("\nStatusCode empty has value %d (%s) is success %d is failure %d\n",
         empty2.value, status_code_message(empty2).c_str,
         status_code_is_success(empty2), status_code_is_failure(empty2));
  printf("StatusCode success has value %d (%s) is success %d is failure %d\n",
         success2.value, status_code_message(success2).c_str,
         status_code_is_success(success2), status_code_is_failure(success2));
  printf("StatusCode failure has value %d (%s) is success %d is failure %d\n",
         failure2.value, status_code_message(failure2).c_str,
         status_code_is_success(failure2), status_code_is_failure(failure2));

  printf("\n(empty1 == empty2) = %d\n",
         status_code_equivalent(
         empty1,
         empty2));  // True, empty ec's always compare equal no matter the type
  printf("(success1 == success2) = %d\n",
         status_code_equivalent(success1,
                                success2));  // True, success maps onto success
  printf("(success1 == failure2) = %d\n",
         status_code_equivalent(
         success1, failure2));  // False, success does not map onto failure
  printf("(failure1 == success2) = %d\n",
         status_code_equivalent(
         failure1, success2));  // False, failure does not map onto success
  printf("(failure1 == failure2) = %d\n",
         status_code_equivalent(
         failure1, failure2));  // True, filename_too_long maps onto nospace
  CHECK(status_code_equivalent(empty1, empty2));
  CHECK(status_code_equivalent(success1, success2));
  CHECK(!status_code_equivalent(success1, failure2));
  CHECK(!status_code_equivalent(failure1, success2));
  CHECK(status_code_equivalent(failure1, failure2));

  // Test typed clone and destroy macros
#if !defined(_MSC_VER) || defined(__clang__)
  {
    StatusCode dest = {{WG14_RESULT_NULLPTR}, Code_success1};
    CHECK(status_code_clone(&dest, &failure2) == 0);
    printf("(dest == failure2) = %d\n",
           status_code_strictly_equivalent(dest, failure2));
    CHECK(status_code_strictly_equivalent(dest, failure2));
    status_code_destroy(&dest);
  }
#endif

  // Test atomic_refcounted_string_ref (used by Windows status codes)
  {
    const char msg[] = "status test message";

    WG14_RESULT_PREFIX(status_code_domain_string_ref)
    shared_str1 = WG14_RESULT_PREFIX(
    status_code_domain_string_ref_atomic_refcounted_from_string)(msg);
    WG14_RESULT_PREFIX(status_code_domain_string_ref) shared_str2;
    CHECK(0 == WG14_RESULT_PREFIX(status_code_domain_string_ref_copy)(
               &shared_str2, &shared_str1));

    CHECK(!status_code_domain_string_ref_is_empty(&shared_str1));
    CHECK(!status_code_domain_string_ref_is_empty(&shared_str2));
    CHECK(shared_str1.c_str == shared_str2.c_str);
    CHECK(0 == strcmp(shared_str1.c_str, msg));
    CHECK(0 == strcmp(shared_str2.c_str, msg));

    WG14_RESULT_PREFIX(status_code_domain_string_ref) shared_str3;
    WG14_RESULT_PREFIX(status_code_domain_string_ref_move)
    (&shared_str3, &shared_str1);
    CHECK(status_code_domain_string_ref_is_empty(&shared_str1));
    CHECK(!status_code_domain_string_ref_is_empty(&shared_str2));
    CHECK(!status_code_domain_string_ref_is_empty(&shared_str3));
    CHECK(shared_str2.c_str == shared_str3.c_str);
    CHECK(0 == strcmp(shared_str3.c_str, msg));

    WG14_RESULT_PREFIX(status_code_domain_string_ref_destroy)(&shared_str1);
    WG14_RESULT_PREFIX(status_code_domain_string_ref_destroy)(&shared_str2);
    WG14_RESULT_PREFIX(status_code_domain_string_ref_destroy)(&shared_str3);
  }

#if WG14_RESULT_HAVE_POSIX_SUPPORT
  // Test posix_code
  WG14_RESULT_CONSTEXPR_OR_CONST WG14_RESULT_PREFIX(status_code_posix)
  success9 = WG14_RESULT_PREFIX(status_code_posix_make)(0),
  failure9 = WG14_RESULT_PREFIX(status_code_posix_make)(EACCES);
  CHECK(status_code_is_success(success9));
  CHECK(status_code_is_failure(failure9));
  const WG14_RESULT_PREFIX(status_code_domain_string_ref) success9msg =
  status_code_message(success9);
  const WG14_RESULT_PREFIX(status_code_domain_string_ref) failure9msg =
  status_code_message(failure9);
  printf("\nPOSIX code success has value %d (%s) is success %d is failure %d\n",
         success9.value, success9msg.c_str, status_code_is_success(success9),
         status_code_is_failure(success9));
  printf("POSIX code failure has value %d (%s) is success %d is failure %d\n",
         failure9.value, failure9msg.c_str, status_code_is_success(failure9),
         status_code_is_failure(failure9));
  CHECK(status_code_equivalent_errc(
  success9, WG14_RESULT_PREFIX(status_code_errc_success)));
  CHECK(status_code_equivalent_errc(
  failure9, WG14_RESULT_PREFIX(status_code_errc_permission_denied)));
  CHECK(status_code_equivalent(failure9, failure1));
  CHECK(status_code_equivalent(failure9, failure2));
#if 0
  system_code success10(success9), failure10(failure9);
  CHECK(success10 == errc::success);
  CHECK(failure10 == errc::permission_denied);
  CHECK(failure10 == failure1);
  CHECK(failure10 == failure2);
#endif
  WG14_RESULT_PREFIX(status_code_domain_string_ref_destroy)(&success9msg);
  WG14_RESULT_PREFIX(status_code_domain_string_ref_destroy)(&failure9msg);
#endif

  printf("Exiting main with result %d ...\n", ret);
  return ret;
}
