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
	material_->albedo = { 1.0f,1.0f,1.0f };
	//model_.SetMaterial(material_);
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
	static const Vector3 accelaration = {0.0f, -0.01f,0.0f };
	velocity_ += accelaration;
	transform.translate += velocity_;
	if (!isGround_) {
		float rotationSpeed = 1.0f * Math::ToRadian * (float(index_ % 2) * 2.0f - 1.0f);
		transform.rotate *= Quaternion::MakeFromAngleAxis(rotationSpeed,{1.0f,1.0f,1.0f});
	}

	Map::PosKey posKey = map_->CalcTilePosition(Vector2{ transform.translate.x,transform.translate.y - collSphere_.radius });
	const auto& tileData = map_->GetTileData();
	if (tileData[posKey.row][posKey.column] == Tile::Enum::Block) {
		const auto& tileInstance = map_->GetTileInstanceList();
		if (tileInstance.contains(posKey)) {
			const auto& tile = tileInstance.at(posKey);

			//床にはねる ここをマップチップに
			if (tile->IsActive() == true) {
				transform.translate.y = -posKey.row * MapProperty::kBlockSize + collSphere_.radius;
				if (-velocity_.y <= collSphere_.radius) {
					isGround_ = true;
				}
				velocity_ = -velocity_ * 0.2f;
			}
		}
	}

	model_.SetIsActive(isAlive_);

	UpdateTransform();
}

void BlockParticle::Reset()
{
	isAlive_ = false;
	model_.SetIsActive(false);
}

void BlockParticle::UpdateTransform() {
	transform.UpdateMatrix();
	model_.SetWorldMatrix(transform.worldMatrix);
}