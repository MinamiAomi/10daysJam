#pragma once

#include <cstdint>
#include <array>
#include "Graphics/Model.h"
#include "Math/Transform.h"
#include "Math/Random.h"

#include "Engine/Graphics/ParticleManager.h"
class Camera;
class Player;

class Particles {
public:


	void SetCamera(const Camera* camera) { camera_ = camera; }
	void SetPlayer(const Player* player) { player_ = player; }
	void Initialize();
	void Update();

	void Reset();

	void SetEmit(bool is) { isEmit_ = is; }
	void SetEmitRotate(bool is) { isEmitRotate_ = is; }
	void SetEmitPlayer(bool is) { isEmitPlayer_ = is; }

	void EmitTrunade(const Vector3& position);

private:

	ParticleManager* particleManager_ = nullptr;

	void Emit();
	void RotateEmit();
	void PlayerEmit();

	Random::RandomNumberGenerator rng_;
	const Camera* camera_;
	const Player* player_;

	Transform emitTransform_;
	Transform emitPlayerTransform_;
	bool isEmit_ = false;
	bool isEmitRotate_ = false;
	bool isEmitPlayer_ = false;
	uint32_t emitNum_ = 1;
	uint32_t emitRotateNum_ = 1;
	uint32_t emitPlayerNum_ = 4;
	uint32_t emitPlayerFrame_ = 2;
	uint32_t emitTranadeNum_ = 3;

	uint32_t frame_ = 0;
	Vector3 minDirection_;
	Vector3 maxDirection_;
	
};