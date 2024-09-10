#include "BlockParticles.h"

#include "Framework/AssetManager.h"

#include "CollisionAttribute.h"
#include "GameProperty.h"

#include "Game/Player.h"

#include "Engine/Utility/Utility.h"

void BlockParticles::Initialize(Player* player) {
	player_ = player;
	Utility::Positions("blockParticlePosition.txt", blockParticlesPositions_);
	Utility::Spheres("blockParticleSphere.txt", blockParticleSpheres_);
}

void BlockParticles::Emit(const Vector3& position)
{
	int emitNum = 0;
	int handle = 0;
	for (int i = 0; i < kEmitOneBlockPaticleNum; i++) {
		for (int j = handle; j < kBlockParticleNum;j++) {
			if (!blockParticles_[j].GetIsAlive()){
				Vector3 a = blockParticlesPositions_[emitNum];
				blockParticles_[j].Emit(position + a, blockParticleSpheres_[emitNum], emitNum);
				emitNum++;
				handle = j;
				break;
			}
		}
		if (emitNum >= kEmitOneBlockPaticleNum) {
			break;
		}
	}
}

void BlockParticles::Update() {
	for (int i = 0; i < kBlockParticleNum; i++) {
		if (blockParticles_[i].GetIsAlive()) {
			blockParticles_[i].Update();
		}
	}
}

