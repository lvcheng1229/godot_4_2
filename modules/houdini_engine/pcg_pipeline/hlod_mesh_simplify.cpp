#include "hlod_mesh_simplify.h"
#include "../houdini_engine/godot_mesh_translator.h"
#include "../houdini_engine/houdini_input_tranlator.h"
#include "../houdini_engine/houdini_api.h"
#include "../houdini_engine.h"

void HLODMeshBuilder::hlod_mesh_simplify(const Vector<HlodInputMesh> &input_meshs) {

	// Create houdini input geos
	HoudiniInput houdini_input;
	houdini_input.set_houdini_input_type(HoudiniInputType::HTP_GEOMETRY);
	Vector<HoudiniInputNode> *houdini_input_geo_nodes_ptr = houdini_input.get_houdini_input_array(HoudiniInputType::HTP_GEOMETRY);
	Vector<HoudiniInputNode> &houdini_input_geo_nodes = *houdini_input_geo_nodes_ptr;
	houdini_input_geo_nodes.resize(input_meshs.size());

	// Add mesh
	for (int index = 0; index < input_meshs.size();index++) {
		GoDotMeshTranslator::hapi_create_input_node_for_mesh(input_meshs.get(index).mesh, input_meshs.get(index).xform, houdini_input_geo_nodes.get(index));
	}

	// set hda node
	houdini_input.set_hda_node_id(hda_lib_id);

	// create merge node :set_input_node_id
	// connect to hda node : get_sop_input_index
	HoudiniInputTranslator::upload_input_data(&houdini_input);
}

void HLODMeshBuilder::init() {
	String code_path(__FILE__);
	Vector<String> split_string = code_path.split("hlod_mesh_simplify.cpp");
	String hda_path = split_string.get(0) + String("hda\\godot_simplify_mesh.hda");
	HoudiniApi::load_asset_library_from_file(HoudiniEngine::get().get_session(), hda_path.ascii().get_data(), true, &hda_lib_id);
}
