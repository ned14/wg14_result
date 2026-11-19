// The out of line definitions of the inline functions and of the `*_empty`
// objects the library exports. This is the only TU built with
// WG14_RESULT_SOURCE_INLINES, so each of those symbols is defined exactly once.
#include "../../include/wg14_result/all.h"

#ifdef __cplusplus
extern "C"
{
#endif

  WG14_RESULT_CONSTEXPR_OR_CONST
  WG14_RESULT_PREFIX(status_code_domain_string_ref)
  WG14_RESULT_PREFIX(status_code_domain_string_ref_empty) =
  WG14_RESULT_STATUS_CODE_DOMAIN_STRING_REF_EMPTY_INITIALISER;

  WG14_RESULT_CONSTEXPR_OR_CONST
  WG14_RESULT_PREFIX(status_code_generic)
  WG14_RESULT_PREFIX(status_code_generic_empty) =
  WG14_RESULT_STATUS_CODE_GENERIC_EMPTY_INITIALISER;

  WG14_RESULT_CONSTEXPR_OR_CONST
  WG14_RESULT_PREFIX(status_code_system)
  WG14_RESULT_PREFIX(status_code_system_empty) =
  WG14_RESULT_STATUS_CODE_SYSTEM_EMPTY_INITIALISER;

#ifdef __cplusplus
}
#endif
