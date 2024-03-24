/* register_types.cpp */

#include "register_types.h"

#include "core/object/class_db.h"
#include "houdini_engine.h"


static HoudiniEngine *houdini__engine;

void initialize_houdini_engine_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	ClassDB::register_class<HoudiniEngine>();

	houdini__engine = new HoudiniEngine();
	houdini__engine->houdini_engine_instance = houdini__engine;
}

void uninitialize_houdini_engine_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	delete houdini__engine;
}
