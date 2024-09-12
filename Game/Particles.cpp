#include "Particles.h"
#include "Engine/Framework/AssetManager.h"
#include "Engine/Math/Camera.h"

void Particles::Initialize() {
	minDirection_ = { -0.5f,0.5f,-0.5f };
	maxDirection_ = { 0.5f,1.0f,0.5f };
	for (Particle& particle : particles_) {
		particle.model_ = std::make_unique<ModelInstance>();
		particle.model_->SetModel(AssetManager::GetInstance()->FindModel("particle"));
	}
	Reset();
}

void Particles::Update() {
	Emit();
	ParticleUpdate();
}

void Particles::Reset() {
	for (Particle & particle : particles_) {
		particle.isActive = false;
		particle.model_->SetIsActive(false);
	}
}

void Particles::Emit()
{

	if (isEmit_) {

		emitTransform_.UpdateMatrix();

		for (uint32_t i = 0; i < emitNum_; i++) {
			for (Particle& particle : particles_) {
				if (!particle.isActive) {
					particle.isActive = true;
					particle.model_->SetIsActive(true);
					
					particle.transform.translate = emitTransform_.worldMatrix.GetTranslate();
					particle.transform.rotate = Quaternion::identity;
					particle.transform.scale = initialScale;
					particle.direction = Vector3{ rng_.NextFloatRange(minDirection_.x,maxDirection_.x),rng_.NextFloatRange(minDirection_.y,maxDirection_.y) ,rng_.NextFloatRange(minDirection_.z,maxDirection_.z) };
					break;
				}
			}
		}
	}
}

void Particles::ParticleUpdate()
{
	for (uint32_t i = 0; Particle & particle : particles_) {
		if (particle.isActive) {
			particle.transform.translate += particle.direction * speed_;
			particle.transform.scale.x -= scaleSpeed_;
			particle.transform.scale.y -= scaleSpeed_;
			particle.transform.scale.z -= scaleSpeed_;
			particle.transform.rotate.x = -90.0f * Math::ToRadian;
			particle.transform.UpdateMatrix();
			particle.transform.worldMatrix = particle.transform.worldMatrix * camera_->GetBillbordMatrix();
			particle.model_->SetWorldMatrix(particle.transform.worldMatrix);
			if (particle.transform.scale.x <= 0.0f) {
				particle.isActive = false;
				particle.model_->SetIsActive(false);
			}
		}
		i++;
	}
}
