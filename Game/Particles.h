#pragma once

#include <cstdint>
#include <array>
#include "Graphics/Model.h"
#include "Math/Transform.h"
#include "Math/Random.h"
class Camera;

class Particles {
public:
	static const uint32_t kParticleNum = 50;

	void SetCamera(const Camera* camera) { camera_ = camera; }
	void Initialize();
	void Update();

	void Reset();

	struct Particle {
		Transform transform;
		Vector3 direction;
		Vector3 velocity;
		std::unique_ptr<ModelInstance> model_;
		bool isActive;
	};

	void SetEmit(bool is) { isEmit_ = is; }

private:

	void Emit();
	void ParticleUpdate();

	Random::RandomNumberGenerator rng_;
	const Camera* camera_;

	Transform emitTransform_;
	std::array<Particle, kParticleNum> particles_;
	float speed_ = 0.1f;
	float scaleSpeed_ = 0.03f;
	Vector3 initialScale = {5.5f,5.5f,5.5f};
	bool isEmit_ = false;
	uint32_t emitNum_ = 20;
	Vector3 minDirection_;
	Vector3 maxDirection_;
	
};