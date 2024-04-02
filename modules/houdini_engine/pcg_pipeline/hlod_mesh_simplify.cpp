#include "hlod_mesh_simplify.h"
#include "../houdini_engine/godot_mesh_translator.h"
#include "../houdini_engine/houdini_input_tranlator.h"
#include "../houdini_engine/houdini_api.h"
#include "../houdini_engine.h"
#include "../houdini_engine/houdini_engine_string.h"
#include "../houdini_engine/houdini_engine_utils.h"
#include "editor/editor_settings.h"


void HLODMeshSimplifier::hlod_mesh_simplify(const Vector<HlodInputMesh> &input_meshs) {

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
	int asset_count;
	HOUDINI_CHECK_ERROR_RETURN(HoudiniApi::get_available_asset_count(HoudiniEngine::get().get_session(), hda_lib_id, &asset_count));

	if (asset_count > 1) {
		return;
	}

	HAPI_StringHandle asset_string_handle;
	HOUDINI_CHECK_ERROR_RETURN(HoudiniApi::get_available_assets(HoudiniEngine::get().get_session(), hda_lib_id, &asset_string_handle, asset_count),);

	HAPI_NodeId hda_node_id;
	HoudiniEngineString asset_str(asset_string_handle);
	CharString char_asset_str;
	asset_str.to_gd_string(char_asset_str);
	HOUDINI_CHECK_ERROR_RETURN(HoudiniEngineUtils::create_node(-1, char_asset_str.get_data(), false, &hda_node_id), );
	houdini_input.set_hda_node_id(hda_node_id);

	// create merge node :set_input_node_id
	// connect to hda node : get_sop_input_index
	HoudiniInputTranslator::upload_input_data(&houdini_input);

	HoudiniEngineUtils::hapi_cook_node(houdini_input.get_hda_node_id(), nullptr, true);
}

void HLODMeshSimplifier::init() {
	String hda_path = EDITOR_GET("filesystem/houdini/HoudiniHda");
	hda_path += "/godot_simplify_mesh.hda";
	HoudiniApi::load_asset_library_from_file(HoudiniEngine::get().get_session(), hda_path.ascii().get_data(), true, &hda_lib_id);
}
