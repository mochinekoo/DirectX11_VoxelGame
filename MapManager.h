#pragma once
#include "MochinekoEngine/FBX.h"

struct BlockData {
	FBX* fbx_ = nullptr;
};

enum class BlockType {
	AIR,
	BLOCK,
	BLOCK1
};

namespace MapManager {

	void Init();
	void Draw();
	
	void SetBlock(BlockType type, int x, int y, int z);
}