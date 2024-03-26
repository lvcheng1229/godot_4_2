#pragma once
#include "scene/resources/mesh.h"

struct HlodInputMesh {
	Transform3D xform;
	Ref<Mesh> mesh;
};

class HLODMeshBuilder
{
public:
	void init();
	void hlod_mesh_simplify(const Vector<HlodInputMesh> &input_meshs);
private:
	int hda_lib_id;
};





