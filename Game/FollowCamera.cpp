#include "FollowCamera.h"

void FollowCamera::Initialize() {}

void FollowCamera::Update() {
	camera_->SetPosition({ 0.0f,player_->transform.worldMatrix.GetTranslate().y ,-80.0f });
	camera_->UpdateMatrices();
}
