#pragma once
#include "core/templates/local_vector.h"
#include "houdini_input_node.h"
#include "houdini_engine_runtime_common.h"
class HoudiniInput{
public:
	HoudiniInput();

	Vector<HoudiniInputNode> *get_houdini_input_array(HoudiniInputType houdini_input_type);

	inline void set_houdini_input_type(HoudiniInputType input_type) { type = input_type; };
	inline HoudiniInputType get_houdini_input_type() { return type; };

	inline int get_input_node_id() const { return input_node_id; };
	inline void set_input_node_id(const int &new_node_id) { input_node_id = new_node_id; };

	inline int get_hda_node_id() const { return hda_node_id; }
	inline void set_hda_node_id(int new_node_id) { hda_node_id = new_node_id; }

	inline int get_sop_input_index() const { return sop_input_index; };
	inline void set_sop_input_index(int new_sop_input_index)  { sop_input_index = new_sop_input_index; }

private:

	HoudiniInputType type;
	Vector<HoudiniInputNode> geometry_input_nodes;

	// NodeId of the created input node
	// when there is multiple inputs objects, this will be the merge node.
	int input_node_id;

	// houdini asset node id
	int hda_node_id;

	int sop_input_index;
};
