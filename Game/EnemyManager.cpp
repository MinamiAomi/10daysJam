#include "EnemyManager.h"

void EnemyManager::Initialize() {
	Reset();
}

void EnemyManager::Update() {
	time_--;
	if (time_ <= 0) {
		Create();
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
	auto enemy = std::make_shared<Enemy>();
	Vector3 position(rnd_.NextFloatRange(-18.0f, 18.0f), -30.0f, 0.0f);
	enemy->Initialize(position);
	enemies_.emplace_back(enemy);
	time_ = interval_;
}
