#include "houdini_engine.h"
#include "houdini_engine/houdini_engine_utils.h"
#include "houdini_engine/houdini_api.h"
#include "houdini_engine/houdini_common.h"
#include "pcg_pipeline/hlod_mesh_simplify.h"

static IHLODMeshSimplifier *create_hloa_simplifier() {
	HLODMeshSimplifier *mesh_simplifier = memnew(HLODMeshSimplifier);
	mesh_simplifier->init();
	return mesh_simplifier;
}

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

	// Try to connect to an existing pipe session first
	HAPI_Result session_result = HoudiniApi::create_thrift_named_pipe_session(&session, "hapi");

	if (session_result != HAPI_Result::HAPI_RESULT_SUCCESS)
	{
		HAPI_ThriftServerOptions serverOptions{ 0 };
		serverOptions.autoClose = true;
		serverOptions.timeoutMs = 3000.0f;
		HoudiniApi::start_thrift_named_pipe_server(&serverOptions, "hapi", nullptr, nullptr);
		session_result = HoudiniApi::create_thrift_named_pipe_session(&session, "hapi");
	}

	if (session_result != HAPI_Result::HAPI_RESULT_SUCCESS)
	{
		HoudiniEngineString connect_string(HoudiniStringType::Connect_Error_String);
		CharString error_string;    
		connect_string.get_last_connect_error_gd_string(error_string);
		WARN_PRINT(error_string);
	}


	//HOUDINI_CHECK_ERROR_RETURN(HoudiniApi::start_thrift_named_pipe_server(&serverOptions, "hapi", nullptr, nullptr),);
	//HOUDINI_CHECK_ERROR_RETURN(HoudiniApi::create_thrift_named_pipe_session(&session, "hapi"), );
	//HoudiniApi::create_inprocess_session(&session);
	HAPI_Result Result = HoudiniApi::initlialize(get_session(), &options, false, -1, "", "", "", "", "");
	if (Result == HAPI_RESULT_ALREADY_INITIALIZED) {
		WARN_PRINT("Successfully intialized the Houdini Engine module - HAPI was already initialzed.");
	} else if (Result != HAPI_RESULT_SUCCESS) {
		WARN_PRINT("Houdini Engine API initialization failed");
	}

	IHLODMeshSimplifier::create_hlod_baker = create_hloa_simplifier;
}
