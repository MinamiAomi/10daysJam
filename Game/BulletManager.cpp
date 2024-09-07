#include "BulletManager.h"

void BulletManager::Initialize() {
	Reset();
}

void BulletManager::Update() {
	for (auto it = bullets_.begin(); it != bullets_.end(); ) {
		auto& bullet = *it;
		bullet->Update();

		// 弾が生存していない場合は削除
		if (!bullet->GetIsAlive()) {
			it = bullets_.erase(it); // イテレータを使って削除
		}
		else {
			++it; // 生存している場合は次の弾へ
		}
	}
}

void BulletManager::FireBullet(const Vector3& position, const Vector3& velocity, uint32_t attribute) {
	switch (attribute) {
	case CollisionAttribute::PlayerBullet:
	{
		auto bullet = std::make_shared<PlayerBullet>();
		bullet->Initialize(position, velocity);
		bullets_.emplace_back(bullet);

	}
	break;
	case CollisionAttribute::EnemyBullet:
	{
		auto bullet = std::make_shared<EnemyBullet>();
		bullet->Initialize(position, velocity);
		bullets_.emplace_back(bullet);
	}
	break;
	default:
		break;
	}
}

void BulletManager::RemoveBullet(std::shared_ptr<Bullet> bullet) {
	bullets_.erase(std::remove(bullets_.begin(), bullets_.end(), bullet), bullets_.end());
}

void BulletManager::Reset() {
	bullets_.clear();

}
