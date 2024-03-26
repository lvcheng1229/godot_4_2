#pragma once

#include "core/object/ref_counted.h"
#include "houdini_engine\HAPI\HAPI.h"

class HoudiniEngine : public RefCounted {
	GDCLASS(HoudiniEngine, RefCounted);

public:
	static HoudiniEngine &get();
	const HAPI_Session *get_session();
	bool start_session(HAPI_Session *&SessionPtr);

	HoudiniEngine();

private:
	HAPI_Session session;
};
