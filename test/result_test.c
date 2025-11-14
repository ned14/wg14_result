#include "test_common.h"

#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-function"
#endif

#include "wg14_result/result.h"
#include "wg14_result/status_code_generic.h"
#include "wg14_result/status_code_system.h"
#include "wg14_result/try.h"

#ifdef _MSC_VER
#include <crtdbg.h>
#pragma warning(disable : 4116)  // unnamed type definition in parentheses
#pragma warning(disable : 4505)  // unreferenced function with internal linkage
                                 // has been removed
#endif


/* The unit test is heavily borrowed from
 * https://github.com/ned14/status-code/blob/master/test/result.cpp
 */

WG14_RESULT_DECLARE(int, int)
typedef WG14_RESULT(int) result_int;
WG14_RESULT_DECLARE(bool, bool)
typedef WG14_RESULT(bool) result_bool;
typedef struct WG14_RESULT_PREFIX(result_with_void) result_void;

result_int test1(int x)
{
  if(x > 0)
  {
    return WG14_RESULT_MAKE_SUCCESS(int, x);
  }
  return WG14_RESULT_MAKE_FAILURE(
  int, WG14_RESULT_PREFIX(status_code_system_make_from_errc)(
       WG14_RESULT_PREFIX(status_code_errc_argument_out_of_domain)));
}

result_int test2(int x)
{
  WG14_RESULT_TRY(
  int r /* variable to extract any successful value into */,
  printf(
  "test2 finds test1 returns failure.\n") /* cleanup to perform if TRY fails */,
  test1(x) /* result returning expression to TRY */);
  printf("test2 finds test1 returns successful value %d.\n", r);
  return WG14_RESULT_MAKE_SUCCESS(int, r);
}

result_void test3(int x)
{
  WG14_RESULT_TRY(
  int r /* variable to extract any successful value into */,
  void /* override the return result type */,
  printf(
  "test3 finds test1 returns failure.\n") /* cleanup to perform if TRY fails */,
  test1(x) /* result returning expression to TRY */);
  printf("test3 finds test1 returns successful value %d.\n", r);
  return WG14_RESULT_MAKE_SUCCESS(void, );
}

