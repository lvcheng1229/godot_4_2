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
		Vector<Vector2> uv = a[Mesh::ARRAY_TEX_UV2];
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
				indices.push_back(global_index + 0);
				indices.push_back(global_index + 1);
				indices.push_back(global_index + 2);
				global_index+=3;
			}
		}
	}

	// Create part.
	HAPI_PartInfo part;
	HoudiniApi::partion_info_init(&part);


	part.id = 0;
	part.nameSH = 0;
	part.attributeCounts[HAPI_ATTROWNER_POINT] = 0;
	part.attributeCounts[HAPI_ATTROWNER_PRIM] = 0;
	part.attributeCounts[HAPI_ATTROWNER_VERTEX] = 0;
	part.attributeCounts[HAPI_ATTROWNER_DETAIL] = 0;
	part.vertexCount = NumVertexInstances;
	part.faceCount = NumTriangles;
	part.pointCount = NumVertices;
	part.type = HAPI_PARTTYPE_MESH;

	// Create point attribute info.
	HAPI_AttributeInfo attribute_info_point;
	HoudiniApi::attribute_info_init(&attribute_info_point);
	attribute_info_point.count = part.pointCount;
	attribute_info_point.tupleSize = 3;
	attribute_info_point.exists = true;
	attribute_info_point.owner = HAPI_ATTROWNER_POINT;
	attribute_info_point.storage = HAPI_STORAGETYPE_FLOAT;
	attribute_info_point.originalOwner = HAPI_ATTROWNER_INVALID;
	HoudiniApi::attribute_add(HoudiniEngine::get().get_session(), houdini_node.get_input_node_id(), 0, HAPI_ATTRIB_POSITION, &attribute_info_point);



	return true;
}
