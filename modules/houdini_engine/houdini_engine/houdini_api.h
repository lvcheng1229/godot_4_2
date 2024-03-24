#pragma once
#include "HAPI\HAPI.h"
typedef HAPI_Result (*add_attribute_func_ptr)(const HAPI_Session *session, HAPI_NodeId node_id, HAPI_PartId part_id, const char *name, const HAPI_AttributeInfo *attr_info);
typedef HAPI_Result (*create_node_func_ptr)(const HAPI_Session *session, HAPI_NodeId parent_node_id, const char *operator_name, const char *node_label, HAPI_Bool cook_on_creation, HAPI_NodeId *new_node_id);
typedef HAPI_Result (*create_input_node_func_ptr)(const HAPI_Session *session, HAPI_NodeId *node_id, const char *name);
typedef HAPI_Result (*create_inprocess_session_func_ptr)(HAPI_Session *session);
typedef HAPI_Result (*initlialize_func_ptr)(const HAPI_Session *session, const HAPI_CookOptions *cook_options, HAPI_Bool use_cooking_thread, int cooking_thread_stack_size, const char *houdini_environment_files, const char *otl_search_path, const char *dso_search_path, const char *image_dso_search_path, const char *audio_dso_search_path);
typedef HAPI_Result (*connect_node_input_func_ptr)(const HAPI_Session *session, HAPI_NodeId node_id, int input_index, HAPI_NodeId node_id_to_connect, int output_index);
typedef HAPI_Result (*set_attribute_float_data_func_ptr)(const HAPI_Session *session, HAPI_NodeId node_id, HAPI_PartId part_id, const char *name, const HAPI_AttributeInfo *attr_info, const float *data_array, int start, int length);
typedef void (*partion_info_init_func_ptr)(HAPI_PartInfo *in);
typedef void (*attributeinfo_init_func_ptr)(HAPI_AttributeInfo *in);

struct HoudiniApi {
public:
	static void initialize_hapi(void* houdini_handle);
public:
	//typedef void (*attributeinfo_init_func_ptr)(HAPI_AttributeInfo *in);
	static add_attribute_func_ptr attribute_add;

	static create_node_func_ptr create_node;
	static create_input_node_func_ptr create_input_node;
	static create_inprocess_session_func_ptr create_inprocess_session;
	static initlialize_func_ptr initlialize;
	static connect_node_input_func_ptr connect_node_input;

	
	//typedef HAPI_Result (*set_attribute_float_data_func_ptr)(const HAPI_Session *session, HAPI_NodeId node_id, HAPI_PartId part_id, const char *name, const HAPI_AttributeInfo *attr_info, const float *data_array, int start, int length);
	static set_attribute_float_data_func_ptr set_attribute_float_data;

	static partion_info_init_func_ptr partion_info_init;

	static attributeinfo_init_func_ptr attribute_info_init;
};
