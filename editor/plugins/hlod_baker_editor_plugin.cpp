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
#include "hlod_baker_editor_plugin.h"

#include "editor/editor_node.h"
#include "editor/editor_string_names.h"
#include "editor/gui/editor_file_dialog.h"

void HLODBakerEditorPlugin::_bake() {

}

void HLODBakerEditorPlugin::_bind_methods() {
	ClassDB::bind_method("_bake", &HLODBakerEditorPlugin::_bake);
}

void HLODBakerEditorPlugin::edit(Object *p_object) {
	HLODBaker *s = Object::cast_to<HLODBaker>(p_object);
	if (!s) {
		return;
	}

	hlodbaker = s;
}

bool HLODBakerEditorPlugin::handles(Object *p_object) const {
	return p_object->is_class("HLODBaker");
}

void HLODBakerEditorPlugin::make_visible(bool p_visible) {
	if (p_visible) {
		bake->show();
	} else {
		bake->hide();
	}
}

HLODBakerEditorPlugin::HLODBakerEditorPlugin() {
	bake = memnew(Button);
	bake->set_theme_type_variation("FlatButton");
	bake->set_icon(EditorNode::get_singleton()->get_editor_theme()->get_icon(SNAME("Bake"), EditorStringName(EditorIcons)));
	bake->set_text(TTR("Bake HLOD"));
	bake->hide();
	bake->connect("pressed", Callable(this, "_bake"));
	add_control_to_container(CONTAINER_SPATIAL_EDITOR_MENU, bake);
}
