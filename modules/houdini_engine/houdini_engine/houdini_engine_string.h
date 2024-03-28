#pragma once
#include "houdini_api.h"

#include "core/string/ustring.h"
#include "../houdini_engine.h"

enum class HoudiniStringType
{
	Cook_Result_String,
	Cook_State_String,
	Call_String,
	Connect_Error_String
};

class HoudiniEngineString {
public:
	HoudiniEngineString(int input_houdini_string_handle);
	HoudiniEngineString(HoudiniStringType input_houdini_string_type);
	void to_gd_string(CharString &string);
	void get_last_error_gd_string(CharString &string);
	void get_last_connect_error_gd_string(CharString &string);

private:
	int houdini_string_handle = -1;
	HoudiniStringType houdini_string_type;
};
