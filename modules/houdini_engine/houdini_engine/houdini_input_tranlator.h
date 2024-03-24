#pragma once
#include "houdini_common.h"
#include "../houdini_engine_runtime/houdini_input.h"
#include "../houdini_engine_runtime/houdini_asset.h"
struct HoudiniInputTranslator {
	static bool upload_changed_inputs(HoudiniAsset *houdini_asset);
	static bool upload_input_data(HoudiniInput *houdini_input);
	static bool connect_input_node(HoudiniInput* houdini_input);// Connect an input's nodes to its linked HDA node
};
