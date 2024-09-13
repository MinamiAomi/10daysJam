#pragma once

#include <array>

struct Tile {
	enum Enum {
		Air = 0,
		Block = 1,
		Gravity = 2,
		LeftBar = 3,
		RightBar = 4,
		Bomb = 5,

		NumTypes
	};
};

namespace MapProperty {
	static const uint32_t kMapColumn = 30;
	static const float kBlockSize = 2.0f;
	static const float kMapYOffset = kBlockSize * 74.0f;
	using RowData = std::array<Tile::Enum, kMapColumn>;

	

	static const float kGravityRadius = 16.0f;
	static const float kBombRadius = 3.0f;
	// 中心からの距離
	static const float kSideLimit = (float(kMapColumn) * kBlockSize) * 0.5f;
}