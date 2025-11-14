%module wg14_result
%{
#include "wg14_result/all.h"
%}

%ignore abortf_impl;

// These must be kept in order of inclusion to work correctly
%include "wg14_result/config.h"
%include "wg14_result/status_code_domain.h"
%include "wg14_result/status_code_generic.h"
%include "wg14_result/status_code_system.h"
%include "wg14_result/status_code_posix.h"
%include "wg14_result/status_code.h"
%include "wg14_result/result.h"
%include "wg14_result/try.h"

