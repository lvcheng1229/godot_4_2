/* register_types.cpp */

#include "register_types.h"

#include "core/object/class_db.h"
#include "houdini_engine.h"

static HoudiniEngine *houdini_engine = nullptr;



HoudiniEngine &HoudiniEngine::get() {
	return *houdini_engine;
}

void initialize_houdini_engine_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	ClassDB::register_class<HoudiniEngine>();

	houdini_engine = new HoudiniEngine();

	
}

void uninitialize_houdini_engine_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	delete houdini_engine;
}
