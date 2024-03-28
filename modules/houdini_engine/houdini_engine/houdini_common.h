#pragma once
#include "core/error/error_macros.h"
#include "HAPI\HAPI.h"
#include "houdini_engine_string.h"

#define HOUDINI_CHECK_CALL_ERROR_RETURN(houdini_func_call, return_value)            \
	{                                                                               \
		HAPI_Result result = houdini_func_call;                                     \
		if (result != HAPI_RESULT_SUCCESS) {                                        \
			HoudiniEngineString houdini_engine_str(HoudiniStringType::Call_String); \
			CharString error_string;                                                \
			houdini_engine_str.get_last_error_gd_string(error_string);              \
			WARN_PRINT(error_string);                                               \
			return return_value;                                                    \
		}                                                                           \
	}\


#define HOUDINI_CHECK_ERROR_RETURN(houdini_func_call, return_value) \
	{                                                               \
		HAPI_Result result = houdini_func_call;                     \
		if (result != HAPI_RESULT_SUCCESS) {                        \
			WARN_PRINT(#houdini_func_call);                         \
			return return_value;                                    \
		}                                                           \
	}\
