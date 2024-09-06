#include "PlayerBulletManager.h"

void PlayerBulletManager::Initialize() {
    Reset();
}

void PlayerBulletManager::Update() {
    for (auto it = playerBullets_.begin(); it != playerBullets_.end(); ) {
        auto& bullet = *it;
        bullet->Update();

        // 弾が生存していない場合は削除
        if (!bullet->GetIsAlive()) {
            it = playerBullets_.erase(it); // イテレータを使って削除
        }
        else {
            ++it; // 生存している場合は次の弾へ
        }
    }
}

void PlayerBulletManager::FireBullet(const Vector3& position, const Vector3& velocity) {
    auto bullet = std::make_shared<PlayerBullet>();
    bullet->Initialize(position, velocity);
    playerBullets_.emplace_back(bullet);
}

void PlayerBulletManager::RemoveBullet(std::shared_ptr<PlayerBullet> bullet) {
    playerBullets_.erase(std::remove(playerBullets_.begin(), playerBullets_.end(), bullet), playerBullets_.end());
}

void PlayerBulletManager::Reset() {
    playerBullets_.clear();

}
