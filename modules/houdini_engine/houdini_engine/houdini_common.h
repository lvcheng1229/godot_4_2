#pragma once
#include "core/error/error_macros.h"
#include "HAPI\HAPI.h"

#define HOUDINI_CHECK_ERROR_RETURN(houdini_func_call,return_value)\
HAPI_Result result = houdini_func_call;\
if (result != HAPI_RESULT_SUCCESS) {\
WARN_PRINT(#houdini_func_call);\
return return_value;\
}\
