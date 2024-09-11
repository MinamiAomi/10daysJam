#include "Map.h"

#include <filesystem>
#include <fstream>

#include "Externals/nlohmann/json.hpp"

#include "MapBlock.h"
#include "MapGravity.h"
#include "MapSection.h"

namespace {
    static const std::filesystem::path kMapDataRootPath = "Resources/Map";
    static const std::filesystem::path kMapDataFile = "MapData.json";

    Vector2 Projection(const Vector2* vertices, size_t count, const Vector2& axis) {
        Vector2 minmax(Dot(axis, vertices[0]));
        for (size_t i = 1; i < count; ++i) {
            float dot = Dot(axis, vertices[i]);
            minmax.x = std::min(dot, minmax.x);
            minmax.y = std::max(dot, minmax.y);
        }
        return minmax;
    }

    float GetOverlap(const Vector2& minmax1, const Vector2& minmax2) {
        float range1 = minmax1.y - minmax1.x;
        float range2 = minmax2.y - minmax2.x;
        float maxOverlap = std::max(minmax1.y, minmax2.y) - std::min(minmax1.x, minmax2.x);
        return range1 + range2 - maxOverlap;
    }

    bool IsCollision(const Vector2* vertices1, const Vector2* vertices2, uint32_t vertexCount, const Vector2* axes, uint32_t axisCount) {

        float minOverlap = FLT_MAX;
        Vector2 minOverlapAxis = {};

        // 分離軸判定関数
        auto IsSeparateAxis = [&](const Vector2& axis) {
            if (axis == Vector2::zero) { return false; }
            Vector2 minmax1 = Projection(vertices1, vertexCount, axis);
            Vector2 minmax2 = Projection(vertices2, vertexCount, axis);

            // 分離軸である
            if (!(minmax1.x <= minmax2.y && minmax1.y >= minmax2.x)) {
                return true;
            }

            float overlap = GetOverlap(minmax1, minmax2);

            if (overlap < minOverlap) {
                minOverlapAxis = axis;
                minOverlap = overlap;
            }

            return false;
            };

        for (uint32_t i = 0; i < axisCount; ++i) {
            if (std::isnan(axes[i].x) || std::isnan(axes[i].y)) { continue; }
            if (IsSeparateAxis(axes[i])) { return false; }
        }
        return true;
    }

}

void Map::Initialize() {

    Load();

    Generate();
}

void Map::Update() {
    for (auto& [key, tile] : tileInstanceList_) {
        if (tile->IsActive()) {
            tile->OnUpdate();
        }
    }
}

void Map::Generate() {
    for (uint32_t i = 0; i < 10; ++i) {
        uint32_t addSectionIndex = rng_.NextUIntRange(0, (uint32_t)sections_.size() - 1);
        AddSection(addSectionIndex);
    }
}

void Map::CheckCollision() {
    float invBlockSize = 1.0f / MapProperty::kBlockSize;
    for (const auto& collider : colliders_) {
        Vector2 vertices[] = {
            { -0.5f, -0.5f },
            { -0.5f,  0.5f },
            {  0.5f,  0.5f },
            {  0.5f, -0.5f },
        };
        Matrix3x3 worldMatrix = Matrix3x3::MakeAffineTransform(collider->size_, collider->rotate_, collider->position_);
        float minX = FLT_MAX, maxX = FLT_MIN, minY = FLT_MAX, maxY = FLT_MIN;
        for (auto& vertex : vertices) {
            vertex = vertex * worldMatrix;

            Vector2 mapVertex = { (vertex.x + MapProperty::kMapColumn) * invBlockSize, vertex.y * -invBlockSize };

            minX = std::min(minX, mapVertex.x);
            maxX = std::max(maxX, mapVertex.x);
            minY = std::min(minY, mapVertex.y);
            maxY = std::max(maxY, mapVertex.y);
        }
        minX = std::max(minX, 0.0f);
        maxX = std::max(maxX, 0.0f);
        minY = std::max(minY, 0.0f);
        maxY = std::max(maxY, 0.0f);
        uint16_t startColumn = std::min((uint16_t)minX, (uint16_t)(MapProperty::kMapColumn - 1));
        uint16_t endColumn = std::min((uint16_t)maxX, (uint16_t)(MapProperty::kMapColumn - 1));
        uint16_t startRow = std::min((uint16_t)minY, (uint16_t)(tileData_.size() - 1));
        uint16_t endRow = std::min((uint16_t)maxY, (uint16_t)(tileData_.size() - 1));

        Matrix3x3 rotateMatrix = Matrix3x3::MakeRotation(collider->rotate_);
        Vector2 axes[] = {
            Vector2(1.0f, 0.0f) * rotateMatrix,
            Vector2(0.0f, 1.0f) * rotateMatrix,
            Vector2(1.0f, 0.0f),
            Vector2(0.0f, 1.0f)
        };

        for (uint16_t row = startRow; row <= endRow; ++row) {
            for (uint16_t column = startColumn; column <= endColumn; ++column) {
                if (tileInstanceList_.contains(PosKey(row, column)) &&
                    tileInstanceList_[PosKey(row, column)]->IsActive()) {

                    Vector2 tileVertices[] = {
                            { -1.0f, -1.0f },
                            { -1.0f,  1.0f},
                            {  1.0f,  1.0f},
                            {  1.0f, -1.0f},
                    };
                    Vector2 base = { MapProperty::kBlockSize * (float)column - (float)MapProperty::kMapColumn, -MapProperty::kBlockSize * (float)row };
                    for (auto& vertex : tileVertices) {
                        vertex = vertex + base;
                    }


                    if (IsCollision(vertices, tileVertices, _countof(vertices), axes, _countof(axes))) {
                        switch (collider->mode_)
                        {
                        case MapCollider::Break:
                            tileInstanceList_[PosKey(row, column)]->OnBreak();
                            break;
                        default:
                            break;
                        }
                    }

                }
            }
        }



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

Map::PosKey Map::CalcTilePosition(const Vector2& worldPosition) {
    float invBlockSize = 1.0f / MapProperty::kBlockSize;
    Vector2 tilePosition = { (worldPosition.x + MapProperty::kMapColumn) * invBlockSize, worldPosition.y * -invBlockSize };
    tilePosition.x = std::max(tilePosition.x, 0.0f);
    tilePosition.y = std::max(tilePosition.y, 0.0f);
    PosKey posKey((uint16_t)tilePosition.y, (uint16_t)tilePosition.x);
    posKey.row = std::min((uint16_t)posKey.row, (uint16_t)(tileData_.size() - 1));
    posKey.column = std::min((uint16_t)posKey.column, (uint16_t)(MapProperty::kMapColumn - 1));
    return posKey;
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
    case Tile::Block:
    {
        auto block = std::make_unique<MapBlock>(*this, row, column);
        block->SetScore(score_);
        block->SetBlockParticles(blockParticles_);
        instance = std::move(block);
        break;
    }
    case Tile::Gravity:
    {
        auto block = std::make_unique<MapGravity>(*this, row, column);
        block->SetScore(score_);
        block->SetPlayer(player_);
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
