#include "houdini_engine_string.h"
#include "houdini_common.h"

HoudiniEngineString::HoudiniEngineString(int input_houdini_string_handle) :
		houdini_string_handle(input_houdini_string_handle) {
}

HoudiniEngineString::HoudiniEngineString(HoudiniStringType input_houdini_string_type) :
		houdini_string_type(input_houdini_string_type) {
}

void HoudiniEngineString::to_gd_string(CharString& string) {
	if (houdini_string_handle == 0) {
		return;
	}

	int buffer_length;
	HOUDINI_CHECK_ERROR_RETURN(HoudiniApi::get_string_buf_length(HoudiniEngine::get().get_session(), houdini_string_handle, &buffer_length));
	
	char *buffer = new char[buffer_length];
	HoudiniApi::get_string(HoudiniEngine::get().get_session(), houdini_string_handle, buffer, buffer_length);
	string = CharString(buffer);
	delete[] buffer;
}

void HoudiniEngineString::get_last_error_gd_string(CharString &string) {

	HAPI_StatusType status_type;
	if (houdini_string_type == HoudiniStringType::Call_String)
	{
		status_type = HAPI_STATUS_CALL_RESULT;
	} else if (houdini_string_type == HoudiniStringType::Cook_Result_String)
	{
		status_type = HAPI_STATUS_COOK_RESULT;
	} else if (houdini_string_type == HoudiniStringType::Cook_State_String) {
		status_type = HAPI_STATUS_COOK_STATE;
	} else {
		return;
	}

	int buffer_length;
	HOUDINI_CHECK_ERROR_RETURN(HoudiniApi::get_status_string_buf_length(HoudiniEngine::get().get_session(), status_type, HAPI_STATUSVERBOSITY_ERRORS, &buffer_length));

	char *buffer = new char[buffer_length];
	HoudiniApi::get_status_string(HoudiniEngine::get().get_session(), status_type, buffer, buffer_length);
	string = CharString(buffer);
	delete[] buffer;
}

void HoudiniEngineString::get_last_connect_error_gd_string(CharString &string) {
	int buffer_length;
	HoudiniApi::get_connection_error_length(&buffer_length);

	char *buffer = new char[buffer_length];
	HoudiniApi::get_connection_error(buffer, buffer_length, true);
	string = CharString(buffer);
	delete[] buffer;
}
