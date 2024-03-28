#include "houdini_engine.h"
#include "houdini_engine/houdini_engine_utils.h"
#include "houdini_engine/houdini_api.h"
#include "houdini_engine\houdini_common.h"

const HAPI_Session *HoudiniEngine::get_session() {
	return &session;
	//return nullptr;
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

	HAPI_ThriftServerOptions serverOptions{ 0 };
	serverOptions.autoClose = true;
	serverOptions.timeoutMs = 3000.0f;
	HoudiniApi::start_thrift_named_pipe_server(&serverOptions, "hapi", nullptr, nullptr);
	HAPI_Result create_session_result = HoudiniApi::create_thrift_named_pipe_session(&session, "hapi");

	if (create_session_result != HAPI_Result::HAPI_RESULT_SUCCESS)
	{
		HoudiniEngineString connect_string(HoudiniStringType::Connect_Error_String);
		CharString error_string;    
		connect_string.get_last_connect_error_gd_string(error_string);
		WARN_PRINT(error_string);
	}


	//HOUDINI_CHECK_ERROR_RETURN(HoudiniApi::start_thrift_named_pipe_server(&serverOptions, "hapi", nullptr, nullptr),);
	//HOUDINI_CHECK_ERROR_RETURN(HoudiniApi::create_thrift_named_pipe_session(&session, "hapi"), );
	//HoudiniApi::create_inprocess_session(&session);
	HOUDINI_CHECK_CALL_ERROR_RETURN(HoudiniApi::initlialize(get_session(), &options, false, -1, "", "", "", "", ""),);
}
