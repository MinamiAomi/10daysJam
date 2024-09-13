#pragma once
#include "MapTileBase.h"

#include "GameObject/GameObject.h"
#include "Math/Transform.h"
#include "Graphics/Model.h"
#include "Engine/Collision/Collider.h"

class Player;

class MapBar :
	public MapTileBase, GameObject {
public:
	using MapTileBase::MapTileBase;

	enum Mode {
		Left,
		Right

	};

	void OnInitialize() override;
	void OnUpdate() override;
	void OnBreak() override;
	
	void SetPlayer(const std::shared_ptr<Player>& player) { player_ = player; }
	void SetMode(Mode mode) { mode_ = mode; }

private:
	void OnCollision(const CollisionInfo& collisionInfo);

	ModelInstance model_;
	std::shared_ptr<BoxCollider> collider_;
	std::shared_ptr<Player> player_;
	Mode mode_;
	float animationParameter_ = 0.0f;

};