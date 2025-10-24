#include "wg14_result/status_code_generic.h"

static WG14_RESULT_PREFIX(status_code_domain_string_ref)
status_code_generic_domain_vtable_name(void);
static WG14_RESULT_PREFIX(status_code_domain_payload_info_t)
status_code_generic_domain_vtable_payload_info(void);
static bool status_code_generic_domain_vtable_failure(
const WG14_RESULT_PREFIX(status_code_untyped) * code);
static bool status_code_generic_domain_vtable_equivalent(
const WG14_RESULT_PREFIX(status_code_untyped) * code1,
const WG14_RESULT_PREFIX(status_code_untyped) * code2);
static WG14_RESULT_PREFIX(status_code_generic)
status_code_generic_domain_vtable_generic_code(
const WG14_RESULT_PREFIX(status_code_untyped) * code);
static WG14_RESULT_PREFIX(status_code_domain_string_ref)
status_code_generic_domain_vtable_message(
const WG14_RESULT_PREFIX(status_code_untyped) * code);

static const char *
generic_code_message(enum WG14_RESULT_PREFIX(status_code_errc) code)
{
  switch(code)
  {
  case WG14_RESULT_PREFIX(status_code_errc_success):
    return "Success";
  case WG14_RESULT_PREFIX(status_code_errc_address_family_not_supported):
    return "Address family not supported by protocol";
  case WG14_RESULT_PREFIX(status_code_errc_address_in_use):
    return "Address already in use";
  case WG14_RESULT_PREFIX(status_code_errc_address_not_available):
    return "Cannot assign requested address";
  case WG14_RESULT_PREFIX(status_code_errc_already_connected):
    return "Transport endpoint is already connected";
  case WG14_RESULT_PREFIX(status_code_errc_argument_list_too_long):
    return "Argument list too long";
  case WG14_RESULT_PREFIX(status_code_errc_argument_out_of_domain):
    return "Numerical argument out of domain";
  case WG14_RESULT_PREFIX(status_code_errc_bad_address):
    return "Bad address";
  case WG14_RESULT_PREFIX(status_code_errc_bad_file_descriptor):
    return "Bad file descriptor";
  case WG14_RESULT_PREFIX(status_code_errc_bad_message):
    return "Bad message";
  case WG14_RESULT_PREFIX(status_code_errc_broken_pipe):
    return "Broken pipe";
  case WG14_RESULT_PREFIX(status_code_errc_connection_aborted):
    return "Software caused connection abort";
  case WG14_RESULT_PREFIX(status_code_errc_connection_already_in_progress):
    return "Operation already in progress";
  case WG14_RESULT_PREFIX(status_code_errc_connection_refused):
    return "Connection refused";
  case WG14_RESULT_PREFIX(status_code_errc_connection_reset):
    return "Connection reset by peer";
  case WG14_RESULT_PREFIX(status_code_errc_cross_device_link):
    return "Invalid cross-device link";
  case WG14_RESULT_PREFIX(status_code_errc_destination_address_required):
    return "Destination address required";
  case WG14_RESULT_PREFIX(status_code_errc_device_or_resource_busy):
    return "Device or resource busy";
  case WG14_RESULT_PREFIX(status_code_errc_directory_not_empty):
    return "Directory not empty";
  case WG14_RESULT_PREFIX(status_code_errc_executable_format_error):
    return "Exec format error";
  case WG14_RESULT_PREFIX(status_code_errc_file_exists):
    return "File exists";
  case WG14_RESULT_PREFIX(status_code_errc_file_too_large):
    return "File too large";
  case WG14_RESULT_PREFIX(status_code_errc_filename_too_long):
    return "File name too long";
  case WG14_RESULT_PREFIX(status_code_errc_function_not_supported):
    return "Function not implemented";
  case WG14_RESULT_PREFIX(status_code_errc_host_unreachable):
    return "No route to host";
  case WG14_RESULT_PREFIX(status_code_errc_identifier_removed):
    return "Identifier removed";
  case WG14_RESULT_PREFIX(status_code_errc_illegal_byte_sequence):
    return "Invalid or incomplete multibyte or wide character";
  case WG14_RESULT_PREFIX(status_code_errc_inappropriate_io_control_operation):
    return "Inappropriate ioctl for device";
  case WG14_RESULT_PREFIX(status_code_errc_interrupted):
    return "Interrupted system call";
  case WG14_RESULT_PREFIX(status_code_errc_invalid_argument):
    return "Invalid argument";
  case WG14_RESULT_PREFIX(status_code_errc_invalid_seek):
    return "Illegal seek";
  case WG14_RESULT_PREFIX(status_code_errc_io_error):
    return "Input/output error";
  case WG14_RESULT_PREFIX(status_code_errc_is_a_directory):
    return "Is a directory";
  case WG14_RESULT_PREFIX(status_code_errc_message_size):
    return "Message too long";
  case WG14_RESULT_PREFIX(status_code_errc_network_down):
    return "Network is down";
  case WG14_RESULT_PREFIX(status_code_errc_network_reset):
    return "Network dropped connection on reset";
  case WG14_RESULT_PREFIX(status_code_errc_network_unreachable):
    return "Network is unreachable";
  case WG14_RESULT_PREFIX(status_code_errc_no_buffer_space):
    return "No buffer space available";
  case WG14_RESULT_PREFIX(status_code_errc_no_child_process):
    return "No child processes";
  case WG14_RESULT_PREFIX(status_code_errc_no_link):
    return "Link has been severed";
  case WG14_RESULT_PREFIX(status_code_errc_no_lock_available):
    return "No locks available";
  case WG14_RESULT_PREFIX(status_code_errc_no_message):
    return "No message of desired type";
  case WG14_RESULT_PREFIX(status_code_errc_no_protocol_option):
    return "Protocol not available";
  case WG14_RESULT_PREFIX(status_code_errc_no_space_on_device):
    return "No space left on device";
  case WG14_RESULT_PREFIX(status_code_errc_no_stream_resources):
    return "Out of streams resources";
  case WG14_RESULT_PREFIX(status_code_errc_no_such_device_or_address):
    return "No such device or address";
  case WG14_RESULT_PREFIX(status_code_errc_no_such_device):
    return "No such device";
  case WG14_RESULT_PREFIX(status_code_errc_no_such_file_or_directory):
    return "No such file or directory";
  case WG14_RESULT_PREFIX(status_code_errc_no_such_process):
    return "No such process";
  case WG14_RESULT_PREFIX(status_code_errc_not_a_directory):
    return "Not a directory";
  case WG14_RESULT_PREFIX(status_code_errc_not_a_socket):
    return "Socket operation on non-socket";
  case WG14_RESULT_PREFIX(status_code_errc_not_a_stream):
    return "Device not a stream";
  case WG14_RESULT_PREFIX(status_code_errc_not_connected):
    return "Transport endpoint is not connected";
  case WG14_RESULT_PREFIX(status_code_errc_not_enough_memory):
    return "Cannot allocate memory";
#if ENOTSUP != EOPNOTSUPP
  case WG14_RESULT_PREFIX(status_code_errc_not_supported):
    return "Operation not supported";
#endif
  case WG14_RESULT_PREFIX(status_code_errc_operation_canceled):
    return "Operation canceled";
  case WG14_RESULT_PREFIX(status_code_errc_operation_in_progress):
    return "Operation now in progress";
  case WG14_RESULT_PREFIX(status_code_errc_operation_not_permitted):
    return "Operation not permitted";
  case WG14_RESULT_PREFIX(status_code_errc_operation_not_supported):
    return "Operation not supported";
#if EAGAIN != EWOULDBLOCK
  case WG14_RESULT_PREFIX(status_code_errc_operation_would_block):
    return "Resource temporarily unavailable";
#endif
  case WG14_RESULT_PREFIX(status_code_errc_owner_dead):
    return "Owner died";
  case WG14_RESULT_PREFIX(status_code_errc_permission_denied):
    return "Permission denied";
  case WG14_RESULT_PREFIX(status_code_errc_protocol_error):
    return "Protocol error";
  case WG14_RESULT_PREFIX(status_code_errc_protocol_not_supported):
    return "Protocol not supported";
  case WG14_RESULT_PREFIX(status_code_errc_read_only_file_system):
    return "Read-only file system";
  case WG14_RESULT_PREFIX(status_code_errc_resource_deadlock_would_occur):
    return "Resource deadlock avoided";
  case WG14_RESULT_PREFIX(status_code_errc_resource_unavailable_try_again):
    return "Resource temporarily unavailable";
  case WG14_RESULT_PREFIX(status_code_errc_result_out_of_range):
    return "Numerical result out of range";
  case WG14_RESULT_PREFIX(status_code_errc_state_not_recoverable):
    return "State not recoverable";
  case WG14_RESULT_PREFIX(status_code_errc_stream_timeout):
    return "Timer expired";
  case WG14_RESULT_PREFIX(status_code_errc_text_file_busy):
    return "Text file busy";
  case WG14_RESULT_PREFIX(status_code_errc_timed_out):
    return "Connection timed out";
  case WG14_RESULT_PREFIX(status_code_errc_too_many_files_open_in_system):
    return "Too many open files in system";
  case WG14_RESULT_PREFIX(status_code_errc_too_many_files_open):
    return "Too many open files";
  case WG14_RESULT_PREFIX(status_code_errc_too_many_links):
    return "Too many links";
  case WG14_RESULT_PREFIX(status_code_errc_too_many_symbolic_link_levels):
    return "Too many levels of symbolic links";
  case WG14_RESULT_PREFIX(status_code_errc_value_too_large):
    return "Value too large for defined data type";
  case WG14_RESULT_PREFIX(status_code_errc_wrong_protocol_type):
    return "Protocol wrong type for socket";
  default:
    return "unknown";
  }
}

