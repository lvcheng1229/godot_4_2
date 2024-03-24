#include "houdini_api.h"
#include "core/os/os.h"

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

	OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_CreateNode", create_node_ptr);
	OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_CreateInputNode", create_input_node_ptr);
	OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_CreateInProcessSession", create_inprocess_session_ptr);
	OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_Initialize", initlialize_ptr);
	OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_ConnectNodeInput", connect_node_input_ptr);
	OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_PartInfo_Init", partion_info_init_ptr);
	OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_AttributeInfo_Init", attribute_info_init_ptr);
	OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_AddAttribute", attribute_add_ptr);
	OS::get_singleton()->get_dynamic_library_symbol_handle(houdini_handle, "HAPI_SetAttributeFloatData", set_attribute_float_data_ptr);
}
