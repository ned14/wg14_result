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
#include <errno.h>
#include <stdbool.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wclass-memaccess"
#endif
#endif
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996)  // use strerror_s instead
#endif

  //! \brief Type of a unique id of a domain
  typedef unsigned long long
  WG14_RESULT_PREFIX(status_code_domain_unique_id_type);

  //! \brief Type of a string ref of a domain
  typedef struct WG14_RESULT_PREFIX(status_code_domain_string_ref_s)
  WG14_RESULT_PREFIX(status_code_domain_string_ref);

  //! \brief Type of a string ref thunk op
  enum WG14_RESULT_PREFIX(status_code_domain_string_ref_thunk_op)
  {
    WG14_RESULT_PREFIX(status_code_domain_string_ref_thunk_op_copy),
    WG14_RESULT_PREFIX(status_code_domain_string_ref_thunk_op_move),
    WG14_RESULT_PREFIX(status_code_domain_string_ref_thunk_op_destruct),
  };

  //! \brief Type of the arguments to a string ref thunk function
  typedef struct WG14_RESULT_PREFIX(status_code_domain_string_ref_thunk_args_s)
  {
    WG14_RESULT_PREFIX(status_code_domain_string_ref) * dest;
    WG14_RESULT_PREFIX(status_code_domain_string_ref) * src;
    enum WG14_RESULT_PREFIX(status_code_domain_string_ref_thunk_op) op;
  } WG14_RESULT_PREFIX(status_code_domain_string_ref_thunk_args);

  //! \brief Type of a string ref thunk function. Returns an `errno` value.
  //! Copies can fail. Nothing else can.
  typedef int (*status_code_domain_string_ref_thunk_spec)(
  const WG14_RESULT_PREFIX(status_code_domain_string_ref_thunk_args) * args);

  //! \brief Type of a string ref of a domain
  struct WG14_RESULT_PREFIX(status_code_domain_string_ref_s)
  {
    const char *c_str, *end;
    void *state[3];
    status_code_domain_string_ref_thunk_spec thunk;
  };

  //! \brief Returns true if the string ref is empty
  WG14_RESULT_INLINE bool
  WG14_RESULT_PREFIX(status_code_domain_string_ref_is_empty)(
  const WG14_RESULT_PREFIX(status_code_domain_string_ref) * s)
  {
    return s->c_str == WG14_RESULT_NULLPTR;
  }

  //! \brief Attempt to copy a string ref, returning an `errno` cause if it
  //! failed.
  WG14_RESULT_INLINE int WG14_RESULT_PREFIX(status_code_domain_string_ref_copy)(
  WG14_RESULT_PREFIX(status_code_domain_string_ref) * dest,
  const WG14_RESULT_PREFIX(status_code_domain_string_ref) * src)
  {
    if(src->thunk != WG14_RESULT_NULLPTR)
    {
      const WG14_RESULT_PREFIX(status_code_domain_string_ref_thunk_args)
      args = {dest, (WG14_RESULT_PREFIX(status_code_domain_string_ref) *) src,
              WG14_RESULT_PREFIX(status_code_domain_string_ref_thunk_op_copy)};
      return src->thunk(&args);
    }
    memcpy(dest, src, sizeof(*dest));
    return 0;
  }

  //! \brief Attempt to move a string ref
  WG14_RESULT_INLINE void
  WG14_RESULT_PREFIX(status_code_domain_string_ref_move)(
  WG14_RESULT_PREFIX(status_code_domain_string_ref) * dest,
  WG14_RESULT_PREFIX(status_code_domain_string_ref) * src)
  {
    if(src->thunk != WG14_RESULT_NULLPTR)
    {
      const WG14_RESULT_PREFIX(status_code_domain_string_ref_thunk_args)
      args = {dest, src,
              WG14_RESULT_PREFIX(status_code_domain_string_ref_thunk_op_move)};
      const int errcode = src->thunk(&args);
      if(errcode != 0)
      {
        WG14_RESULT_ABORTF(
        "status_code_domain_string_ref_move failed due to %d (%s)", errcode,
        strerror(errcode));
      }
      return;
    }
    memcpy(dest, src, sizeof(*dest));
    memset(src, 0, sizeof(*src));
  }

  //! \brief Destroy a string ref
  WG14_RESULT_INLINE void
  WG14_RESULT_PREFIX(status_code_domain_string_ref_destroy)(
  const WG14_RESULT_PREFIX(status_code_domain_string_ref) * _src)
  {
    WG14_RESULT_PREFIX(status_code_domain_string_ref) *src =
    (WG14_RESULT_PREFIX(status_code_domain_string_ref) *) _src;
    if(src->thunk != WG14_RESULT_NULLPTR)
    {
      const WG14_RESULT_PREFIX(status_code_domain_string_ref_thunk_args)
      args = {
      src, WG14_RESULT_NULLPTR,
      WG14_RESULT_PREFIX(status_code_domain_string_ref_thunk_op_destruct)};
      const int errcode = src->thunk(&args);
      if(errcode != 0)
      {
        WG14_RESULT_ABORTF(
        "status_code_domain_string_ref_destroy failed due to %d (%s)", errcode,
        strerror(errcode));
      }
      return;
    }
    memset(src, 0, sizeof(*src));
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

//! \brief A default initialiser for a `status_code_domain_payload_info_t` for
//! most types
#define STATUS_CODE_DOMAIN_PAYLOAD_INFO_INIT(T)                                \
  {sizeof(T), sizeof(WG14_RESULT_PREFIX(status_code_domain) *) + sizeof(T),    \
   (__alignof(T) > __alignof(WG14_RESULT_PREFIX(status_code_domain) *)) ?      \
   __alignof(T) :                                                              \
   __alignof(WG14_RESULT_PREFIX(status_code_domain) *)}

  //! \brief Type of an untyped status code
  typedef struct WG14_RESULT_PREFIX(status_code_untyped)
  {
    WG14_RESULT_PREFIX(status_code_domain) * domain;
  } WG14_RESULT_PREFIX(status_code_untyped);

  //! \brief Type of a generic status code
  typedef struct WG14_RESULT_PREFIX(status_code_generic_s)
  WG14_RESULT_PREFIX(status_code_generic);

  /* We have to do a bit of work to achieve C++ vtable calling convention
  (__thiscall) compatibility which won't always be possible on all platforms,
  but for those where it is then the C++ implementation can mark domains `final`
  and get the benefits of linker implemented devirtualisation.

  As a quick refresher as I'll likely forget all this in years to come, and
  **remembering that complex return types take the first parameter register to
  point to where in the stack the return type should be stored**:

  - x86 Microsoft:
      - cdecl right-to-left order, caller cleanup, always uses stack
      - stdcall right-to-left order, **callee** cleanup, always uses stack
      - thiscall right-to-left order, **callee** cleanup, always uses stack (BUT
        ECX is `this` and effectively gets dropped for stdcall code)
  - x86 System V:
      - cdecl right-to-left order, caller cleanup, always uses stack
      - thiscall right-to-left order, caller cleanup, always uses stack
  - x64 Microsoft:
      - cdecl right-to-left order, caller cleanup, starts from RCX
      - thiscall right-to-left order, caller cleanup, starts from RDX (RCX is
        specially treated as always `this`, so complex return type stack address
        would go into RDX, then RCX and RDX are swapped compared to cdecl)
  - x64 System V:
      - cdecl right-to-left order, caller cleanup, starts from RDI
      - thiscall right-to-left order, caller cleanup, starts from RDI

  As far as I can tell, no major modern compiler other than MSVC does weird
  things with `this`, so for everybody else we can safely assume `this` will be
  the first argument. If we avoid complex return types, we eliminate a great
  deal of complexity so we do exactly that.
  */
//! \brief The implementation specific markup for a C++ vtable function, if any.
#ifndef WG14_RESULT_VTABLE_API
#define WG14_RESULT_VTABLE_API_GLUE2(x, y) x##y
#define WG14_RESULT_VTABLE_API_GLUE(x, y) WG14_RESULT_VTABLE_API_GLUE2(x, y)
#define WG14_RESULT_VTABLE_API_UNIQUE_NAME                                     \
  WG14_RESULT_VTABLE_API_GLUE(_wg14_result_vtable_api_unique_name_temporary,   \
                              __COUNTER__)

#ifdef _MSC_VER
#ifdef _M_IX86
  // Use __stdcall to implement callee cleanup. `this` gets dropped.
#define WG14_RESULT_VTABLE_API(name, ...) __stdcall name(__VA_ARGS__)
#define WG14_RESULT_VTABLE_DECL(name, ...) (__stdcall name)(__VA_ARGS__)
  /* There is almost certainly a more efficient way of doing this, but I'm not
  sure how much I care for x86 MSVC which not a lot of people use nowadays.

  TODO FIXME: This should use stack instead of TLS
  */
  static
  __declspec(thread) struct WG14_RESULT_PREFIX(win32_invoke_with_this_storage_s)
  {
    void *ebp;
    void *esp;
    void *oldstack[3];
  } WG14_RESULT_PREFIX(win32_invoke_with_this_storage);
  inline int __declspec(naked) WG14_RESULT_PREFIX(win32_invoke_with_this)(
  struct WG14_RESULT_PREFIX(win32_invoke_with_this_storage_s) * tls, void *func,
  WG14_RESULT_PREFIX(status_code_domain) * domain, ...)
  {
    (void) tls;
    (void) func;
    (void) domain;
    __asm {
      // save ebp into TLS
        mov eax, [esp + 4]
        mov [eax + 0], ebp
        mov ebp, eax
      // ebp now points at the TLS. Save the original stack pointer
        mov [ebp + 4], esp
      // Save the twelve bytes of stack we are about to clobber
        mov eax, [esp + 0]  // return address
        mov [ebp + 8], eax
        mov eax, [esp + 8]  // function to call
        mov [ebp + 12], eax
        mov eax, [esp + 12]  // this value
        mov [ebp + 16], eax
      // pop return address + tls
        add esp, 8
        pop eax  // function to call
        pop ecx  // this value as per MSVC __thiscall calling convention
        call eax   // jump to function
                // WARNING: You cannot touch eax nor ecx from now on!

      // ebp will point to tls
      // Because __thiscall does callee cleanup, esp will now be wrong because
      // this function is always __cdecl due to the varargs. So fix that first.
        mov esp, [ebp + 4]
        add esp, 16
      // Restore the clobbered stack
        push [ebp + 16]  // this value
        push [ebp + 12]  // function to call
        push ebp  // tls
        push [ebp + 8]  // return address
                   // restore ebp
        mov ebp, [ebp + 0]
                   // return to caller
        ret
    }
  }
  inline bool __declspec(naked) WG14_RESULT_PREFIX(win32_invoke_with_this_bool)(
  struct WG14_RESULT_PREFIX(win32_invoke_with_this_storage_s) * tls, void *func,
  WG14_RESULT_PREFIX(status_code_domain) * domain, ...)
  {
    (void) tls;
    (void) func;
    (void) domain;
    __asm {
      // save ebp into TLS
        mov eax, [esp + 4]
        mov [eax + 0], ebp
        mov ebp, eax
      // ebp now points at the TLS. Save the original stack pointer
        mov [ebp + 4], esp
      // Save the twelve bytes of stack we are about to clobber
        mov eax, [esp + 0]  // return address
        mov [ebp + 8], eax
        mov eax, [esp + 8]  // function to call
        mov [ebp + 12], eax
        mov eax, [esp + 12]  // this value
        mov [ebp + 16], eax
      // pop return address + tls
        add esp, 8
        pop eax  // function to call
        pop ecx  // this value as per MSVC __thiscall calling convention
        call eax   // jump to function
                // WARNING: You cannot touch eax nor ecx from now on!

      // ebp will point to tls
      // Because __thiscall does callee cleanup, esp will now be wrong because
      // this function is always __cdecl due to the varargs. So fix that first.
        mov esp, [ebp + 4]
        add esp, 16
      // Restore the clobbered stack
        push [ebp + 16]  // this value
        push [ebp + 12]  // function to call
        push ebp  // tls
        push [ebp + 8]  // return address
                   // restore ebp
        mov ebp, [ebp + 0]
                   // return to caller
        ret
    }
  }
// We also need to work around a bug in MSVC's handling of bool ...
#ifdef __cplusplus
}
template <class T> struct WG14_RESULT_PREFIX(win32_invoke_with_this_impl)
{
  static constexpr auto value = WG14_RESULT_PREFIX(win32_invoke_with_this);
};
template <> struct WG14_RESULT_PREFIX(win32_invoke_with_this_impl)<bool>
{
  static constexpr auto value = WG14_RESULT_PREFIX(win32_invoke_with_this_bool);
};
extern "C"
{
#define WG14_RESULT_VTABLE_INVOKE_API(domain, name, ...)                       \
  WG14_RESULT_PREFIX(                                                          \
  win32_invoke_with_this_impl<decltype((domain)->vptr->name(                   \
  __VA_ARGS__))>)::value(&WG14_RESULT_PREFIX(win32_invoke_with_this_storage),  \
                         (void *) (domain)->vptr->name, (domain), __VA_ARGS__)
#else
#define WG14_RESULT_VTABLE_INVOKE_API(domain, name, ...)                       \
  (_Generic((domain)->vptr->name(__VA_ARGS__),                                 \
  bool: WG14_RESULT_PREFIX(win32_invoke_with_this_bool)(                       \
            &WG14_RESULT_PREFIX(win32_invoke_with_this_storage),               \
            (void *) (domain)->vptr->name, (domain), __VA_ARGS__),             \
  default: WG14_RESULT_PREFIX(win32_invoke_with_this)(                         \
            &WG14_RESULT_PREFIX(win32_invoke_with_this_storage),               \
            (void *) (domain)->vptr->name, (domain), __VA_ARGS__)))
#endif
#else
#define WG14_RESULT_VTABLE_API(name, ...)                                      \
  name(__pragma(warning(suppress : 4100))                                      \
       const void *WG14_RESULT_VTABLE_API_UNIQUE_NAME /*discard this*/,        \
       __VA_ARGS__)
#define WG14_RESULT_VTABLE_DECL(name, ...)                                     \
  (name)(__pragma(warning(suppress : 4100))                                    \
         const void *WG14_RESULT_VTABLE_API_UNIQUE_NAME /*discard this*/,      \
         __VA_ARGS__)
#define WG14_RESULT_VTABLE_INVOKE_API(domain, name, ...)                       \
  (domain)->vptr->name((domain), __VA_ARGS__)
#endif
#else
// No special markup nor handling
#define WG14_RESULT_VTABLE_API(name, ...)                                      \
  name(__attribute__((unused))                                                 \
       const void *WG14_RESULT_VTABLE_API_UNIQUE_NAME /*discard this*/,        \
       __VA_ARGS__)
#define WG14_RESULT_VTABLE_DECL(name, ...)                                     \
  (name)(__attribute__((unused))                                               \
         const void *WG14_RESULT_VTABLE_API_UNIQUE_NAME /*discard this*/,      \
         __VA_ARGS__)
#define WG14_RESULT_VTABLE_INVOKE_API(domain, name, ...)                       \
  (domain)->vptr->name((domain), __VA_ARGS__)
#endif
#endif

  //! \brief The arguments for `status_code_domain_vtable.name`
  struct WG14_RESULT_PREFIX(status_code_domain_vtable_name_args)
  {
    WG14_RESULT_PREFIX(status_code_domain_string_ref) ret;
    WG14_RESULT_PREFIX(status_code_domain) * domain;
  };

  //! \brief The arguments for `status_code_domain_vtable.payload_info`
  struct WG14_RESULT_PREFIX(status_code_domain_vtable_payload_info_args)
  {
    WG14_RESULT_PREFIX(status_code_domain_payload_info_t) ret;
    WG14_RESULT_PREFIX(status_code_domain) * domain;
  };

  struct WG14_RESULT_PREFIX(status_code_domain_vtable_generic_code_args);

  //! \brief The arguments for `status_code_domain_vtable.message`
  struct WG14_RESULT_PREFIX(status_code_domain_vtable_message_args)
  {
    WG14_RESULT_PREFIX(status_code_domain_string_ref) ret;
    const WG14_RESULT_PREFIX(status_code_untyped) * code;
  };

  //! \brief The functions defined by a status code domain, kept ABI compatible
  //! with a C++ vtable
  typedef const struct WG14_RESULT_PREFIX(status_code_domain_vtable_s)
  {
    //! Name of this category. Returns an `errno` if it failed.
    int WG14_RESULT_VTABLE_DECL(*const name,
                                struct WG14_RESULT_PREFIX(
                                status_code_domain_vtable_name_args) *
                                args);
    //! Information about this domain's payload
    void WG14_RESULT_VTABLE_DECL(*const payload_info,
                                 struct WG14_RESULT_PREFIX(
                                 status_code_domain_vtable_payload_info_args) *
                                 args);
    //! True if code means failure.
    bool WG14_RESULT_VTABLE_DECL(*const failure,
                                 const WG14_RESULT_PREFIX(status_code_untyped) *
                                 code);
    //! True if code is (potentially non-transitively) equivalent to another
    //! code in another domain.
    bool WG14_RESULT_VTABLE_DECL(*const equivalent,
                                 const WG14_RESULT_PREFIX(status_code_untyped) *
                                 code1,
                                 const WG14_RESULT_PREFIX(status_code_untyped) *
                                 code2);
    //! Returns the generic code closest to this code, if any.
    void WG14_RESULT_VTABLE_DECL(*const generic_code,
                                 struct WG14_RESULT_PREFIX(
                                 status_code_domain_vtable_generic_code_args) *
                                 args);
    //! Return a reference to a string textually representing a code. Returns an
    //! `errno` if it failed.
    int WG14_RESULT_VTABLE_DECL(*const message,
                                struct WG14_RESULT_PREFIX(
                                status_code_domain_vtable_message_args) *
                                args);
    //! ABI compatibility slot for throwing a code as a C++ exception, do not
    //! call this even from C++ (use the C++ implementation instead).
    void WG14_RESULT_VTABLE_DECL(*const reserved_slot_for_cxx_throw_exception,
                                 const WG14_RESULT_PREFIX(status_code_untyped) *
                                 code);
    //! For a `status_code<erased<T>>` only, copy from `src` to `dst`. Default
    //! implementation uses `memcpy()`. Returns an `errno` if it failed. You
    //! should return an error code here if your payload is not trivially
    //! copyable or would not fit.
    int WG14_RESULT_VTABLE_DECL(
    *const erased_copy, WG14_RESULT_PREFIX(status_code_untyped) * dst,
    const WG14_RESULT_PREFIX(status_code_untyped) * src,
    WG14_RESULT_PREFIX(status_code_domain_payload_info_t) dstinfo);
    //! For a `status_code<erased<T>>` only, destroy the erased value type.
    //! Default implementation does nothing.
    void WG14_RESULT_VTABLE_DECL(*const erased_destroy,
                                 WG14_RESULT_PREFIX(status_code_untyped) * code,
                                 WG14_RESULT_PREFIX(
                                 status_code_domain_payload_info_t) info);
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
  WG14_RESULT_INLINE int
  WG14_RESULT_VTABLE_API(WG14_RESULT_PREFIX(default_erased_copy_impl),
                         WG14_RESULT_PREFIX(status_code_untyped) * dst,
                         const WG14_RESULT_PREFIX(status_code_untyped) * src,
                         WG14_RESULT_PREFIX(status_code_domain_payload_info_t)
                         dstinfo)
  {
    // Note that dst may not have its domain set
    struct WG14_RESULT_PREFIX(status_code_domain_vtable_payload_info_args) args;
    memset(&args, 0, sizeof(args));
    args.domain = src->domain;
    WG14_RESULT_VTABLE_INVOKE_API(src->domain, payload_info, &args);
    if(dstinfo.total_size < args.ret.total_size)
    {
      return ENOBUFS;
    }
    const size_t tocopy = (dstinfo.total_size > args.ret.total_size) ?
                          args.ret.total_size :
                          dstinfo.total_size;
    memcpy(dst, src, tocopy);
    return 0;
  }

  //! \brief The default implementation for
  //! `status_code_domain_vtable.erased_destroy()`.
  WG14_RESULT_INLINE void
  WG14_RESULT_VTABLE_API(WG14_RESULT_PREFIX(default_erased_destroy_impl),
                         WG14_RESULT_PREFIX(status_code_untyped) * code,
                         WG14_RESULT_PREFIX(status_code_domain_payload_info_t)
                         info)
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

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#ifdef __cplusplus
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif
}
#endif

#if WG14_RESULT_ENABLE_HEADER_ONLY
#include "../../src/wg14_result/status_code_domain.c"
#endif

#endif