static WG14_RESULT_CONSTEXPR WG14_RESULT_PREFIX(status_code_domain_vtable)
status_code_generic_domain_vtable = {
.name = status_code_generic_domain_vtable_name,
.payload_info = status_code_generic_domain_vtable_payload_info,
.failure = status_code_generic_domain_vtable_failure,
.equivalent = status_code_generic_domain_vtable_equivalent,
.generic_code = status_code_generic_domain_vtable_generic_code,
.message = status_code_generic_domain_vtable_message,
.reserved_slot_for_cxx_throw_exception = WG14_RESULT_NULLPTR,
.erased_copy = WG14_RESULT_PREFIX(default_erased_copy_impl),
.erased_destroy = WG14_RESULT_PREFIX(default_erased_destroy_impl)  //
};

WG14_RESULT_PREFIX(status_code_domain) *
WG14_RESULT_PREFIX(status_code_generic_domain)(void)
{
  static WG14_RESULT_CONSTEXPR WG14_RESULT_PREFIX(status_code_domain)
  ret = {&status_code_generic_domain_vtable,
         0x746d6354f4f733e9 /* same as for C++ impl */};
  return &ret;
}

static WG14_RESULT_PREFIX(status_code_domain_string_ref)
status_code_generic_domain_vtable_name(void)
{
  return WG14_RESULT_PREFIX(status_code_domain_string_ref_from_static_string)(
  "generic domain");
}

