#include "hlod_mesh_simplify.h"
#include "../houdini_engine/godot_mesh_translator.h"
#include "../houdini_engine/houdini_input_tranlator.h"
#include "../houdini_engine/houdini_api.h"
#include "../houdini_engine.h"
#include "../houdini_engine/houdini_engine_string.h"
#include "../houdini_engine/houdini_engine_utils.h"
#include "editor/editor_settings.h"

__pragma(optimize("", off))

void HLODMeshSimplifier::hlod_mesh_simplify(const Vector<HlodInputMesh> &input_meshs, HlodSimplifiedMesh &hlod_mesh_simplified) {

	HoudiniInput houdini_input;
	// set hda node
	int asset_count;
	HOUDINI_CHECK_CALL_ERROR_RETURN(HoudiniApi::get_available_asset_count(HoudiniEngine::get().get_session(), hda_lib_id, &asset_count));

	if (asset_count > 1) {
		return;
	}

	HAPI_StringHandle asset_string_handle;
	HOUDINI_CHECK_CALL_ERROR_RETURN(HoudiniApi::get_available_assets(HoudiniEngine::get().get_session(), hda_lib_id, &asset_string_handle, asset_count), );

	HAPI_NodeId hda_node_id;
	HoudiniEngineString asset_str(asset_string_handle);
	CharString char_asset_str;
	asset_str.to_gd_string(char_asset_str);
	HoudiniEngineUtils::create_node(-1, char_asset_str.get_data(), false, &hda_node_id);
	houdini_input.set_hda_node_id(hda_node_id);

	{

		HAPI_NodeInfo node_info = HoudiniApi::nodeinfo_create();
		HoudiniApi::get_node_info(HoudiniEngine::get().get_session(), hda_node_id, &node_info);
		houdini_input.set_parent_node_id(node_info.parentId);
	}

	// Create houdini input geos
	houdini_input.set_houdini_input_type(HoudiniInputType::HTP_GEOMETRY);
	Vector<HoudiniInputNode> *houdini_input_geo_nodes_ptr = houdini_input.get_houdini_input_array(HoudiniInputType::HTP_GEOMETRY);
	houdini_input_geo_nodes_ptr->resize(input_meshs.size());

	// Add mesh
	for (int index = 0; index < input_meshs.size();index++) {
		GoDotMeshTranslator::hapi_create_input_node_for_mesh(input_meshs.get(index).mesh, input_meshs.get(index).xform, houdini_input_geo_nodes_ptr, index, houdini_input.get_parent_node_id());
	}

	// create merge node :set_input_node_id
	// connect to hda node : get_sop_input_index
	HoudiniInputTranslator::upload_input_data(&houdini_input);

	HoudiniEngineUtils::hapi_cook_node(houdini_input.get_hda_node_id(), nullptr, true);

	get_output_mesh(houdini_input.get_hda_node_id(), hlod_mesh_simplified);
}

void HLODMeshSimplifier::init() {
	String hda_path = EDITOR_GET("filesystem/houdini/HoudiniModulePath");
	hda_path += "/hda/godot_mesh_simplify6.hda";
	HoudiniApi::load_asset_library_from_file(HoudiniEngine::get().get_session(), hda_path.ascii().get_data(), true, &hda_lib_id);
}

