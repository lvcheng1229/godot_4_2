#include "houdini_input.h"

HoudiniInput::HoudiniInput() :
		type(HoudiniInputType::HTP_NONE),
		input_node_id (-1),
		hda_node_id(-1) {

}

Vector<HoudiniInputNode> *HoudiniInput::get_houdini_input_array(HoudiniInputType houdini_input_type) {
	if (houdini_input_type == HoudiniInputType::HTP_GEOMETRY) {
		return &geometry_input_nodes;
	}
	return nullptr;
}