static WG14_RESULT_PREFIX(status_code_domain_payload_info_t)
status_code_generic_domain_vtable_payload_info(void)
{
  WG14_RESULT_PREFIX(status_code_domain_payload_info_t)
  ret = {sizeof(enum WG14_RESULT_PREFIX(status_code_errc)),
         sizeof(WG14_RESULT_PREFIX(status_code_domain) *) +
         sizeof(enum WG14_RESULT_PREFIX(status_code_errc)),
         (__alignof(enum WG14_RESULT_PREFIX(status_code_errc)) >
          __alignof(WG14_RESULT_PREFIX(status_code_domain) *)) ?
         __alignof(enum WG14_RESULT_PREFIX(status_code_errc)) :
         __alignof(WG14_RESULT_PREFIX(status_code_domain) *)};
  return ret;
}

static bool status_code_generic_domain_vtable_failure(
const WG14_RESULT_PREFIX(status_code_untyped) * code)
{
  assert(code->domain == WG14_RESULT_PREFIX(status_code_generic_domain)());
  const WG14_RESULT_PREFIX(status_code_generic) *c =
  (const WG14_RESULT_PREFIX(status_code_generic) *) code;
  return c->value != WG14_RESULT_PREFIX(status_code_errc_success);  // NOLINT
}

static bool status_code_generic_domain_vtable_equivalent(
const WG14_RESULT_PREFIX(status_code_untyped) * code1,
const WG14_RESULT_PREFIX(status_code_untyped) * code2)
{
  assert(code1->domain == WG14_RESULT_PREFIX(status_code_generic_domain)());
  const WG14_RESULT_PREFIX(status_code_generic) *c1 =
  (const WG14_RESULT_PREFIX(status_code_generic) *) code1;
  if(code2->domain->id == code1->domain->id)
  {
    const WG14_RESULT_PREFIX(status_code_generic) *c2 =
    (const WG14_RESULT_PREFIX(status_code_generic) *) code2;
    return c1->value == c2->value;
  }
  return false;
}

static WG14_RESULT_PREFIX(status_code_generic)
status_code_generic_domain_vtable_generic_code(
const WG14_RESULT_PREFIX(status_code_untyped) * code)
{
  assert(code->domain == WG14_RESULT_PREFIX(status_code_generic_domain)());
  const WG14_RESULT_PREFIX(status_code_generic) *c =
  (const WG14_RESULT_PREFIX(status_code_generic) *) code;
  return *c;
}

static WG14_RESULT_PREFIX(status_code_domain_string_ref)
status_code_generic_domain_vtable_message(
const WG14_RESULT_PREFIX(status_code_untyped) * code)
{
  assert(code->domain == WG14_RESULT_PREFIX(status_code_generic_domain)());
  const WG14_RESULT_PREFIX(status_code_generic) *c =
  (const WG14_RESULT_PREFIX(status_code_generic) *) code;
  return WG14_RESULT_PREFIX(status_code_domain_string_ref_from_static_string)(
  generic_code_message(c->value));
}
