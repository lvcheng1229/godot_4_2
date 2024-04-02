#include "houdini_api.h"
#include "core/os/os.h"

add_attribute_func_ptr HoudiniApi::attribute_add = nullptr;
create_node_func_ptr HoudiniApi::create_node = nullptr;
create_input_node_func_ptr HoudiniApi::create_input_node = nullptr;
create_inprocess_session_func_ptr HoudiniApi::create_inprocess_session = nullptr;
initlialize_func_ptr HoudiniApi::initlialize = nullptr;
connect_node_input_func_ptr HoudiniApi::connect_node_input = nullptr;
set_attribute_float_data_func_ptr HoudiniApi::set_attribute_float_data = nullptr;
set_vertex_list_func_ptr HoudiniApi::set_vertex_list = nullptr;
set_face_counts_func_ptr HoudiniApi::set_face_counts = nullptr;
commit_geo_func_ptr HoudiniApi::commit_geo = nullptr;
partion_info_init_func_ptr HoudiniApi::partion_info_init = nullptr;
attributeinfo_init_func_ptr HoudiniApi::attribute_info_init = nullptr;
load_asset_library_from_file_func_ptr HoudiniApi::load_asset_library_from_file = nullptr;
get_available_asset_count_func_ptr HoudiniApi::get_available_asset_count = nullptr;
get_available_assets_func_ptr HoudiniApi::get_available_assets = nullptr;
get_string_buf_length_func_ptr HoudiniApi::get_string_buf_length = nullptr;
get_string_func_ptr HoudiniApi::get_string = nullptr;
cook_node_func_ptr HoudiniApi::cook_node = nullptr;
get_status_func_ptr HoudiniApi::get_status = nullptr;
cook_options_init_func_ptr HoudiniApi::cook_options_init = nullptr;
start_thrift_named_pipe_server_func_ptr HoudiniApi::start_thrift_named_pipe_server = nullptr;
create_thrift_named_pipe_session_func_ptr HoudiniApi::create_thrift_named_pipe_session = nullptr;
get_status_string_func_ptr HoudiniApi::get_status_string = nullptr;
get_status_string_buf_length_func_ptr HoudiniApi::get_status_string_buf_length = nullptr;
get_connection_error_length_func_ptr HoudiniApi::get_connection_error_length = nullptr;
get_connection_error_func_ptr HoudiniApi::get_connection_error = nullptr;
set_part_info_func_ptr HoudiniApi::set_part_info = nullptr;

void valid_symbol(Error error) {
	if (error != OK) {
		ERR_PRINT("symbol not found in library ");
	};
}

