#include "houdini_engine.h"
#include "houdini_engine/houdini_engine_utils.h"
#include "houdini_engine/houdini_api.h"

HoudiniEngine &HoudiniEngine::get() {
	return *houdini_engine_instance;
}

const HAPI_Session *HoudiniEngine::get_session() {
	return &session;
}

bool HoudiniEngine::start_session(HAPI_Session *&SessionPtr) {
	HAPI_CookOptions options;
	HoudiniApi::create_inprocess_session(SessionPtr);
	HoudiniApi::initlialize(SessionPtr, &options, false, -1, "", "", "", "", "");
	return true;
}

HoudiniEngine::HoudiniEngine() {
	void *hapi_library_handle = HoudiniEngineUtils::load_lib_api();
	HoudiniApi::initialize_hapi(hapi_library_handle);
	HAPI_Session* session_ptr = &session;
	start_session(session_ptr);
}
