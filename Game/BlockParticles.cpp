#include "BlockParticles.h"

#include "Framework/AssetManager.h"

#include "CollisionAttribute.h"
#include "GameProperty.h"

#include "Game/Player.h"

void BlockParticles::Initialize(Player* player) {
	player_ = player;
	float halfBlockSize = GameProperty::BlockSize / 2.0f;
	for (int i = 0; i < kEmitOneBlockPaticleNum; i++) {
		randomPosition_[i] = Vector3{rand_.NextFloatRange(-halfBlockSize,halfBlockSize),rand_.NextFloatRange(-halfBlockSize,halfBlockSize) ,rand_.NextFloatRange(-halfBlockSize,halfBlockSize)};
	}
}

void BlockParticles::Emit(const Vector3& position)
{
	int emitNum = 0;
	for (int i = 0; i < kEmitOneBlockPaticleNum; i++) {
		if (!blockParticles_[i].GetIsAlive()) {
			emitNum++;
			blockParticles_[i].Emit(position + randomPosition_[emitNum]);
			if (emitNum >= kEmitOneBlockPaticleNum) {
				break;
			}
		}
	}
}

void BlockParticles::Update() {
	for (int i = 0; i < kEmitOneBlockPaticleNum; i++) {
		blockParticles_[i].Update();
	}
}

