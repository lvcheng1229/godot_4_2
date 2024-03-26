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


void HoudiniApi::initialize_hapi(void *houdini_handle) {
	void *create_node_ptr = &create_node;
	void *create_input_node_ptr = &create_input_node;
	void *create_inprocess_session_ptr = &create_inprocess_session;
	void *initlialize_ptr = &initlialize;
	void *connect_node_input_ptr = &connect_node_input;
	void *partion_info_init_ptr = &partion_info_init;
	void *attribute_info_init_ptr = &attribute_info_init;
	void *attribute_add_ptr = &attribute_add;
	void *set_attribute_float_data_ptr = &set_attribute_float_data;
	void *set_vertex_list_ptr = &set_vertex_list;
	void *set_face_counts_ptr = &set_face_counts;
	void *commit_geo_ptr = &commit_geo;
	void *load_asset_library_from_file_ptr = &load_asset_library_from_file;

	OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_CreateNode", create_node_ptr);
	OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_CreateInputNode", create_input_node_ptr);
	OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_CreateInProcessSession", create_inprocess_session_ptr);
	OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_Initialize", initlialize_ptr);
	OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_ConnectNodeInput", connect_node_input_ptr);
	OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_PartInfo_Init", partion_info_init_ptr);
	OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_AttributeInfo_Init", attribute_info_init_ptr);
	OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_AddAttribute", attribute_add_ptr);
	OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_SetAttributeFloatData", set_attribute_float_data_ptr);
	OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_SetVertexList", set_vertex_list_ptr);
	OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_SetFaceCounts", set_face_counts_ptr);
	OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_CommitGeo", commit_geo_ptr);
	OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_LoadAssetLibraryFromFile", load_asset_library_from_file_ptr);
}
