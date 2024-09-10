#include "Map.h"

#include <filesystem>
#include <fstream>

#include "Externals/nlohmann/json.hpp"

#include "MapBlock.h"
#include "MapSection.h"

namespace {
    static const std::filesystem::path kMapDataRootPath = "Resources/Map";
    static const std::filesystem::path kMapDataFile = "MapData.json";
}

void Map::Initialize() {

    Load();

    for (uint32_t i = 0; i < 10; ++i) {
        uint32_t addSectionIndex = rng_.NextUIntRange(0, (uint32_t)sections_.size() - 1);
        AddSection(addSectionIndex);
    }
}

void Map::Update() {
    for (auto& [key, tile] : tileInstanceList_) {
        if (tile->IsActive()) {
            tile->OnUpdate();
        }
    }
}

void Map::CheckCollision() {
    for (const auto& collider : colliders_) {
        Vector2 vertices[] = {
            { -0.5f, -0.5f },
            { -0.5f,  0.5f },
            {  0.5f,  0.5f },
            {  0.5f, -0.5f },
        };
        Matrix3x3 worldMatrix = Matrix3x3::MakeAffineTransform(collider->size_, collider->rotate_, collider->position_);
        for (auto& vertex : vertices) {
            vertex = vertex * worldMatrix;
        }
        float minX = FLT_MAX, maxX = FLT_MIN, minY = FLT_MAX, maxY = FLT_MIN;




        }
    }

    void Map::AddCollider(const std::shared_ptr<MapCollider>& collider) {
        assert(collider);
        collider->map_ = this;
        colliders_.emplace_back(collider);
    }

    void Map::RemoveCollider(const std::shared_ptr<MapCollider>& collider) {
        colliders_.remove(collider);
    }

    void Map::Load() {
        nlohmann::json json;
        {
            std::ifstream file(kMapDataRootPath / kMapDataFile);
            assert(file.is_open());
            file >> json;
            file.close();
        }

        assert(json["Sections"].is_array());
        // 区間データの読み込み
        sections_.reserve(json["Sections"].size());
        for (const auto& sectionFile : json["Sections"]) {
            auto& section = sections_.emplace_back(std::make_unique<MapSection>());
            section->Load(kMapDataRootPath / sectionFile.get<std::string>());
        }

    }

    void Map::AddSection(uint32_t sectionIndex) {
        assert(sectionIndex < sections_.size());
        // 追加する区画の種類
        sectionOrder_.emplace_back(sectionIndex);
        auto& section = *sections_[sectionIndex];
        // 追加するタイルデータ
        const auto& addTileData = section.GetTileData();
        // 現状の行
        uint16_t baseRow = (uint16_t)tileData_.size();
        // 追加
        tileData_.insert(tileData_.end(), addTileData.begin(), addTileData.end());
        // タイルのインスタンスを追加
        for (uint16_t row = 0; row < addTileData.size(); ++row) {
            for (uint16_t column = 0; column < MapProperty::kMapColumn; ++column) {
                auto tile = addTileData[row][column];
                uint16_t mapRow = row + baseRow, mapColumn = column;
                auto tileInstance = CreateTileInstance(tile, mapRow, mapColumn);
                if (tileInstance) {
                    tileInstanceList_[PosKey(mapRow, mapColumn)] = std::move(tileInstance);
                }
            }
        }
    }

    std::unique_ptr<MapTileBase> Map::CreateTileInstance(Tile::Enum tile, uint16_t row, uint16_t column) {
        std::unique_ptr<MapTileBase> instance;
        switch (tile) {
        case Tile::Block: {
            auto block = std::make_unique<MapBlock>(*this, row, column);
            instance = std::move(block);
            break;
        }
        case Tile::Air:
        default:
            break;
        }

        if (instance) {
            instance->OnInitialize();
        }
        return instance;
    }
