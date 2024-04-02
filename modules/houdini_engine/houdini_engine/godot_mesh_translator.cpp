#include "godot_mesh_translator.h"
#include "houdini_api.h"
#include "houdini_common.h"
#include "houdini_engine_utils.h"
#include "../houdini_engine.h"

bool GoDotMeshTranslator::hapi_create_input_node_for_mesh(const Ref<Mesh> input_mesh, const Transform3D transform, HoudiniInputNode &houdini_node) {
	HAPI_NodeId node_id;

	HOUDINI_CHECK_ERROR_RETURN(HoudiniEngineUtils::create_input_node(node_id), false);
	houdini_node.set_input_node_id(node_id);

	// lightmapper.h struct MeshData
	struct HoudiniMeshData {
		Vector<Vector3> points;
		Vector<Vector3> uv; // uv.z == 0
		Vector<Vector3> normal;
	};

	Vector<int> indices;
	HoudiniMeshData houdini_mesh_data;

	//generate_triangle_mesh
	//LightmapGI::bake
	Basis normal_xform = transform.basis.inverse().transposed();
	int global_index = 0;
	for (int i = 0; i < input_mesh->get_surface_count(); i++) {
		if (input_mesh->surface_get_primitive_type(i) != Mesh::PRIMITIVE_TRIANGLES) {
			continue;
		}

		Array a = input_mesh -> surface_get_arrays(i);

		Vector<Vector3> vertices = a[Mesh::ARRAY_VERTEX];
		Vector<Vector2> uv = a[Mesh::ARRAY_TEX_UV];
		Vector<Vector3> normals = a[Mesh::ARRAY_NORMAL];

		Vector<int> index = a[Mesh::ARRAY_INDEX];

		ERR_CONTINUE(uv.size() == 0);
		ERR_CONTINUE(normals.size() == 0);

		const Vector3 *vr = vertices.ptr();
		const Vector2 *uvr = uv.ptr();
		const Vector3 *nr = normals.ptr();

		int facecount;
		const int *ir = nullptr;

		if (index.size()) {
			facecount = index.size() / 3;
			ir = index.ptr();
		} else {
			facecount = vertices.size() / 3;
		}

		for (int j = 0; j < facecount; j++) {
			uint32_t vidx[3];

			if (ir) {
				for (int k = 0; k < 3; k++) {
					vidx[k] = ir[j * 3 + k];
				}
			} else {
				for (int k = 0; k < 3; k++) {
					vidx[k] = j * 3 + k;
				}
			}

			for (int k = 0; k < 3; k++) {
				Vector3 v = transform.xform(vr[vidx[k]]);
				
				houdini_mesh_data.points.push_back(v);
				houdini_mesh_data.uv.push_back(Vector3(uvr[vidx[k]].x, uvr[vidx[k]].y, 0));
				houdini_mesh_data.normal.push_back(normal_xform.xform(nr[vidx[k]]).normalized());
			}

			indices.push_back(global_index + 0);
			indices.push_back(global_index + 1);
			indices.push_back(global_index + 2);
			global_index += 3;
		}
	}


	const uint32_t num_triangles = houdini_mesh_data.points.size() / 3;
	const uint32_t num_vertex_instances = houdini_mesh_data.points.size();
	const uint32_t num_vertices = num_vertex_instances;

	// Create part.
	HAPI_PartInfo part;
	HoudiniApi::partion_info_init(&part);
	part.id = 0;
	part.nameSH = 0;
	part.attributeCounts[HAPI_ATTROWNER_POINT] = 0;
	part.attributeCounts[HAPI_ATTROWNER_PRIM] = 0;
	part.attributeCounts[HAPI_ATTROWNER_VERTEX] = 0;
	part.attributeCounts[HAPI_ATTROWNER_DETAIL] = 0;
	part.vertexCount = num_vertex_instances; // index count
	part.faceCount = num_triangles;
	part.pointCount = num_vertices;
	part.type = HAPI_PARTTYPE_MESH;
	HOUDINI_CHECK_CALL_ERROR_RETURN(HoudiniApi::set_part_info(HoudiniEngine::get().get_session(), node_id, 0, &part), false);

	// Create point attribute info.
	{
		HAPI_AttributeInfo attribute_info_point;
		HoudiniApi::attribute_info_init(&attribute_info_point);
		attribute_info_point.count = part.pointCount; //TODO:!!!!!!!!!!!!!!!!
		attribute_info_point.tupleSize = 3;
		attribute_info_point.exists = true;
		attribute_info_point.owner = HAPI_ATTROWNER_POINT;
		attribute_info_point.storage = HAPI_STORAGETYPE_FLOAT;
		attribute_info_point.originalOwner = HAPI_ATTROWNER_INVALID;
		attribute_info_point.exists = true;
		HoudiniApi::attribute_add(HoudiniEngine::get().get_session(), node_id, 0, HAPI_ATTRIB_POSITION, &attribute_info_point);
		HOUDINI_CHECK_CALL_ERROR_RETURN(HoudiniApi::set_attribute_float_data(
				HoudiniEngine::get().get_session(),
				node_id, 0, HAPI_ATTRIB_POSITION,
				&attribute_info_point,
				(float *)houdini_mesh_data.points.ptrw(),
				0, attribute_info_point.count),false);
	}

	// Create attribute for normals.
	{
		HAPI_AttributeInfo attribute_info_normal;
		HoudiniApi::attribute_info_init(&attribute_info_normal);
		attribute_info_normal.tupleSize = 3;
		attribute_info_normal.count = houdini_mesh_data.normal.size();
		attribute_info_normal.exists = true;
		attribute_info_normal.owner = HAPI_ATTROWNER_VERTEX;
		attribute_info_normal.storage = HAPI_STORAGETYPE_FLOAT;
		attribute_info_normal.originalOwner = HAPI_ATTROWNER_INVALID;
		HoudiniApi::attribute_add(HoudiniEngine::get().get_session(), node_id, 0, HAPI_ATTRIB_NORMAL, &attribute_info_normal);
		HOUDINI_CHECK_CALL_ERROR_RETURN(HoudiniApi::set_attribute_float_data(
				HoudiniEngine::get().get_session(),
				node_id, 0, HAPI_ATTRIB_NORMAL,
				&attribute_info_normal,
				(float *)houdini_mesh_data.normal.ptr(),
										   0, attribute_info_normal.count),
				false);
	}

	// Create attribute for uvs.
	{
		HAPI_AttributeInfo attribute_info_uv;
		HoudiniApi::attribute_info_init(&attribute_info_uv);
		attribute_info_uv.count = houdini_mesh_data.uv.size();
		attribute_info_uv.tupleSize = 3;
		attribute_info_uv.exists = true;
		attribute_info_uv.owner = HAPI_ATTROWNER_VERTEX;
		attribute_info_uv.storage = HAPI_STORAGETYPE_FLOAT;
		attribute_info_uv.originalOwner = HAPI_ATTROWNER_INVALID;
		HoudiniApi::attribute_add(HoudiniEngine::get().get_session(), node_id, 0, HAPI_ATTRIB_UV, &attribute_info_uv);
		HOUDINI_CHECK_CALL_ERROR_RETURN(HoudiniApi::set_attribute_float_data(
				HoudiniEngine::get().get_session(),
				node_id, 0, HAPI_ATTRIB_UV,
				&attribute_info_uv,
				(float *)houdini_mesh_data.uv.ptr(),
				0, attribute_info_uv.count),false);
	}

	HOUDINI_CHECK_CALL_ERROR_RETURN(HoudiniApi::set_vertex_list(HoudiniEngine::get().get_session(), node_id, 0, indices.ptr(), 0, indices.size()), false);

	Vector<int> face_array;
	face_array.resize(part.faceCount);

	for (int index = 0; index < face_array.size();index++) {
		face_array.set(index, 3);
	}

	HOUDINI_CHECK_CALL_ERROR_RETURN(HoudiniApi::set_face_counts(HoudiniEngine::get().get_session(), node_id, 0, face_array.ptr(), 0, face_array.size()), false);
	HOUDINI_CHECK_CALL_ERROR_RETURN(HoudiniApi::commit_geo(HoudiniEngine::get().get_session(), node_id), false);
	return true;
}
