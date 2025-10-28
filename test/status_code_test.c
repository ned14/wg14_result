#include "test_common.h"

#include "wg14_result/status_code.h"
#include "wg14_result/status_code_generic.h"

#ifdef _MSC_VER
#include <crtdbg.h>
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
static WG14_RESULT_PREFIX(status_code_domain_string_ref)
Code_domain_vtable_name(void);
static WG14_RESULT_PREFIX(status_code_domain_payload_info_t)
Code_domain_vtable_payload_info(void);
static bool Code_domain_vtable_failure(
const WG14_RESULT_PREFIX(status_code_untyped) * code);
static bool Code_domain_vtable_equivalent(
const WG14_RESULT_PREFIX(status_code_untyped) * code1,
const WG14_RESULT_PREFIX(status_code_untyped) * code2);
static WG14_RESULT_PREFIX(status_code_generic)
Code_domain_vtable_generic_code(const WG14_RESULT_PREFIX(status_code_untyped) *
                                code);
static WG14_RESULT_PREFIX(status_code_domain_string_ref)
Code_domain_vtable_message(const WG14_RESULT_PREFIX(status_code_untyped) *
                           code);

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

static WG14_RESULT_PREFIX(status_code_domain_string_ref)
Code_domain_vtable_name(void)
{
  return WG14_RESULT_PREFIX(status_code_domain_string_ref_from_static_string)(
  "Code_category_impl");
}

static WG14_RESULT_PREFIX(status_code_domain_payload_info_t)
Code_domain_vtable_payload_info(void)
{
  WG14_RESULT_PREFIX(status_code_domain_payload_info_t)
  ret = {
  sizeof(enum Code),
  sizeof(WG14_RESULT_PREFIX(status_code_domain) *) + sizeof(enum Code),
  (__alignof(enum Code) > __alignof(WG14_RESULT_PREFIX(status_code_domain) *)) ?
  __alignof(enum Code) :
  __alignof(WG14_RESULT_PREFIX(status_code_domain) *)};
  return ret;
}

static bool
Code_domain_vtable_failure(const WG14_RESULT_PREFIX(status_code_untyped) * code)
{
  assert(code->domain == &Code_domain);
  const StatusCode *c = (const StatusCode *) code;
  return ((int) c->value & 1) != 0;  // NOLINT
}

static bool Code_domain_vtable_equivalent(
const WG14_RESULT_PREFIX(status_code_untyped) * code1,
const WG14_RESULT_PREFIX(status_code_untyped) * code2)
{
  assert(code1->domain == &Code_domain);
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

static WG14_RESULT_PREFIX(status_code_generic)
Code_domain_vtable_generic_code(const WG14_RESULT_PREFIX(status_code_untyped) *
                                code)
{
  assert(code->domain == &Code_domain);
  const StatusCode *c = (const StatusCode *) code;
  switch(c->value)
  {
  case Code_success1:
  case Code_success2:
    return WG14_RESULT_PREFIX(status_code_generic_make)(
    WG14_RESULT_PREFIX(status_code_errc_success));
  case Code_goaway:
    return WG14_RESULT_PREFIX(status_code_generic_make)(
    WG14_RESULT_PREFIX(status_code_errc_permission_denied));
  case Code_error2:
    break;
  }
  WG14_RESULT_PREFIX(status_code_generic)
  ret = {{WG14_RESULT_NULLPTR}, WG14_RESULT_PREFIX(status_code_errc_success)};
  return ret;
}

static WG14_RESULT_PREFIX(status_code_domain_string_ref)
Code_domain_vtable_message(const WG14_RESULT_PREFIX(status_code_untyped) * code)
{
  assert(code->domain == &Code_domain);
  const StatusCode *c = (const StatusCode *) code;
  switch(c->value)
  {
  case Code_success1:
    return WG14_RESULT_PREFIX(status_code_domain_string_ref_from_static_string)(
    "success1");
  case Code_goaway:
    return WG14_RESULT_PREFIX(status_code_domain_string_ref_from_static_string)(
    "goaway");
  case Code_success2:
    return WG14_RESULT_PREFIX(status_code_domain_string_ref_from_static_string)(
    "success2");
  case Code_error2:
    return WG14_RESULT_PREFIX(status_code_domain_string_ref_from_static_string)(
    "error2");
  }
  WG14_RESULT_PREFIX(status_code_domain_string_ref)
  ret;
  memset(&ret, 0, sizeof(ret));
  return ret;
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


  printf("Exiting main with result %d ...\n", ret);
  return ret;
}
