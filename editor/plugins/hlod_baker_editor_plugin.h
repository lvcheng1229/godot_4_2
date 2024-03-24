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

#ifndef HLOD_BAKER_EDITOR_PLUGIN_H
#define HLOD_BAKER_EDITOR_PLUGIN_H

#include "editor/editor_plugin.h"
#include "scene/3d/hlod_baker.h"

struct EditorProgress;
class EditorFileDialog;

class HLODBakerEditorPlugin : public EditorPlugin {
	GDCLASS(HLODBakerEditorPlugin, EditorPlugin);

	HLODBaker *hlodbaker = nullptr;

	Button *bake = nullptr;

	void _bake();

protected:
	static void _bind_methods();

public:
	virtual String get_name() const override { return "HLODBaker"; }
	bool has_main_screen() const override { return false; }
	virtual void edit(Object *p_object) override;
	virtual bool handles(Object *p_object) const override;
	virtual void make_visible(bool p_visible) override;

	HLODBakerEditorPlugin();
};

#endif
