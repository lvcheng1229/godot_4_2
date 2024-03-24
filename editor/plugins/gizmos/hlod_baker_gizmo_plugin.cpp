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

#include "hlod_baker_gizmo_plugin.h"

#include "editor/editor_node.h"
#include "editor/editor_settings.h"
#include "editor/editor_string_names.h"
#include "editor/plugins/node_3d_editor_plugin.h"
#include "scene/3d/hlod_baker.h"

bool HLODBakerGizmoPlugin::has_gizmo(Node3D *p_spatial) {
	return Object::cast_to<HLODBaker>(p_spatial) != nullptr;
}

String HLODBakerGizmoPlugin::get_gizmo_name() const {
	return "HLODBaker";
}

int HLODBakerGizmoPlugin::get_priority() const {
	return -1;
}

void HLODBakerGizmoPlugin::redraw(EditorNode3DGizmo *p_gizmo) {
	Ref<Material> icon = get_material("hlod_baker_icon", p_gizmo);
	p_gizmo->clear();
	p_gizmo->add_unscaled_billboard(icon, 0.05);
}

HLODBakerGizmoPlugin::HLODBakerGizmoPlugin() {
	create_icon_material("hlod_baker_icon", EditorNode::get_singleton()->get_editor_theme()->get_icon(SNAME("GizmoHLODBaker"), EditorStringName(EditorIcons)));
}
