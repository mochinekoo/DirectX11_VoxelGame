#include "MapManager.h"
#include <vector>
#include <array>

namespace {
	std::array<std::array<std::array<BlockData*, 50>, 50>, 50> blockMap_;

	FBX* blockFBX_ = nullptr;
	FBX* subBlockFBX_ = nullptr;
}

void MapManager::Init() {
	blockFBX_ = new FBX("block.fbx");
	subBlockFBX_ = new FBX("block1.fbx");
	blockFBX_->Init();
	subBlockFBX_->Init();

	SetBlock(BlockType::BLOCK, 0, 0, 0);
	SetBlock(BlockType::BLOCK1, 0, 0, 1);
	SetBlock(BlockType::BLOCK, 0, 0, 2);
	SetBlock(BlockType::BLOCK1, 0, 0, 3);
	SetBlock(BlockType::BLOCK, 0, 0, 4);
	SetBlock(BlockType::BLOCK1, 0, 0, 5);

	SetBlock(BlockType::BLOCK, 1, 0, 0);
	SetBlock(BlockType::BLOCK1, 2, 0, 0);
	SetBlock(BlockType::BLOCK, 3, 0, 0);
	SetBlock(BlockType::BLOCK1, 4, 0, 0);
	SetBlock(BlockType::BLOCK, 5, 0, 0);
}

void MapManager::Draw() {
	for (int z = 0; z < blockMap_.size(); z++) {
		for (int y = 0; y < blockMap_[z].size(); y++) {
			for (int x = 0; x < blockMap_[z][y].size(); x++) {
				const float blockSize = 2.0f;
				float setX = 0.0f + (blockSize * x);
				float setY = 0.0f + (blockSize * y);
				float setZ = 0.0f + (blockSize * z);

				BlockData* blockData = blockMap_[z][y][x];
				if (blockData != nullptr) {
					FBX* fbx = blockData->fbx_;

					fbx->SetLocation({ setX, setY, setZ });
					fbx->UpdateTransform();
					fbx->Update();

					fbx->Draw();
				}
			}
		}
	}
}

void MapManager::SetBlock(BlockType type, int x, int y, int z) {
	switch (type) {
	case BlockType::BLOCK: {
		BlockData* data = new BlockData();
		data->fbx_ = blockFBX_;
		blockMap_[z][y][x] = data;
		break;
	}
	case BlockType::BLOCK1: {
		BlockData* data = new BlockData();
		data->fbx_ = subBlockFBX_;
		blockMap_[z][y][x] = data;
		break;
	}
	default:
		blockMap_[z][y][x] = nullptr;
		break;
	}
}
