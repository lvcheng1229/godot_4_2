/***************************************************************************
MIT License

Copyright(c) 2023 lvchengTSH

Permission is hereby granted, free of charge, to any person obtaining a copy
of this softwareand associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright noticeand this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
***************************************************************************/

#include "hlod_baker.h"
#include "scene/resources/surface_tool.h"
#include "editor/editor_node.h"

IHLODMeshSimplifier::CreateFunc IHLODMeshSimplifier::create_hlod_baker = nullptr;
IHLODTextureGenerator::CreateFunc IHLODTextureGenerator::create_hlod_tex_generator = nullptr;

Ref<IHLODMeshSimplifier> IHLODMeshSimplifier::create() {
	if (create_hlod_baker) {
		return Ref<IHLODMeshSimplifier>(create_hlod_baker());
	}
	return Ref<IHLODMeshSimplifier>();
}

Ref<IHLODTextureGenerator> IHLODTextureGenerator::create() {
	if (create_hlod_tex_generator) {
		return Ref<IHLODTextureGenerator>(create_hlod_tex_generator());
	}
	return Ref<IHLODTextureGenerator>();
}



bool HLODBaker::bake(Node *p_from_node) {

	Ref<IHLODMeshSimplifier> hlod_mesh_simplifier = IHLODMeshSimplifier::create();
	Vector<HlodInputMesh> hlod_input_meshs;
	find_meshs(p_from_node, hlod_input_meshs);

	HlodSimplifiedMesh hlod_mesh_simplified;
	hlod_mesh_simplifier->hlod_mesh_simplify(hlod_input_meshs, hlod_mesh_simplified);

	Ref<IHLODTextureGenerator> hlod_tex_generator = IHLODTextureGenerator::create();
	hlod_tex_generator->hlod_mesh_texture_generate(hlod_input_meshs, hlod_mesh_simplified);

	Ref<SurfaceTool> surftool;
	surftool.instantiate();
	surftool->begin(Mesh::PRIMITIVE_TRIANGLES);

	for (int index = 0; index < hlod_mesh_simplified.points.size(); index++)
	{
		surftool->set_normal(hlod_mesh_simplified.normal.get(index));

		Vector2 uv = Vector2(hlod_mesh_simplified.uv.get(index).x, hlod_mesh_simplified.uv.get(index).y);
		surftool->set_uv(uv);

		surftool->add_vertex(hlod_mesh_simplified.points.get(index));
	}

	for (int index = 0; index < hlod_mesh_simplified.indices.size(); index++)
	{
		surftool->add_index(hlod_mesh_simplified.indices.get(index));
	}


	Ref<ArrayMesh>  array_mesh = surftool->commit();
	MeshInstance3D *mesh_instance = memnew(MeshInstance3D);
	mesh_instance->set_mesh(array_mesh);
	mesh_instance->set_name("hlod_generated");

	Node *current_edited_scene_root = EditorNode::get_singleton()->get_edited_scene();
	current_edited_scene_root->add_child(mesh_instance);
	mesh_instance->set_owner(current_edited_scene_root);

	return false;
}

HLODBaker::HLODBaker() {
}

void HLODBaker::find_meshs(Node *p_from_node, Vector<HlodInputMesh> &hlod_meshs_found) {
	MeshInstance3D *mi = Object::cast_to<MeshInstance3D>(p_from_node);

	//if (mi && mi->get_gi_mode() == GeometryInstance3D::GI_MODE_STATIC && mi->is_visible_in_tree()) {
	if (mi) {
		Ref<Mesh> mesh = mi->get_mesh();
		if (mesh.is_valid()) {
			bool surfaces_found = false;

			bool found_uv = false;
			bool found_normal = false;
			bool found_tangent = false;

			for (int i = 0; i < mesh->get_surface_count(); i++) {
				if (mesh->surface_get_primitive_type(i) != Mesh::PRIMITIVE_TRIANGLES) {
					continue;
				}
				if (mesh->surface_get_format(i) & Mesh::ARRAY_FORMAT_TEX_UV) {
					found_uv = true;
				}
				if (mesh->surface_get_format(i) & Mesh::ARRAY_FORMAT_NORMAL) {
					found_normal = true;
				}
				if (mesh->surface_get_format(i) & Mesh::ARRAY_FORMAT_TANGENT) {
					found_tangent = true;
				}
			}

			surfaces_found = found_uv && found_normal && found_tangent;

			if (surfaces_found) {
				HlodInputMesh hlod_input_mesh;
				hlod_input_mesh.xform = get_global_transform().affine_inverse() * mi->get_global_transform();
				hlod_input_mesh.mesh = mesh;
				hlod_meshs_found.push_back(hlod_input_mesh);
			}
		}
	}

	for (int i = 0; i < p_from_node->get_child_count(); i++) {
		Node *child = p_from_node->get_child(i);
		if (!child->get_owner()) {
			continue; //maybe a helper
		}

		find_meshs(child, hlod_meshs_found);
	}
}

