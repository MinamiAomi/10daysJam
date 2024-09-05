#include "BlockManager.h"

const uint32_t BlockManager::kMaxBlockCount = 20;

void BlockManager::Initialize() {
	Reset();
}

void BlockManager::Update() {
	for (auto it = blocks_.begin(); it != blocks_.end(); ) {
		auto& block = *it;
		block->Update();

		// 弾が生存していない場合は削除
		if (!block->GetIsAlive()) {
			it = blocks_.erase(it); // イテレータを使って削除
		}
		else {
			++it; // 生存している場合は次の弾へ
		}
	}
}

void BlockManager::Reset() {
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