int main(void)
{
  int ret = 0;
#ifdef _MSC_VER
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
  WG14_RESULT_CONSTEXPR_OR_CONST WG14_RESULT_PREFIX(status_code_system)
  erased_bad_address = WG14_RESULT_PREFIX(status_code_system_make_from_errc)(
  WG14_RESULT_PREFIX(status_code_errc_bad_address));

  {  // errored int
    result_int m = WG14_RESULT_MAKE_FAILURE(int, erased_bad_address);
    CHECK(!WG14_RESULT_HAS_VALUE(m));
    CHECK(WG14_RESULT_HAS_ERROR(m));
  }
  {  // errored void
    result_void m = WG14_RESULT_MAKE_FAILURE(void, erased_bad_address);
    CHECK(!WG14_RESULT_HAS_VALUE(m));
    CHECK(WG14_RESULT_HAS_ERROR(m));
  }
  {  // valued int
    result_int m = WG14_RESULT_MAKE_SUCCESS(int, 5);
    CHECK(WG14_RESULT_HAS_VALUE(m));
    CHECK(!WG14_RESULT_HAS_ERROR(m));
    CHECK(m.value == 5);
    m.value = 6;
    CHECK(m.value == 6);
  }
  {  // valued bool
    result_bool m = WG14_RESULT_MAKE_SUCCESS(bool, false);
    CHECK(WG14_RESULT_HAS_VALUE(m));
    CHECK(!WG14_RESULT_HAS_ERROR(m));
    CHECK(m.value == false);
    m.value = true;
    CHECK(m.value == true);
  }
  {  // valued void
    result_void m = WG14_RESULT_MAKE_SUCCESS(void, );
    CHECK(WG14_RESULT_HAS_VALUE(m));
    CHECK(!WG14_RESULT_HAS_ERROR(m));
  }
#if 0
  {  // errored
    error ec(errc::no_link);
    result<int> m(ec.clone());
    CHECK(!m);
    CHECK(!m.has_value());
    CHECK(m.has_error());
    // CHECK(!m.has_exception());
    BOOST_CHECK_THROW(m.value(), generic_error);
    CHECK(m.error() == ec);
  }
  //  if(false)  // NOLINT
  //  {          // void, void is permitted, but is not constructible
  //    result<void, void> *m = nullptr;
  //    m->value();
  //    m->error();
  //  }

  {
    // Deliberately define non-trivial operations
    struct udt
    {
      int _v{0};
      udt() = default;
      udt(udt &&o) noexcept
          : _v(o._v)
      {
      }
      udt(const udt &o)  // NOLINT
          : _v(o._v)
      {
      }
      udt &operator=(udt &&o) noexcept
      {
        _v = o._v;
        return *this;
      }
      udt &operator=(const udt &o)  // NOLINT
      {
        _v = o._v;
        return *this;
      }
      ~udt() { _v = 0; }
    };
    // No default construction, no copy nor move
    struct udt2
    {
      udt2() = delete;
      udt2(udt2 &&) = delete;
      udt2(const udt2 &) = delete;
      udt2 &operator=(udt2 &&) = delete;
      udt2 &operator=(const udt2 &) = delete;
      explicit udt2(int /*unused*/) {}
      ~udt2() = default;
    };
    // Can only be constructed via multiple args
    struct udt3
    {
      udt3() = delete;
      udt3(udt3 &&) = delete;
      udt3(const udt3 &) = delete;
      udt3 &operator=(udt3 &&) = delete;
      udt3 &operator=(const udt3 &) = delete;
      explicit udt3(int /*unused*/, const char * /*unused*/,
                    std::nullptr_t /*unused*/)
      {
      }
      ~udt3() = default;
    };

    result<int> a(5);
    result<int> b(generic_code{errc::invalid_argument});
    std::cout << sizeof(a) << std::endl;  // 32 bytes
    if(false)                             // NOLINT
    {
      b.assume_value();
      a.assume_error();
    }
#ifdef __cpp_exceptions
    try
    {
      b.value();
      std::cerr << "fail" << std::endl;
      std::terminate();
    }
    catch(const generic_error &e)
    {
      CHECK(!strcmp(e.what(), b.error().message().c_str()));
    }
#endif
    static_assert(!std::is_default_constructible<decltype(a)>::value, "");
    static_assert(!std::is_nothrow_default_constructible<decltype(a)>::value,
                  "");
    static_assert(!std::is_copy_constructible<decltype(a)>::value, "");
    static_assert(!std::is_trivially_copy_constructible<decltype(a)>::value,
                  "");
    static_assert(!std::is_nothrow_copy_constructible<decltype(a)>::value, "");
    static_assert(!std::is_copy_assignable<decltype(a)>::value, "");
    static_assert(!std::is_trivially_copy_assignable<decltype(a)>::value, "");
    static_assert(!std::is_nothrow_copy_assignable<decltype(a)>::value, "");
    static_assert(!std::is_trivially_destructible<decltype(a)>::value, "");
    static_assert(std::is_nothrow_destructible<decltype(a)>::value, "");

    // Test void compiles
    result<void> c(in_place_type<void>);

    // Test a standard udt compiles
    result<udt> d(in_place_type<udt>);
    static_assert(!std::is_default_constructible<decltype(d)>::value, "");
    static_assert(!std::is_nothrow_default_constructible<decltype(d)>::value,
                  "");
    static_assert(!std::is_copy_constructible<decltype(d)>::value, "");
    static_assert(!std::is_trivially_copy_constructible<decltype(d)>::value,
                  "");
    static_assert(!std::is_nothrow_copy_constructible<decltype(d)>::value, "");
    static_assert(!std::is_copy_assignable<decltype(d)>::value, "");
    static_assert(!std::is_trivially_copy_assignable<decltype(d)>::value, "");
    static_assert(!std::is_nothrow_copy_assignable<decltype(d)>::value, "");
    static_assert(std::is_move_assignable<decltype(d)>::value, "");
    static_assert(!std::is_trivially_move_assignable<decltype(d)>::value, "");
    static_assert(std::is_nothrow_move_assignable<decltype(d)>::value, "");
    static_assert(!std::is_trivially_destructible<decltype(d)>::value, "");
    static_assert(std::is_nothrow_destructible<decltype(d)>::value, "");

    // Test a highly pathological udt compiles
    result<udt2> e(in_place_type<udt2>, 5);
    // result<udt2> e2(e);
    static_assert(!std::is_default_constructible<decltype(e)>::value, "");
    static_assert(!std::is_nothrow_default_constructible<decltype(e)>::value,
                  "");
    static_assert(!std::is_copy_constructible<decltype(e)>::value, "");
    static_assert(!std::is_trivially_copy_constructible<decltype(e)>::value,
                  "");
    static_assert(!std::is_nothrow_copy_constructible<decltype(e)>::value, "");
    static_assert(!std::is_copy_assignable<decltype(e)>::value, "");
    static_assert(!std::is_trivially_copy_assignable<decltype(e)>::value, "");
    static_assert(!std::is_nothrow_copy_assignable<decltype(e)>::value, "");
    static_assert(!std::is_move_assignable<decltype(e)>::value, "");
    static_assert(!std::is_trivially_move_assignable<decltype(e)>::value, "");
    static_assert(!std::is_nothrow_move_assignable<decltype(e)>::value, "");

    // Test a udt which can only be constructed in place compiles
    result<udt3> g(in_place_type<udt3>, 5, static_cast<const char *>("niall"),
                   nullptr);
    // Does converting inplace construction also work?
    result<udt3> h(5, static_cast<const char *>("niall"), nullptr);
    result<udt3> i(generic_code{errc::not_enough_memory});
    CHECK(h.has_value());
    CHECK(i.has_error());
  }

  // Test direct use of error code enum works
  {
    /*constexpr*/ result<int> a(5), b(errc::invalid_argument);
    CHECK(a.value() == 5);
    CHECK(b.error() == errc::invalid_argument);
  }
#endif

  {
    result_int success1 = test2(1);
    result_int failure1 = test2(0);
    result_void success2 = test3(1);
    result_void failure2 = test3(0);

    CHECK(WG14_RESULT_HAS_VALUE(success1));
    CHECK(WG14_RESULT_HAS_ERROR(failure1));
    CHECK(WG14_RESULT_HAS_VALUE(success2));
    CHECK(WG14_RESULT_HAS_ERROR(failure2));
  }

  printf("Exiting main with result %d ...\n", ret);
  return ret;
}
