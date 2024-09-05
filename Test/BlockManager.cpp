#include "BlockManager.h"

const uint32_t BlockManager::kMaxBlockCount = 40;

void BlockManager::Initialize() {
	blocks_.clear();

	float distance = 2.0f;
	float startX = 0.0f;
	uint32_t num = kMaxBlockCount;
	// 奇数or偶数
	if (num % 2 == 0) {
		startX = -(float(kMaxBlockCount) * 0.5f - 0.5f) * distance;
	}
	else {
		startX = -(float(kMaxBlockCount) * 0.5f) * distance;
	}

	for (uint32_t i = 0; i < kMaxBlockCount; i++) {
		auto block = std::make_shared<Block>();
		Vector3 position(startX + i * distance, 0.0f, 0.0f);
		block->Initialize(position);
		blocks_.emplace_back(block);
	}
}

void BlockManager::Update() {
	for (auto& block : blocks_) {
		block->Update();
	}
}
