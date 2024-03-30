/* register_types.cpp */

#include "register_types.h"

#include "core/object/class_db.h"
#include "pcg_pipeline/hlod_mesh_simplify.h"
#include "houdini_engine.h"

static HoudiniEngine *houdini_engine = nullptr;

static IHLODMeshSimplifier *create_hloa_simplifier() {
	HLODMeshSimplifier *mesh_simplifier = memnew(HLODMeshSimplifier);
	mesh_simplifier->init();
	return mesh_simplifier;
}

HoudiniEngine &HoudiniEngine::get() {
	return *houdini_engine;
}

void initialize_houdini_engine_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	ClassDB::register_class<HoudiniEngine>();

	houdini_engine = new HoudiniEngine();

	IHLODMeshSimplifier::create_hlod_baker = create_hloa_simplifier;
}

void uninitialize_houdini_engine_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	delete houdini_engine;
}
