#pragma once
#include "MochinekoEngine/ModelManager.h"

namespace UserResource {

	inline int blockHandle_ = -1;

	inline void Init() {
		blockHandle_ = ModelManager::Load("block.fbx");
	}
}