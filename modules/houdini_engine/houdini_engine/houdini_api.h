#pragma once
#include "HAPI\HAPI.h"
typedef HAPI_Result (*add_attribute_func_ptr)(const HAPI_Session *session, HAPI_NodeId node_id, HAPI_PartId part_id, const char *name, const HAPI_AttributeInfo *attr_info);
typedef HAPI_Result (*create_node_func_ptr)(const HAPI_Session *session, HAPI_NodeId parent_node_id, const char *operator_name, const char *node_label, HAPI_Bool cook_on_creation, HAPI_NodeId *new_node_id);
typedef HAPI_Result (*create_input_node_func_ptr)(const HAPI_Session *session, HAPI_NodeId *node_id, const char *name);
typedef HAPI_Result (*create_inprocess_session_func_ptr)(HAPI_Session *session);
typedef HAPI_Result (*initlialize_func_ptr)(const HAPI_Session *session, const HAPI_CookOptions *cook_options, HAPI_Bool use_cooking_thread, int cooking_thread_stack_size, const char *houdini_environment_files, const char *otl_search_path, const char *dso_search_path, const char *image_dso_search_path, const char *audio_dso_search_path);
typedef HAPI_Result (*connect_node_input_func_ptr)(const HAPI_Session *session, HAPI_NodeId node_id, int input_index, HAPI_NodeId node_id_to_connect, int output_index);
typedef HAPI_Result (*set_attribute_float_data_func_ptr)(const HAPI_Session *session, HAPI_NodeId node_id, HAPI_PartId part_id, const char *name, const HAPI_AttributeInfo *attr_info, const float *data_array, int start, int length);
typedef HAPI_Result (*set_vertex_list_func_ptr)(const HAPI_Session *session, HAPI_NodeId node_id, HAPI_PartId part_id, const int *vertex_list_array, int start, int length);
typedef HAPI_Result (*set_face_counts_func_ptr)(const HAPI_Session *session, HAPI_NodeId node_id, HAPI_PartId part_id, const int *face_counts_array, int start, int length);
typedef void (*partion_info_init_func_ptr)(HAPI_PartInfo *in);
typedef void (*attributeinfo_init_func_ptr)(HAPI_AttributeInfo *in);
typedef HAPI_Result (*commit_geo_func_ptr)(const HAPI_Session *session, HAPI_NodeId node_id);
typedef HAPI_Result (*load_asset_library_from_file_func_ptr)(const HAPI_Session *session, const char *file_path, HAPI_Bool allow_overwrite, HAPI_AssetLibraryId *library_id);
typedef HAPI_Result (*get_available_asset_count_func_ptr)(const HAPI_Session *session, HAPI_AssetLibraryId library_id, int *asset_count);
typedef HAPI_Result (*get_available_assets_func_ptr)(const HAPI_Session *session, HAPI_AssetLibraryId library_id, HAPI_StringHandle *asset_names_array, int asset_count);
typedef HAPI_Result (*get_string_buf_length_func_ptr)(const HAPI_Session *session, HAPI_StringHandle string_handle, int *buffer_length);
typedef HAPI_Result (*get_string_func_ptr)(const HAPI_Session *session, HAPI_StringHandle string_handle, char *string_value, int length);

typedef HAPI_Result (*cook_node_func_ptr)(const HAPI_Session *session, HAPI_NodeId node_id, const HAPI_CookOptions *cook_options);
typedef HAPI_Result (*get_status_func_ptr)(const HAPI_Session *session, HAPI_StatusType status_type, int *status);
typedef void (*cook_options_init_func_ptr)(HAPI_CookOptions *in);

typedef HAPI_Result (*start_thrift_named_pipe_server_func_ptr)(const HAPI_ThriftServerOptions *options, const char *pipe_name, HAPI_ProcessId *process_id, const char *log_file);
typedef HAPI_Result (*create_thrift_named_pipe_session_func_ptr)(HAPI_Session *session, const char *pipe_name);

typedef HAPI_Result (*get_status_string_func_ptr)(const HAPI_Session *session, HAPI_StatusType status_type, char *string_value, int length);
typedef HAPI_Result (*get_status_string_buf_length_func_ptr)(const HAPI_Session *session, HAPI_StatusType status_type, HAPI_StatusVerbosity verbosity, int *buffer_length);

typedef HAPI_Result (*get_connection_error_func_ptr )(char *string_value, int length, HAPI_Bool clear);
typedef HAPI_Result (*get_connection_error_length_func_ptr)(int *buffer_length);

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

	//typedef HAPI_Result (*set_vertex_list_func_ptr)(const HAPI_Session *session, HAPI_NodeId node_id, HAPI_PartId part_id, const int *vertex_list_array, int start, int length);
	static set_vertex_list_func_ptr set_vertex_list;

	//typedef HAPI_Result (*set_face_counts_func_ptr)(const HAPI_Session *session, HAPI_NodeId node_id, HAPI_PartId part_id, const int *face_counts_array, int start, int length);
	static set_face_counts_func_ptr set_face_counts;

	//typedef HAPI_Result (*commit_geo_func_ptr)(const HAPI_Session *session, HAPI_NodeId node_id);
	static commit_geo_func_ptr commit_geo;

	static partion_info_init_func_ptr partion_info_init;

	static attributeinfo_init_func_ptr attribute_info_init;

	static load_asset_library_from_file_func_ptr load_asset_library_from_file;

	static get_available_asset_count_func_ptr get_available_asset_count;
	static get_available_assets_func_ptr get_available_assets;

	static get_string_buf_length_func_ptr get_string_buf_length;
	static get_string_func_ptr get_string;

	static cook_node_func_ptr cook_node;
	static get_status_func_ptr get_status;
	static cook_options_init_func_ptr cook_options_init;

	static start_thrift_named_pipe_server_func_ptr start_thrift_named_pipe_server;
	static create_thrift_named_pipe_session_func_ptr create_thrift_named_pipe_session;
	static get_status_string_func_ptr get_status_string;
	static get_status_string_buf_length_func_ptr get_status_string_buf_length;

	static get_connection_error_func_ptr get_connection_error;
	static get_connection_error_length_func_ptr get_connection_error_length;
};
