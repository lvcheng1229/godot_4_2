#include "houdini_engine_utils.h"
#include "../houdini_engine.h"
#include "core/os/os.h"

//temporaty code
void *HoudiniEngineUtils::load_lib_api() {
	String houdini_localtion = String(("C:\\Program Files\\Side Effects Software\\Houdini 19.5.303\\bin\\libHAPIL.dll"));//HoudiniLocation = FString::Printf(TEXT("C:\\Program Files\\Side Effects Software\\Houdini %s\\%s"), *HoudiniVersionString, HAPI_HFS_SUBFOLDER_WINDOWS);
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

HAPI_CookOptions HoudiniEngineUtils::get_default_cook_options() {
	HAPI_CookOptions cook_options;
	HoudiniApi::cook_options_init(&cook_options);
	cook_options.curveRefineLOD = 8.0f;
	cook_options.clearErrorsAndWarnings = false;
	cook_options.maxVerticesPerPrimitive = 3;
	cook_options.splitGeosByGroup = false;
	cook_options.splitGeosByAttribute = false;
	cook_options.splitAttrSH = 0;
	cook_options.refineCurveToLinear = true;
	cook_options.handleBoxPartTypes = false;
	cook_options.handleSpherePartTypes = false;
	cook_options.splitPointsByVertexAttributes = false;
	cook_options.packedPrimInstancingMode = HAPI_PACKEDPRIM_INSTANCING_MODE_FLAT;
	cook_options.cookTemplatedGeos = true;
	return cook_options;
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

bool HoudiniEngineUtils::hapi_cook_node(const HAPI_NodeId &in_node_id, HAPI_CookOptions *in_cook_options, const bool &wait_for_completion) {
	if (in_node_id < 0)
		return false;

	if (in_cook_options == nullptr) {
		HAPI_CookOptions cook_options = HoudiniEngineUtils::get_default_cook_options();
		HOUDINI_CHECK_ERROR_RETURN(HoudiniApi::cook_node(HoudiniEngine::get().get_session(), in_node_id, &cook_options), false);
	} else {
		HOUDINI_CHECK_ERROR_RETURN(HoudiniApi::cook_node(HoudiniEngine::get().get_session(), in_node_id, in_cook_options), false);
	}

	if (!wait_for_completion)
		return true;

	// Wait for the cook to finish
	HAPI_Result result = HAPI_RESULT_SUCCESS;
	while (true) {
		int status = HAPI_STATE_STARTING_COOK;
		HOUDINI_CHECK_ERROR_RETURN(HoudiniApi::get_status(HoudiniEngine::get().get_session(), HAPI_STATUS_COOK_STATE, &status), false);

		if (status == HAPI_STATE_READY) {
			return true;
		} else if (status == HAPI_STATE_READY_WITH_FATAL_ERRORS || status == HAPI_STATE_READY_WITH_COOK_ERRORS) {
			return false;
		}
	}
}
