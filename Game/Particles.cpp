#include "Particles.h"
#include "Engine/Framework/AssetManager.h"
#include "Engine/Math/Camera.h"
#include "Player.h"

void Particles::Initialize() {

	particleManager_ = ParticleManager::GetInstance();

	minDirection_ = { -0.5f,0.5f,-0.5f };
	maxDirection_ = { 0.5f,1.0f,0.5f };
	emitPlayerTransform_.SetParent(&player_->transform);
	emitPlayerTransform_.translate = { 0.0f,0.0f,-player_->GetSize().y };
	emitPlayerTransform_.rotate = Quaternion::MakeForXAxis(-90.0f * Math::ToRadian);
	Reset();
}

void Particles::Update() {
	frame_++;
	Emit();
	RotateEmit();
	PlayerEmit();
}

void Particles::Reset() {
	particleManager_->Reset();
	frame_ = 0;
}

void Particles::Emit()
{
	emitTransform_.UpdateMatrix();

	if (isEmit_) {


		std::list<Particle> emitParticles;

		for (uint32_t i = 0; i < emitNum_; i++) {

			Particle particle;
			particle.acceleration = { 0.0f,0.0f,0.0f };
			particle.startAlpha = 1.0f;
			particle.endAlpha = 0.0f;
			particle.startColor = { 1.0f,1.0f,1.0f };
			particle.endColor = { 1.0f,1.0f,1.0f };
			particle.startColor = { 1.0f,1.0f,1.0f };
			particle.startSize = 3.0f;
			particle.endSize = 2.0f;
			particle.lifeTime = 300;
			particle.existenceTime = 0;
			particle.position = { 0.0f,0.0f,0.0f };
			particle.velocity = Vector3{ rng_.NextFloatRange(minDirection_.x,maxDirection_.x),rng_.NextFloatRange(minDirection_.y,maxDirection_.y) ,rng_.NextFloatRange(minDirection_.z,maxDirection_.z) }.Normalize() * 1.0f;
			particle.spin = 0.0f;
			particle.spinSpeed = (3.0f * Math::ToRadian) * ((i % 2 ) * -1.0f);
			particle.isCircle = false;

			emitParticles.push_back(particle);
		}

		particleManager_->AddParticles(std::move(emitParticles));
	}
}

void Particles::RotateEmit()
{
	emitTransform_.UpdateMatrix();
	if (isEmitRotate_) {


		std::list<Particle> emitParticles;

		for (uint32_t i = 0; i < emitRotateNum_; i++) {

			Particle particle;
			particle.acceleration = { 0.0f,0.0f,0.0f };
			particle.startAlpha = 1.0f;
			particle.endAlpha = 0.0f;
			particle.startColor = { 1.0f,1.0f,1.0f };
			particle.endColor = { 1.0f,1.0f,1.0f };
			particle.startColor = { 1.0f,1.0f,1.0f };
			particle.startSize = 3.0f;
			particle.endSize = 2.0f;
			particle.lifeTime = 300;
			particle.existenceTime = 0;
			particle.position = { 0.0f,0.0f,0.0f };
			particle.velocity = Vector3{ rng_.NextFloatRange(minDirection_.x,maxDirection_.x),rng_.NextFloatRange(minDirection_.y,maxDirection_.y) ,rng_.NextFloatRange(minDirection_.z,maxDirection_.z) }.Normalize() * 1.0f;
			particle.spin = 0.0f;
			particle.spinSpeed = (1.0f * Math::ToRadian) * ((i % 2 ) * -1.0f);
			particle.isCircle = false;

			particle.rotate = 0.0f;
			particle.endRadius = 10.0f;
			particle.startRadius = 0.0f;
			particle.rotateCenter = { 0.0f,0.0f,0.0f };
			particle.ratateSpeed = (2.0f * Math::ToRadian) * ((i % 2 ) * -1.0f);


			emitParticles.push_back(particle);
		}

		particleManager_->AddRotateParticles(std::move(emitParticles));
	}
}

void Particles::PlayerEmit()
{
	emitPlayerTransform_.UpdateMatrix();
	if (isEmitPlayer_) {
		
		if (frame_ % emitPlayerFrame_ == 0) {

			std::list<Particle> emitParticles;

			for (uint32_t i = 0; i < emitPlayerNum_; i++) {

				Particle particle;
				particle.acceleration = { 0.0f,0.0f,0.0f };
				particle.startAlpha = 1.0f;
				particle.endAlpha = 0.0f;
				particle.startColor = { 1.0f,1.0f + 0.5f * ((i % 2) * -1.0f),0.0f };
				particle.endColor = { 1.0f,1.0f + 0.5f * ((i % 2) * -1.0f),0.0f };
				particle.startSize = 0.3f + 0.1f * (i % 2);
				particle.endSize = 0.3f + 0.1f * (i % 2);
				particle.lifeTime = 60;
				particle.existenceTime = 0;
				particle.position = { 0.0f,0.0f,0.0f };
				particle.velocity = player_->GetVelocity() / 1.5f;
				particle.spin = 0.0f;
				particle.spinSpeed = 0.0f;
				particle.isCircle = true;

				particle.rotate = 0.0f;
				particle.endRadius = 10.0f;
				particle.startRadius = 0.0f;
				particle.rotateCenter = emitPlayerTransform_.worldMatrix.GetTranslate();
				particle.ratateSpeed = (rng_.NextFloatRange(20.0f, 30.0f) * Math::ToRadian) ;
				particle.playerQuaternion = player_->transform.rotate;


				emitParticles.push_back(particle);
			}

			particleManager_->AddPlayerParticles(std::move(emitParticles));
		}
	}
}

