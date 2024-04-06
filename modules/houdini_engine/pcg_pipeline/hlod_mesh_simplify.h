#pragma once
#include "scene/resources/mesh.h"
#include "scene/3d/hlod_baker.h"

class HLODMeshSimplifier : public IHLODMeshSimplifier {
public:
	void init();
	void get_output_mesh(int node_id, HlodSimplifiedMesh &hlod_mesh_simplified);
	virtual void hlod_mesh_simplify(const Vector<HlodInputMesh> &input_meshs, HlodSimplifiedMesh &hlod_mesh_simplified) override;

private:
	int hda_lib_id;
};





