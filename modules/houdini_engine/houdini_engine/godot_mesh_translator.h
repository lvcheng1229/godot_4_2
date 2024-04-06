#pragma once
#include "scene/resources/mesh.h"
#include "../houdini_engine_runtime/houdini_input_node.h"

struct GoDotMeshTranslator {
public:
	static bool hapi_create_input_node_for_mesh(const Ref<Mesh> input_mesh, const Transform3D transform, Vector<HoudiniInputNode> *houdini_nodes, int index, const int parent_node_id);
};
