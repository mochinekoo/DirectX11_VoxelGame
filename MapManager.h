#pragma once
#include "MochinekoEngine/FBX.h"
#define MAX_MAP_SIZE 50

struct BlockData {
	FBX* fbx_ = nullptr;
};

enum class BlockType {
	AIR,
	BLOCK,
	BLOCK1
};

namespace {
	const float DRAW_BLOCK_SIZE = 2.0f;
	const float MAX_WORLD_DEPTH = -30.0f;
}

namespace MapManager {

	void Init();
	void Draw();
	
	void SetBlock(BlockType type, int x, int y, int z);
	bool IsHitBlock(float x, float y, float z);
}