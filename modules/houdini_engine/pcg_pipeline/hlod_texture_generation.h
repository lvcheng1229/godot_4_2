#pragma once
#include "scene/3d/hlod_baker.h"
#include "scene/resources/mesh.h"
#include "hwrtl/hwrtl_lodtex.h"

class HLODTexGenerator : public IHLODTextureGenerator {
public:
	void init();
	void generate_array_mesh();
	virtual void hlod_mesh_texture_generate(const Vector<HlodInputMesh> &input_meshs, const HlodSimplifiedMesh &hlod_mesh_simplified, Ref<ArrayMesh>& out_mesh) override;
};
