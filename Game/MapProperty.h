#pragma once

#include <array>

struct Tile {
    enum Enum {
        Air,
        Block,

        NumTypes
    };
};

namespace MapProperty {
    static const uint32_t kMapColumn = 12;
    using RowData = std::array<Tile::Enum, kMapColumn>;
}