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
	static const uint32_t kMapColumn = 12;
	using RowData = std::array<Tile::Enum, kMapColumn>;

	static const float kGravityRadius = 16.0f;
}