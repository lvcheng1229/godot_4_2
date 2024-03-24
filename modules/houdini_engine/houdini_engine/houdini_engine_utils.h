#pragma once
#include "core/string/ustring.h"
#include "HAPI/HAPI.h"
#include "houdini_api.h"

class HoudiniEngineUtils {
public:
	static void *load_lib_api();

	static HAPI_Result create_node(
			const HAPI_NodeId &input_parent_node_id,
			const String &input_operator_name,
			const HAPI_Bool &is_cook_on_creation,
			HAPI_NodeId *out_new_node_id);

	static HAPI_Result create_input_node(const String &input_node_label, HAPI_NodeId &out_node_id);
	static HAPI_Result create_input_node(HAPI_NodeId &out_node_id);
};
