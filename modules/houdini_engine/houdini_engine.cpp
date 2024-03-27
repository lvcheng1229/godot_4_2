#include "houdini_engine.h"
#include "houdini_engine/houdini_engine_utils.h"
#include "houdini_engine/houdini_api.h"
#include "houdini_engine\houdini_common.h"


const HAPI_Session *HoudiniEngine::get_session() {
	return &session;
}

bool HoudiniEngine::start_session() {
	HAPI_CookOptions options;

	printf("debug 0");
	HoudiniApi::create_inprocess_session(&session);

	printf("debug 0");
	HoudiniApi::initlialize(&session, &options, false, -1, "", "", "", "", "");
	return true;
}

HoudiniEngine::HoudiniEngine() {
	void *hapi_library_handle = HoudiniEngineUtils::load_lib_api();
	HoudiniApi::initialize_hapi(hapi_library_handle);

	HAPI_CookOptions options;

	printf("debug 0");
	HoudiniApi::create_inprocess_session(&session);

	printf("debug 0");
	HoudiniApi::initlialize(&session, &options, false, -1, "", "", "", "", "");
}
