#pragma once

#include <filesystem>
#include <vector>

#include "MapProperty.h"

class MapSection {
public:
    ~MapSection() = default;

    // 読み込み
    void Load(const std::filesystem::path& path);

    const std::vector<MapProperty::RowData>& GetTileData() const { return tileData_; }
    uint32_t GetHeight() const { return (uint32_t)tileData_.size(); }

private:
    // タイルのデータ
    std::vector<MapProperty::RowData> tileData_;

};