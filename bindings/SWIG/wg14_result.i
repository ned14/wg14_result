%module wg14_result
%{
#include "wg14_result/all.h"
%}

%ignore abortf_impl;

// These must be kept in order of inclusion to work correctly
%include "wg14_result/config.h"
%include "wg14_result/status_code_domain.h"

%template(wg14_result_status_code_special_member_functions) wg14_result::status_code_special_member_functions< struct status_code_generic_s >;
%template(wg14_result_status_code_special_member_functions) wg14_result::status_code_special_member_functions< struct status_code_system_s >;
%template(wg14_result_status_code_special_member_functions) wg14_result::status_code_special_member_functions< struct status_code_posix_s >;

%include "wg14_result/status_code_generic.h"
%include "wg14_result/status_code_system.h"
%include "wg14_result/status_code_posix.h"
%include "wg14_result/status_code.h"

%template(wg14_result_special_member_functions) wg14_result::wg14_result_special_member_functions< result_with_void_s >;

%include "wg14_result/result.h"
%include "wg14_result/try.h"



