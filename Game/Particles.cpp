#include "Particles.h"
#include "Engine/Framework/AssetManager.h"
#include "Engine/Math/Camera.h"
#include "Player.h"
#include "MapProperty.h"

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

void Particles::EmitTrunade(const Vector3& position)
{

	std::list<Particle> emitParticles;

	for (uint32_t i = 0; i < emitTranadeNum_; i++) {

		Particle particle;
		particle.acceleration = { 0.0f,0.0f,0.0f };
		particle.startAlpha = 1.0f;
		particle.endAlpha = 1.0f;
		float color = rng_.NextFloatRange(0.1f, 0.5f);
		particle.startColor = { color,color,1.0f };
		particle.endColor = { color,color,1.0f };
		particle.startSize = 0.5f;
		particle.endSize = 0.1f;
		particle.lifeTime = 120;
		particle.existenceTime = 0;
		particle.spin = 0.0f;
		particle.spinSpeed = (1.0f * Math::ToRadian) * ((i % 2) * 2.0f - 1.0f);
		particle.isCircle = bool(rng_.NextIntRange(0, 1));

		particle.rotate = rng_.NextFloatRange(0.0f, Math::Pi * 2.0f) * ((i % 2) * 2.0f - 1.0f);
		particle.endRadius = 0.0f;
		particle.startRadius = rng_.NextFloatRange(MapProperty::kGravityRadius * 0.5f, MapProperty::kGravityRadius);
		particle.rotateCenter = { position.x,position.y,-2.0f };
		particle.ratateSpeed = rng_.NextFloatRange(0.0f, 5.0f * Math::ToRadian) * ((i % 2) * 2.0f - 1.0f);


		emitParticles.push_back(particle);
	}

	particleManager_->AddRotateParticles(std::move(emitParticles));
}

void Particles::EmitBar(const Vector3& position, const Vector3& direction, const float length)
{

	std::list<Particle> emitParticles;

	Vector3 vertical;
	vertical.x = direction.y;
	vertical.y = -direction.x;
	vertical.z = 0.0f;

	float halfLength = length * 0.5f;

	for (uint32_t i = 0; i < uint32_t(length); i++) {

		Particle particle;
		particle.acceleration = { 0.0f,0.0f,0.0f };
		particle.startAlpha = 1.0f;
		particle.endAlpha = 0.0f;
		float color = rng_.NextFloatRange(0.1f, 0.5f);
		particle.startColor = { color,1.0f,color };
		particle.endColor = { color,1.0f,color };
		particle.startSize = 1.0f;
		particle.endSize = 1.0f;
		particle.lifeTime = 30;
		particle.existenceTime = 0;
		particle.position = position + (vertical * rng_.NextFloatRange(0.0f,halfLength)) * ((i % 2) * 2.0f - 1.0f);
		particle.position.z = -2.0f;
		particle.velocity = -direction * rng_.NextFloatRange(0.9f,1.2f);
		particle.spin = 0.0f;
		particle.spinSpeed = (3.0f * Math::ToRadian) * ((i % 2) * 2.0f - 1.0f);
		particle.isCircle = bool(rng_.NextIntRange(0, 1));

		emitParticles.push_back(particle);
	}

	particleManager_->AddParticles(std::move(emitParticles));
	
}

void Particles::EmitBom(const Vector3& position)
{
	std::list<Particle> emitParticles;

	for (uint32_t i = 0; i < 32; i++) {

		Particle particle;
		particle.acceleration = { 0.0f,0.0f,0.0f };
		particle.startAlpha = 1.0f;
		particle.endAlpha = 1.0f;
		float color = rng_.NextFloatRange(0.1f, 0.5f);
		particle.startColor = { 1.0f,color,color };
		particle.endColor = { 1.0f,color,color };
		particle.startSize = 1.2f;
		particle.endSize = 0.0f;
		particle.lifeTime = 15;
		particle.existenceTime = 0;
		particle.position = position;
		particle.position.z = -2.0f;
		particle.velocity = Vector3{ 1.0f,0.0f,0.0f } *Matrix4x4::MakeRotationZ(rng_.NextFloatRange(0.0f,360.0f * Math::ToRadian)) * rng_.NextFloatRange(0.9f,1.2f);
		particle.spin = 0.0f;
		particle.spinSpeed = (3.0f * Math::ToRadian) * ((i % 2) * 2.0f - 1.0f);
		particle.isCircle = bool(rng_.NextIntRange(0, 1));

		emitParticles.push_back(particle);
	}

	particleManager_->AddParticles(std::move(emitParticles));
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
			particle.spinSpeed = (3.0f * Math::ToRadian) * ((i % 2) * 2.0f - 1.0f);
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
			particle.spinSpeed = (1.0f * Math::ToRadian) * ((i % 2) * 2.0f - 1.0f);
			particle.isCircle = false;

			particle.rotate = 0.0f;
			particle.endRadius = 10.0f;
			particle.startRadius = 0.0f;
			particle.rotateCenter = { 0.0f,0.0f,0.0f };
			particle.ratateSpeed = (2.0f * Math::ToRadian) * ((i % 2) * 2.0f - 1.0f);


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

			std::list<Particle> emitParticles[2];


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
				particle.spinSpeed = 10.0f * Math::ToRadian;
				particle.isCircle = bool(rng_.NextIntRange(0,1));

				particle.rotate = 0.0f;
				particle.endRadius = 10.0f;
				particle.startRadius = 0.0f;
				particle.rotateCenter = emitPlayerTransform_.worldMatrix.GetTranslate();
				particle.ratateSpeed = (rng_.NextFloatRange(20.0f, 30.0f) * Math::ToRadian) ;
				particle.playerQuaternion = player_->transform.rotate;


				emitParticles[0].push_back(particle);
			}

			particleManager_->AddPlayerParticles(std::move(emitParticles[0]));


			for (uint32_t i = 0; i < 2; i++) {

				Particle particle;
				particle.acceleration = { 0.0f,0.0f,0.0f };
				particle.startAlpha = 1.0f;
				particle.endAlpha = 1.0f;
				particle.startColor = { 1.0f,1.0f + 0.5f * ((i % 2) * -1.0f),0.0f };
				particle.endColor = { 1.0f,1.0f + 0.5f * ((i % 2) * -1.0f),0.0f };
				particle.startSize = 0.5f;
				particle.endSize = 0.0f;
				particle.lifeTime = 30;
				particle.existenceTime = 0;
				float a = rng_.NextFloatRange(-0.5f,0.5f);
				particle.position = player_->transform.translate +  Vector3{ a,a,a};
				particle.velocity = Vector3::zero;
				particle.spin = 0.0f;
				particle.spinSpeed = (5.0f * Math::ToRadian) * ((i % 2) * 2.0f - 1.0f);
				particle.isCircle = bool(rng_.NextIntRange(0, 1));


				emitParticles[1].push_back(particle);
			}

			particleManager_->AddParticles(std::move(emitParticles[1]));
		}
	}
}

