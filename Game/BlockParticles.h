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
class Map;
class BlockParticles {
public:
	static const uint32_t kEmitOneBlockPaticleNum = 20;
	static const uint32_t kBlockParticleNum = kEmitOneBlockPaticleNum * 128 * 2;
	void Initialize();
	void Emit(const Vector3& position);
	void Update();

	void Reset();

	void SetMap(const Map* map) { map_ = map; }
private:
	const Map* map_ = nullptr;
	Random::RandomNumberGenerator rnd_;
	std::array<BlockParticle, kBlockParticleNum> blockParticles_;
	std::vector<Math::Sphere> blockParticleSpheres_;
	std::vector<Vector3> blockParticlesPositions_;
};