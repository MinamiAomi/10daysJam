#include "BlockManager.h"

#include "GameProperty.h"

#include "BlockParticles.h"

void BlockManager::Initialize(BlockParticles* blockParticles) {
	blockParticles_ = blockParticles;
	//Reset();
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

void BlockManager::Create(const Vector3& position, const BlockType::Type& type) {
	switch (type) {
	case BlockType::kNormal:
	{
		auto block = std::make_shared<NormalBlock>();
		block->Initialize(position);
		block->SetBlockParticles(blockParticles_);
		blocks_.emplace_back(block);

	}
	break;
	case BlockType::kExplosion:
	{
		auto block = std::make_shared<ExplosionBlock>();
		block->Initialize(position);
		blocks_.emplace_back(block);
	}
	break;
	default:
		break;
	}
}

void BlockManager::Reset() {
	blocks_.clear();
  //float distance = 2.0f;
	//float startX = 0.0f;
	//uint32_t num = GameProperty::MaxBlockRow;
	//// 奇数or偶数
	//if (num % 2 == 0) {
	//	startX = -(float(GameProperty::MaxBlockRow) * 0.5f - 0.5f) * distance;
	//}
	//else {
	//	startX = -(float(GameProperty::MaxBlockRow) * 0.5f) * distance;
	//}
	//for (uint32_t y = 0; y < GameProperty::MaxBlockColumn; y++) {
	//	for (uint32_t x = 0; x < GameProperty::MaxBlockRow; x++) {
	//		Vector3 position(startX + x * distance, y * -distance, 0.0f);
	//		Create(position, BlockType::kNormal);
	//	}
	//}

}
