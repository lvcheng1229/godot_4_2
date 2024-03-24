#include "houdini_engine_utils.h"
#include "../houdini_engine.h"
#include "core/os/os.h"

//temporaty code
void *HoudiniEngineUtils::load_lib_api() {
	String houdini_localtion = String(("G:\\Houdini20\\bin\\libHAPIL.DLL"));//HoudiniLocation = FString::Printf(TEXT("C:\\Program Files\\Side Effects Software\\Houdini %s\\%s"), *HoudiniVersionString, HAPI_HFS_SUBFOLDER_WINDOWS);
	void *library = nullptr; 
	OS::get_singleton()->open_dynamic_library(houdini_localtion, library, true, nullptr);
	return library;
}

HAPI_Result HoudiniEngineUtils::create_node(
	const HAPI_NodeId &input_parent_node_id,
	const String &input_operator_name,
	const HAPI_Bool &is_cook_on_creation,
	HAPI_NodeId *out_new_node_id) {
	const HAPI_Session *session_ptr = HoudiniEngine::get().get_session();
	const HAPI_Result result = HoudiniApi::create_node(session_ptr, input_parent_node_id, input_operator_name.ascii().get_data(), nullptr, is_cook_on_creation, out_new_node_id);
	return result;
}

HAPI_Result HoudiniEngineUtils::create_input_node(const String &input_node_label, HAPI_NodeId &out_node_id) {
	const HAPI_Session *session_ptr = HoudiniEngine::get().get_session();
	const HAPI_Result result = HoudiniApi::create_input_node(session_ptr, &out_node_id, input_node_label.ascii().get_data());
	return result;
}

HAPI_Result HoudiniEngineUtils::create_input_node(HAPI_NodeId &out_node_id) {
	const HAPI_Session *session_ptr = HoudiniEngine::get().get_session();
	const HAPI_Result result = HoudiniApi::create_input_node(session_ptr, &out_node_id, nullptr);
	return result;
}
