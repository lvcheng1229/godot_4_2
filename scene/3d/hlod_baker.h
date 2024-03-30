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

#ifndef HLOD_BAKER__H
#define HLOD_BAKER__H

#include "core/templates/local_vector.h"
#include "scene/3d/visual_instance_3d.h"
#include "scene/3d/mesh_instance_3d.h"

struct HlodInputMesh {
	Transform3D xform;
	Ref<Mesh> mesh;
};

class IHLODMeshSimplifier : public RefCounted {
	GDCLASS(IHLODMeshSimplifier, RefCounted)
public:
	typedef IHLODMeshSimplifier *(*CreateFunc)();

	static CreateFunc create_hlod_baker;

	static Ref<IHLODMeshSimplifier> create();
	virtual void hlod_mesh_simplify(const Vector<HlodInputMesh> &input_meshs) = 0;
};

class HLODBaker : public VisualInstance3D {
	GDCLASS(HLODBaker, VisualInstance3D);

public:
	bool bake(Node *p_from_node);
	HLODBaker();

private:
	void find_meshs(Node *p_from_node, Vector<HlodInputMesh> &hlod_meshs_found);

};

#endif