void HoudiniApi::initialize_hapi(void *houdini_handle) {
	void *create_node_ptr;
	void *create_input_node_ptr;
	void *create_inprocess_session_ptr;
	void *initlialize_ptr;
	void *connect_node_input_ptr;
	void *partion_info_init_ptr;
	void *attribute_info_init_ptr;
	void *attribute_add_ptr;
	void *set_attribute_float_data_ptr;
	void *set_vertex_list_ptr;
	void *set_face_counts_ptr;
	void *commit_geo_ptr;
	void *load_asset_library_from_file_ptr;
	void *get_available_asset_count_ptr;
	void *get_available_assets_ptr;
	void *get_string_buf_length_ptr;
	void *get_string_ptr;
	void *cook_node_ptr;
	void *get_status_ptr;
	void *cook_options_init_ptr;
	void *start_thrift_named_pipe_server_ptr;
	void *create_thrift_named_pipe_session_ptr;
	void *get_status_string_ptr;
	void *get_status_string_buf_length_ptr;
	void *get_connection_error_length_ptr;
	void *get_connection_error_ptr;
	void *set_part_info_ptr;

	Error error;
	valid_symbol(OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_CreateNode", create_node_ptr));
	valid_symbol(OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_CreateInputNode", create_input_node_ptr));
	valid_symbol(OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_CreateInProcessSession", create_inprocess_session_ptr));
	valid_symbol(OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_Initialize", initlialize_ptr));
	valid_symbol(OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_ConnectNodeInput", connect_node_input_ptr));
	valid_symbol(OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_PartInfo_Init", partion_info_init_ptr));
	valid_symbol(OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_AttributeInfo_Init", attribute_info_init_ptr));
	valid_symbol(OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_AddAttribute", attribute_add_ptr));
	valid_symbol(OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_SetAttributeFloatData", set_attribute_float_data_ptr));
	valid_symbol(OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_SetVertexList", set_vertex_list_ptr));
	valid_symbol(OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_SetFaceCounts", set_face_counts_ptr));
	valid_symbol(OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_CommitGeo", commit_geo_ptr));
	valid_symbol(OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_LoadAssetLibraryFromFile", load_asset_library_from_file_ptr));
	valid_symbol(OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_GetAvailableAssetCount", get_available_asset_count_ptr));
	valid_symbol(OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_GetAvailableAssets", get_available_assets_ptr));
	valid_symbol(OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_GetStringBufLength", get_string_buf_length_ptr));
	valid_symbol(OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_GetString", get_string_ptr));
	valid_symbol(OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_CookNode", cook_node_ptr));
	valid_symbol(OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_GetStatus", get_status_ptr));
	valid_symbol(OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_CookOptions_Init", cook_options_init_ptr));
	valid_symbol(OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_StartThriftNamedPipeServer", start_thrift_named_pipe_server_ptr));
	valid_symbol(OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_CreateThriftNamedPipeSession", create_thrift_named_pipe_session_ptr));

	valid_symbol(OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_GetStatusString", get_status_string_ptr));
	valid_symbol(OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_GetStatusStringBufLength", get_status_string_buf_length_ptr));

	valid_symbol(OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_GetConnectionErrorLength", get_connection_error_length_ptr));
	valid_symbol(OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_GetConnectionError", get_connection_error_ptr));
	valid_symbol(OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_SetPartInfo", set_part_info_ptr));

	create_node = (create_node_func_ptr)create_node_ptr;
	create_input_node = (create_input_node_func_ptr)create_input_node_ptr;
	create_inprocess_session = (create_inprocess_session_func_ptr)create_inprocess_session_ptr;
	initlialize = (initlialize_func_ptr)initlialize_ptr;
	connect_node_input = (connect_node_input_func_ptr)connect_node_input_ptr;
	partion_info_init = (partion_info_init_func_ptr)partion_info_init_ptr;
	attribute_info_init = (attributeinfo_init_func_ptr)attribute_info_init_ptr;
	attribute_add = (add_attribute_func_ptr)attribute_add_ptr;
	set_attribute_float_data = (set_attribute_float_data_func_ptr)set_attribute_float_data_ptr;
	set_vertex_list = (set_vertex_list_func_ptr)set_vertex_list_ptr;
	set_face_counts = (set_face_counts_func_ptr)set_face_counts_ptr;
	commit_geo = (commit_geo_func_ptr)commit_geo_ptr;
	load_asset_library_from_file = (load_asset_library_from_file_func_ptr)load_asset_library_from_file_ptr;
	get_available_asset_count = (get_available_asset_count_func_ptr)get_available_asset_count_ptr;
	get_available_assets = (get_available_assets_func_ptr)get_available_assets_ptr;
	get_string_buf_length = (get_string_buf_length_func_ptr)get_string_buf_length_ptr;
	get_string = (get_string_func_ptr)get_string_ptr;
	cook_node = (cook_node_func_ptr)cook_node_ptr;
	get_status = (get_status_func_ptr)get_status_ptr;
	cook_options_init = (cook_options_init_func_ptr)cook_options_init_ptr;
	start_thrift_named_pipe_server = (start_thrift_named_pipe_server_func_ptr)start_thrift_named_pipe_server_ptr;
	create_thrift_named_pipe_session = (create_thrift_named_pipe_session_func_ptr)create_thrift_named_pipe_session_ptr;
	get_status_string = (get_status_string_func_ptr)get_status_string_ptr;
	get_status_string_buf_length = (get_status_string_buf_length_func_ptr)get_status_string_buf_length_ptr;
	get_connection_error_length = (get_connection_error_length_func_ptr)get_connection_error_length_ptr;
	get_connection_error = (get_connection_error_func_ptr)get_connection_error_ptr;
	set_part_info = (set_part_info_func_ptr)set_part_info_ptr;
}
