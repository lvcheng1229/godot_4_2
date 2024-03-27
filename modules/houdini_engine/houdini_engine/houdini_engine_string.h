#pragma once
#include "houdini_api.h"
#include "houdini_common.h"
#include "core/string/ustring.h"
#include "../houdini_engine.h"

class HoudiniEngineString {
public:
	HoudiniEngineString(int input_houdini_string_handle);
	void to_gd_string(CharString& string);

private:
	int houdini_string_handle;
};
