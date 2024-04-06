#include "../houdini_engine.h"
#include "houdini_input_tranlator.h"
#include "houdini_engine_utils.h"
#include "HAPI\HAPI.h"
#include "houdini_api.h"

//bool HoudiniInputTranslator::upload_changed_inputs(HoudiniAsset *houdini_asset) {
//
//	return false;
//}

__pragma(optimize("", off))

bool HoudiniInputTranslator::upload_input_data(HoudiniInput *houdini_input) {
	const Vector<HoudiniInputNode> *input_node_array_ptr = houdini_input->get_houdini_input_array(houdini_input->get_houdini_input_type());
	const Vector<HoudiniInputNode> &input_node_array = *input_node_array_ptr;

	HAPI_NodeId input_node_id = houdini_input->get_input_node_id();
	if (input_node_id < 0) {
		HoudiniEngineUtils::create_node(houdini_input->get_parent_node_id(), String("merge"), false, &input_node_id);
		//HoudiniEngineUtils::create_node(-1, String("SOP/merge"), false, &input_node_id);
		houdini_input->set_input_node_id(input_node_id);
	}

	HAPI_NodeInfo node_info = HoudiniApi::nodeinfo_create();
	HoudiniApi::get_node_info(HoudiniEngine::get().get_session(), input_node_id, &node_info);

	// Connect all the input objects to the merge node now
	int input_index = 200;
	for (int index = 0; index < input_node_array.size(); index++)
	{
		int current_node_id = input_node_array[index].get_input_node_id();
		if (current_node_id == input_node_id) {
			continue;
		}

		const HAPI_Result result_connect = HoudiniApi::connect_node_input(HoudiniEngine::get().get_session(), input_node_id, input_index, current_node_id, 0);
		input_index++;
	}
	input_index++;

	connect_input_node(houdini_input);
	return true;
}

bool HoudiniInputTranslator::connect_input_node(HoudiniInput *houdini_input) {

	HAPI_NodeId hda_node_id = houdini_input->get_hda_node_id();
	if (hda_node_id < 0) {
		WARN_PRINT("hda_node_id < 0");
		return false;
	}

	HAPI_NodeId input_node_id = houdini_input->get_input_node_id();
	if (input_node_id < 0) {
		WARN_PRINT("input_node_id < 0");
		return false;
	}

	HOUDINI_CHECK_ERROR_RETURN(HoudiniApi::connect_node_input(HoudiniEngine::get().get_session(), hda_node_id, 0, input_node_id, 0), false);
	return true;
}
