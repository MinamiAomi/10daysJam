#pragma once
#include "MapTileBase.h"

#include "GameObject/GameObject.h"
#include "Math/Transform.h"
#include "Graphics/Model.h"
#include "Engine/Collision/Collider.h"

class Player;

class MapBomb :
	public MapTileBase, GameObject {
public:
	using MapTileBase::MapTileBase;

	void OnInitialize() override;
	void OnUpdate() override;
	void OnBreak() override;

	void SetPlayer(const std::shared_ptr<Player>& player) { player_ = player; }

private:
	void OnCollision(const CollisionInfo& collisionInfo);

	ModelInstance model_;
	std::shared_ptr<SphereCollider> collider_;
	std::shared_ptr<Player> player_;
};