#pragma once

#include <array>

struct Tile {
	enum Enum {
		Air,
		Block,
		Gravity,

		NumTypes
	};
};

namespace MapProperty {
	static const uint32_t kMapColumn = 20;
	static const float kBlockSize = 2.0f;
	using RowData = std::array<Tile::Enum, kMapColumn>;

	static const float kGravityRadius = 16.0f;
	// 中心からの距離
	static const float kSideLimit = (float(kMapColumn) * kBlockSize) * 0.5f;
}