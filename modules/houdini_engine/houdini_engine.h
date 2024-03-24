#pragma once

#include "core/object/ref_counted.h"
#include "houdini_engine\HAPI\HAPI.h"

class HoudiniEngine : public RefCounted {
	GDCLASS(HoudiniEngine, RefCounted);

public:
	static HoudiniEngine *houdini_engine_instance;
	static HAPI_Session session;
	static HoudiniEngine &get();
	static const HAPI_Session *get_session();

	bool start_session(HAPI_Session *&SessionPtr);

	HoudiniEngine();
};
