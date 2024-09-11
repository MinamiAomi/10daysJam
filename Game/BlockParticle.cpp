#include "BlockParticle.h"

#include "Framework/AssetManager.h"

#include "CollisionAttribute.h"
#include "GameProperty.h"
#include "Map.h"

void BlockParticle::Initialize() {
	SetName("BlockParticle");
	material_ = std::make_shared<Material>();
}

void BlockParticle::Emit(const Vector3& position,const Vector3& velocity,const Math::Sphere& sphere,int index) {
	isAlive_ = true;
	std::string str = "blockParticle_";
	model_.SetModel(AssetManager::GetInstance()->FindModel(str + std::to_string(index)));
	material_->albedo = { 0.2f,0.2f,0.2f };
	model_.SetMaterial(material_);
	transform.translate = position;
	transform.rotate = Quaternion::identity;
	collSphere_ = sphere;
	time_ = kTime_;
	velocity_ = velocity;
	isGround_ = false;
	index_ = index;
	UpdateTransform();
}

void BlockParticle::Update() {
	time_--;
	if (time_ <= 0) {
		isAlive_ = false;
	}
	/*static const Vector3 accelaration = {0.0f, -0.01f,0.0f };
	velocity_ += accelaration;
	transform.translate += velocity_;
	if (!isGround_) {
		float rotationSpeed = 1.0f * Math::ToRadian * (float(index_ % 2) * 2.0f - 1.0f);
		transform.rotate *= Quaternion::MakeFromAngleAxis(rotationSpeed,{1.0f,1.0f,1.0f});
	}*/
	//床にはねる ここをマップチップに
	/*if (transform.translate.y <= 1.0f + collSphere_.radius) {
		transform.translate.y = 1.0f + collSphere_.radius;
		if (-velocity_.y <= collSphere_.radius) {
			isGround_ = true;
		}
		velocity_ = -velocity_ * 0.5f;
	}*/
	model_.SetIsActive(isAlive_);

	UpdateTransform();
}

void BlockParticle::UpdateTransform() {
	transform.UpdateMatrix();
	model_.SetWorldMatrix(transform.worldMatrix);
}