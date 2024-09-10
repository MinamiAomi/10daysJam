#include "EnemyManager.h"

#include "GameProperty.h"

namespace {
	enum EnemyType {
		kNormal,
		kCount
	};
}

void EnemyManager::Initialize() {
	Reset();
}

void EnemyManager::Update() {
	time_--;
	if (time_ <= 0) {
		//Create();
	}
	for (auto it = enemies_.begin(); it != enemies_.end(); ) {
		auto& enemy = *it;
		enemy->Update();

		// 弾が生存していない場合は削除
		if (!enemy->GetIsAlive()) {
			it = enemies_.erase(it); // イテレータを使って削除
		}
		else {
			++it; // 生存している場合は次の弾へ
		}
	}
}

void EnemyManager::Reset() {
	enemies_.clear();
	time_ = interval_;
}

void EnemyManager::Create() {
	auto enemyType = rnd_.NextUIntRange(0, EnemyType::kCount - 1);

	switch (enemyType) {
	case EnemyType::kNormal:
	{
		auto enemy = std::make_shared<NormalEnemy>();
		Vector3 position(rnd_.NextFloatRange(-GameProperty::GameStageSize.x, GameProperty::GameStageSize.x), -GameProperty::GameStageSize.y, 0.0f);
		enemy->Initialize(position);
		enemy->SetBulletManager(bulletManager_);
		enemies_.emplace_back(enemy);

	}
	break;
	default:
		break;
	}
	// インターバルリセット
	time_ = interval_;
}
