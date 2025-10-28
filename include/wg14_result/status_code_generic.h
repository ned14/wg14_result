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

#ifndef WG14_RESULT_STATUS_CODE_GENERIC_H
#define WG14_RESULT_STATUS_CODE_GENERIC_H

#include "status_code_domain.h"

#include <errno.h>

#ifdef __cplusplus
extern "C"
{
#endif

  //! The generic error coding (POSIX)
  enum WG14_RESULT_PREFIX(status_code_errc)
  {
    WG14_RESULT_PREFIX(status_code_errc_success) = 0,
    WG14_RESULT_PREFIX(status_code_errc_unknown) = -1,

    WG14_RESULT_PREFIX(status_code_errc_address_family_not_supported) =
    EAFNOSUPPORT,
    WG14_RESULT_PREFIX(status_code_errc_address_in_use) = EADDRINUSE,
    WG14_RESULT_PREFIX(status_code_errc_address_not_available) = EADDRNOTAVAIL,
    WG14_RESULT_PREFIX(status_code_errc_already_connected) = EISCONN,
    WG14_RESULT_PREFIX(status_code_errc_argument_list_too_long) = E2BIG,
    WG14_RESULT_PREFIX(status_code_errc_argument_out_of_domain) = EDOM,
    WG14_RESULT_PREFIX(status_code_errc_bad_address) = EFAULT,
    WG14_RESULT_PREFIX(status_code_errc_bad_file_descriptor) = EBADF,
    WG14_RESULT_PREFIX(status_code_errc_bad_message) = EBADMSG,
    WG14_RESULT_PREFIX(status_code_errc_broken_pipe) = EPIPE,
    WG14_RESULT_PREFIX(status_code_errc_connection_aborted) = ECONNABORTED,
    WG14_RESULT_PREFIX(status_code_errc_connection_already_in_progress) =
    EALREADY,
    WG14_RESULT_PREFIX(status_code_errc_connection_refused) = ECONNREFUSED,
    WG14_RESULT_PREFIX(status_code_errc_connection_reset) = ECONNRESET,
    WG14_RESULT_PREFIX(status_code_errc_cross_device_link) = EXDEV,
    WG14_RESULT_PREFIX(status_code_errc_destination_address_required) =
    EDESTADDRREQ,
    WG14_RESULT_PREFIX(status_code_errc_device_or_resource_busy) = EBUSY,
    WG14_RESULT_PREFIX(status_code_errc_directory_not_empty) = ENOTEMPTY,
    WG14_RESULT_PREFIX(status_code_errc_executable_format_error) = ENOEXEC,
    WG14_RESULT_PREFIX(status_code_errc_file_exists) = EEXIST,
    WG14_RESULT_PREFIX(status_code_errc_file_too_large) = EFBIG,
    WG14_RESULT_PREFIX(status_code_errc_filename_too_long) = ENAMETOOLONG,
    WG14_RESULT_PREFIX(status_code_errc_function_not_supported) = ENOSYS,
    WG14_RESULT_PREFIX(status_code_errc_host_unreachable) = EHOSTUNREACH,
    WG14_RESULT_PREFIX(status_code_errc_identifier_removed) = EIDRM,
    WG14_RESULT_PREFIX(status_code_errc_illegal_byte_sequence) = EILSEQ,
    WG14_RESULT_PREFIX(status_code_errc_inappropriate_io_control_operation) =
    ENOTTY,
    WG14_RESULT_PREFIX(status_code_errc_interrupted) = EINTR,
    WG14_RESULT_PREFIX(status_code_errc_invalid_argument) = EINVAL,
    WG14_RESULT_PREFIX(status_code_errc_invalid_seek) = ESPIPE,
    WG14_RESULT_PREFIX(status_code_errc_io_error) = EIO,
    WG14_RESULT_PREFIX(status_code_errc_is_a_directory) = EISDIR,
    WG14_RESULT_PREFIX(status_code_errc_message_size) = EMSGSIZE,
    WG14_RESULT_PREFIX(status_code_errc_network_down) = ENETDOWN,
    WG14_RESULT_PREFIX(status_code_errc_network_reset) = ENETRESET,
    WG14_RESULT_PREFIX(status_code_errc_network_unreachable) = ENETUNREACH,
    WG14_RESULT_PREFIX(status_code_errc_no_buffer_space) = ENOBUFS,
    WG14_RESULT_PREFIX(status_code_errc_no_child_process) = ECHILD,
    WG14_RESULT_PREFIX(status_code_errc_no_link) = ENOLINK,
    WG14_RESULT_PREFIX(status_code_errc_no_lock_available) = ENOLCK,
    WG14_RESULT_PREFIX(status_code_errc_no_message) = ENOMSG,
    WG14_RESULT_PREFIX(status_code_errc_no_protocol_option) = ENOPROTOOPT,
    WG14_RESULT_PREFIX(status_code_errc_no_space_on_device) = ENOSPC,
    WG14_RESULT_PREFIX(status_code_errc_no_stream_resources) = ENOSR,
    WG14_RESULT_PREFIX(status_code_errc_no_such_device_or_address) = ENXIO,
    WG14_RESULT_PREFIX(status_code_errc_no_such_device) = ENODEV,
    WG14_RESULT_PREFIX(status_code_errc_no_such_file_or_directory) = ENOENT,
    WG14_RESULT_PREFIX(status_code_errc_no_such_process) = ESRCH,
    WG14_RESULT_PREFIX(status_code_errc_not_a_directory) = ENOTDIR,
    WG14_RESULT_PREFIX(status_code_errc_not_a_socket) = ENOTSOCK,
    WG14_RESULT_PREFIX(status_code_errc_not_a_stream) = ENOSTR,
    WG14_RESULT_PREFIX(status_code_errc_not_connected) = ENOTCONN,
    WG14_RESULT_PREFIX(status_code_errc_not_enough_memory) = ENOMEM,
    WG14_RESULT_PREFIX(status_code_errc_not_supported) = ENOTSUP,
    WG14_RESULT_PREFIX(status_code_errc_operation_canceled) = ECANCELED,
    WG14_RESULT_PREFIX(status_code_errc_operation_in_progress) = EINPROGRESS,
    WG14_RESULT_PREFIX(status_code_errc_operation_not_permitted) = EPERM,
    WG14_RESULT_PREFIX(status_code_errc_operation_not_supported) = EOPNOTSUPP,
    WG14_RESULT_PREFIX(status_code_errc_operation_would_block) = EWOULDBLOCK,
    WG14_RESULT_PREFIX(status_code_errc_owner_dead) = EOWNERDEAD,
    WG14_RESULT_PREFIX(status_code_errc_permission_denied) = EACCES,
    WG14_RESULT_PREFIX(status_code_errc_protocol_error) = EPROTO,
    WG14_RESULT_PREFIX(status_code_errc_protocol_not_supported) =
    EPROTONOSUPPORT,
    WG14_RESULT_PREFIX(status_code_errc_read_only_file_system) = EROFS,
    WG14_RESULT_PREFIX(status_code_errc_resource_deadlock_would_occur) =
    EDEADLK,
    WG14_RESULT_PREFIX(status_code_errc_resource_unavailable_try_again) =
    EAGAIN,
    WG14_RESULT_PREFIX(status_code_errc_result_out_of_range) = ERANGE,
    WG14_RESULT_PREFIX(status_code_errc_state_not_recoverable) =
    ENOTRECOVERABLE,
    WG14_RESULT_PREFIX(status_code_errc_stream_timeout) = ETIME,
    WG14_RESULT_PREFIX(status_code_errc_text_file_busy) = ETXTBSY,
    WG14_RESULT_PREFIX(status_code_errc_timed_out) = ETIMEDOUT,
    WG14_RESULT_PREFIX(status_code_errc_too_many_files_open_in_system) = ENFILE,
    WG14_RESULT_PREFIX(status_code_errc_too_many_files_open) = EMFILE,
    WG14_RESULT_PREFIX(status_code_errc_too_many_links) = EMLINK,
    WG14_RESULT_PREFIX(status_code_errc_too_many_symbolic_link_levels) = ELOOP,
    WG14_RESULT_PREFIX(status_code_errc_value_too_large) = EOVERFLOW,
    WG14_RESULT_PREFIX(status_code_errc_wrong_protocol_type) = EPROTOTYPE
  };

  //! \brief Retrieve the domain for `status_code_generic`
  WG14_RESULT_EXTERN
  WG14_RESULT_SINGLETON WG14_RESULT_PREFIX(status_code_domain) *
  WG14_RESULT_PREFIX(status_code_generic_domain)(void);

  //! \brief A generic status code
  typedef struct WG14_RESULT_PREFIX(status_code_generic_s)
  {
    WG14_RESULT_PREFIX(status_code_untyped) base;
    enum WG14_RESULT_PREFIX(status_code_errc) value;
  } WG14_RESULT_PREFIX(status_code_generic);

  //! \brief Make a `status_code_generic`
  WG14_RESULT_INLINE WG14_RESULT_PREFIX(status_code_generic) WG14_RESULT_PREFIX(
  status_code_generic_make)(enum WG14_RESULT_PREFIX(status_code_errc) val)
  {
    const WG14_RESULT_PREFIX(status_code_generic)
    ret = {{WG14_RESULT_PREFIX(status_code_generic_domain)()}, val};
    return ret;
  }

  //! \brief True if the status codes are semantically equivalent in any way
  //! (implementation). Guaranteed transitive. Firstly
  //! `status_code_strictly_equivalent()` is run in both directions. If neither
  //! succeeds, each domain is asked for the equivalent generic code, and those
  //! are compared.
  WG14_RESULT_INLINE bool WG14_RESULT_PREFIX(status_code_equivalent)(
  const WG14_RESULT_PREFIX(status_code_untyped) * primary,
  const WG14_RESULT_PREFIX(status_code_untyped) * secondary)
  {
    if(primary->domain != WG14_RESULT_NULLPTR &&
       secondary->domain != WG14_RESULT_NULLPTR)
    {
      if(primary->domain->vptr->equivalent(primary, secondary))
      {
        return true;
      }
      if(secondary->domain->vptr->equivalent(secondary, primary))
      {
        return true;
      }
      const WG14_RESULT_PREFIX(status_code_generic) c1 =
      secondary->domain->vptr->generic_code(secondary);
      if(c1.value != WG14_RESULT_PREFIX(status_code_errc_unknown) &&
         primary->domain->vptr->equivalent(primary, &c1.base))
      {
        return true;
      }
      const WG14_RESULT_PREFIX(status_code_generic) c2 =
      primary->domain->vptr->generic_code(primary);
      if(c2.value != WG14_RESULT_PREFIX(status_code_errc_unknown) &&
         secondary->domain->vptr->equivalent(secondary, &c2.base))
      {
        return true;
      }
    }
    // If we are both empty, we are equivalent
    if(WG14_RESULT_NULLPTR == primary->domain &&
       WG14_RESULT_NULLPTR == secondary->domain)
    {
      return true;  // NOLINT
    }
    // Otherwise not equivalent
    return false;
  }

//! \brief True if the status codes are semantically equivalent in any way
//! (implementation). Guaranteed transitive. Firstly
//! `status_code_strictly_equivalent()` is run in both directions. If neither
//! succeeds, each domain is asked for the equivalent generic code, and those
//! are compared.
#define status_code_equivalent(primary, secondary)                             \
  WG14_RESULT_PREFIX(status_code_equivalent)                                   \
  (&(primary).base, &(secondary).base)

#ifdef __cplusplus
}
#endif

#endif
