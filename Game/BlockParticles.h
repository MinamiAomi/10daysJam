#pragma once
#include "Engine/GameObject/GameObject.h"

#include <memory>
#include <array>
#include "Math/Transform.h"
#include "Math/Random.h"
#include "Graphics/Model.h"
#include "Graphics/Skeleton.h"

#include "Engine/Collision/Collider.h"
#include "Game/BlockParticle.h"
class Player;
class BlockParticles {
public:
	static const uint32_t kEmitOneBlockPaticleNum = 64;
	static const uint32_t kBlockParticleNum = kEmitOneBlockPaticleNum * 64;
	void Initialize(Player* player);
	void Emit(const Vector3& position);
	void Update();
private:
	Player* player_ = nullptr;
	Random::RandomNumberGenerator rand_;
	Vector3 randomPosition_[kEmitOneBlockPaticleNum];
	std::array<BlockParticle, kBlockParticleNum> blockParticles_;
};