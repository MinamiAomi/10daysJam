#pragma once

#include "Engine/Math/MathUtils.h"

namespace GameProperty {
	const uint32_t MaxBlockRow = 20;
	const uint32_t MaxBlockColumn = 15;
	const uint32_t BlockSize = 2;
	// 中心から
	const Vector2 GameStageSize = { float(MaxBlockRow) * float(BlockSize) * 0.5f ,36.0f };
}