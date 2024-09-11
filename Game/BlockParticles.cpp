#include "BlockParticles.h"

#include "Framework/AssetManager.h"

#include "CollisionAttribute.h"
#include "GameProperty.h"

#include "Game/Player.h"

#include "Engine/Utility/Utility.h"
#include "Map.h"

void BlockParticles::Initialize() {
	Utility::Positions("blockParticlePosition.txt", blockParticlesPositions_);
	Utility::Spheres("blockParticleSphere.txt", blockParticleSpheres_);
	for (int i = 0; i < kBlockParticleNum; i++) {
		blockParticles_[i].Initialize();	
		blockParticles_[i].SetMap(map_);
	}
}

void BlockParticles::Emit(const Vector3& position)
{
	static const float vibration = 0.03f;

	int emitNum = 0;
	int handle = 0;
	for (int i = 0; i < kEmitOneBlockPaticleNum; i++) {
		for (int j = handle; j < kBlockParticleNum;j++) {
			if (!blockParticles_[j].GetIsAlive()){
				Vector3 initVelocity = Vector3{rnd_.NextFloatRange(-1.0f,1.0f),rnd_.NextFloatRange(-1.0f,1.0f) ,0.0f}.Normalize() * vibration;
				blockParticles_[j].Emit(position + blockParticlesPositions_[emitNum], initVelocity,blockParticleSpheres_[emitNum], emitNum);
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

void BlockParticles::Reset()
{
	for (int i = 0; i < kBlockParticleNum; i++) {
		blockParticles_[i].Reset();
	}
}