void HLODMeshSimplifier::get_output_mesh(int node_id, HlodSimplifiedMesh &hlod_mesh_simplified) {

	HAPI_GeoInfo geo_info;
	HoudiniApi::get_geo_info(HoudiniEngine::get().get_session(), node_id, &geo_info);

	for (int part_index = 0; part_index < geo_info.partCount; ++part_index)
	{
		HAPI_PartInfo part_info;
		HoudiniApi::get_part_info(HoudiniEngine::get().get_session(), node_id, part_index, &part_info);

		int *vertex_list = new int[part_info.faceCount * 3];
		float *pos_data;
		float *normal_data;
		float *uv_data;

		HoudiniApi::get_vertex_list(HoudiniEngine::get().get_session(), node_id, part_index, vertex_list, 0, part_info.faceCount * 3);

		// pos data
		HAPI_AttributeInfo pos_attribute;
		HAPI_AttributeInfo normal_attribute;
		HAPI_AttributeInfo uv_attribute;

		{
			HAPI_AttributeOwner owner;
			for (int attri_idx = 0; attri_idx < HAPI_ATTROWNER_MAX; ++attri_idx)
			{
				HoudiniApi::get_attribute_info(HoudiniEngine::get().get_session(), node_id, part_index, HAPI_ATTRIB_POSITION, HAPI_AttributeOwner(attri_idx), &pos_attribute);
				if (pos_attribute.exists)
				{
					owner = HAPI_AttributeOwner(attri_idx);
					break;
				}
			}

			pos_data = new float[pos_attribute.tupleSize * pos_attribute.count];
			HoudiniApi::get_attribute_float_data(HoudiniEngine::get().get_session(), node_id, part_index, HAPI_ATTRIB_POSITION, &pos_attribute, pos_attribute.tupleSize, pos_data, 0, pos_attribute.count);
		}

		// normal data
		{
			HAPI_AttributeOwner owner;
			for (int attri_idx = 0; attri_idx < HAPI_ATTROWNER_MAX; ++attri_idx) {
				HoudiniApi::get_attribute_info(HoudiniEngine::get().get_session(), node_id, part_index, HAPI_ATTRIB_NORMAL, HAPI_AttributeOwner(attri_idx), &normal_attribute);
				if (normal_attribute.exists) {
					owner = HAPI_AttributeOwner(attri_idx);
					break;
				}
			}

			normal_data = new float[normal_attribute.tupleSize * normal_attribute.count];
			HoudiniApi::get_attribute_float_data(HoudiniEngine::get().get_session(), node_id, part_index, HAPI_ATTRIB_NORMAL, &normal_attribute, normal_attribute.tupleSize, normal_data, 0, normal_attribute.count);
		}

		// uv data
		{
			HAPI_AttributeOwner owner;
			for (int attri_idx = 0; attri_idx < HAPI_ATTROWNER_MAX; ++attri_idx) {
				HoudiniApi::get_attribute_info(HoudiniEngine::get().get_session(), node_id, part_index, HAPI_ATTRIB_UV, HAPI_AttributeOwner(attri_idx), &uv_attribute);
				if (uv_attribute.exists) {
					owner = HAPI_AttributeOwner(attri_idx);
					break;
				}
			}

			uv_data = new float[uv_attribute.tupleSize * uv_attribute.count];
			HoudiniApi::get_attribute_float_data(HoudiniEngine::get().get_session(), node_id, part_index, HAPI_ATTRIB_UV, &uv_attribute, uv_attribute.tupleSize, uv_data, 0, uv_attribute.count);
		}

		hlod_mesh_simplified.indices.resize(part_info.faceCount * 3);

		for (int face_index = 0; face_index < part_info.faceCount; face_index++)
		{
			hlod_mesh_simplified.indices.set(face_index * 3 + 0, face_index * 3 + 0);
			hlod_mesh_simplified.indices.set(face_index * 3 + 1, face_index * 3 + 1);
			hlod_mesh_simplified.indices.set(face_index * 3 + 2, face_index * 3 + 2);
		}

		hlod_mesh_simplified.points.resize(part_info.faceCount * 3);
		hlod_mesh_simplified.normal.resize(part_info.faceCount * 3);
		hlod_mesh_simplified.uv.resize(part_info.faceCount * 3);
		for (int face_index = 0; face_index < part_info.faceCount; face_index++)
		{
			int index_a = vertex_list[face_index * 3 + 0];
			int index_b = vertex_list[face_index * 3 + 1];
			int index_c = vertex_list[face_index * 3 + 2];

			hlod_mesh_simplified.points.set(face_index * 3 + 0, Vector3(pos_data[index_a * pos_attribute.tupleSize + 0], pos_data[index_a * pos_attribute.tupleSize + 1], pos_data[index_a * pos_attribute.tupleSize + 2]));
			hlod_mesh_simplified.points.set(face_index * 3 + 1, Vector3(pos_data[index_b * pos_attribute.tupleSize + 0], pos_data[index_b * pos_attribute.tupleSize + 1], pos_data[index_b * pos_attribute.tupleSize + 2]));
			hlod_mesh_simplified.points.set(face_index * 3 + 2, Vector3(pos_data[index_c * pos_attribute.tupleSize + 0], pos_data[index_c * pos_attribute.tupleSize + 1], pos_data[index_c * pos_attribute.tupleSize + 2]));


			hlod_mesh_simplified.normal.set(face_index * 3 + 0, Vector3(normal_data[index_a * normal_attribute.tupleSize + 0], normal_data[index_a * normal_attribute.tupleSize + 1], normal_data[index_a * normal_attribute.tupleSize + 2]));
			hlod_mesh_simplified.normal.set(face_index * 3 + 1, Vector3(normal_data[index_b * normal_attribute.tupleSize + 0], normal_data[index_b * normal_attribute.tupleSize + 1], normal_data[index_b * normal_attribute.tupleSize + 2]));
			hlod_mesh_simplified.normal.set(face_index * 3 + 2, Vector3(normal_data[index_c * normal_attribute.tupleSize + 0], normal_data[index_c * normal_attribute.tupleSize + 1], normal_data[index_c * normal_attribute.tupleSize + 2]));

			hlod_mesh_simplified.uv.set(face_index * 3 + 0, Vector3(uv_data[index_a * uv_attribute.tupleSize + 0], uv_data[index_a * uv_attribute.tupleSize + 1], uv_data[index_a * uv_attribute.tupleSize + 2]));
			hlod_mesh_simplified.uv.set(face_index * 3 + 1, Vector3(uv_data[index_b * uv_attribute.tupleSize + 0], uv_data[index_b * uv_attribute.tupleSize + 1], uv_data[index_b * uv_attribute.tupleSize + 2]));
			hlod_mesh_simplified.uv.set(face_index * 3 + 2, Vector3(uv_data[index_c * uv_attribute.tupleSize + 0], uv_data[index_c * uv_attribute.tupleSize + 1], uv_data[index_c * uv_attribute.tupleSize + 2]));
		}

		delete vertex_list;
		delete pos_data;
		delete normal_data;
		delete uv_data;
	}
	
}
