#pragma once
#include "scene/resources/mesh.h"
#include "scene/3d/hlod_baker.h"

class HLODMeshSimplifier : public IHLODMeshSimplifier {
public:
	void init();
	virtual void hlod_mesh_simplify(const Vector<HlodInputMesh> &input_meshs) override;
private:
	int hda_lib_id;
};





