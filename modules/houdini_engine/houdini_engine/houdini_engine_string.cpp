#include "houdini_engine_string.h"

HoudiniEngineString::HoudiniEngineString(int input_houdini_string_handle) :
		houdini_string_handle(input_houdini_string_handle) {
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
