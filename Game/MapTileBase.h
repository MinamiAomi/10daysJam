#pragma once

#include <cstdint>

class Map;

class MapTileBase {
public:
    MapTileBase(Map& map, uint16_t row, uint16_t column) : map_(map), row_(row), column_(column) {}
    virtual ~MapTileBase() = default;

    virtual void OnInitialize() {}
    virtual void OnUpdate() {}
    virtual void OnBreak() {}

    bool IsActive() const { return isActive_; }
    uint16_t GetRow() const { return row_; }
    uint16_t GetColumn() const { return column_; }

protected:
    Map& map_;
    uint16_t row_ = 0;
    uint16_t column_ = 0;
    bool isActive_ = false;
    
};