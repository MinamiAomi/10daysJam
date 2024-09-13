#pragma once

#include <array>
#include <vector>
#include <unordered_map>
#include <memory>
#include <list>

#include "Math/Transform.h"
#include "MapProperty.h"
#include "MapSection.h"
#include "MapTileBase.h"
#include "MapCollider.h"
#include "Math/Random.h"
#include "Math/Camera.h"

#include "Player.h"
#include "BlockParticles.h"
#include "Score.h"
#include "Audio/AudioSource.h"
#include "Particles.h"


class Map {
public:
    // 位置をmapのキーにするためのクラス
    struct PosKey {
        uint16_t row, column;

        PosKey() : row(0), column(0) {}
        PosKey(uint16_t row, uint16_t column) : row(row), column(column) {}
        bool operator==(const PosKey& rhs) const {
            return row == rhs.row && column == rhs.column;
        }
    };

    struct PosKeyHash {
        std::size_t operator()(const PosKey& key) const {
            std::size_t rowHash = std::hash<uint16_t>{}(key.row);
            std::size_t columnHash = std::hash<uint16_t>{}(key.column);
            return rowHash ^ (columnHash << 1);
        }
    };

    void Initialize();
    void Update();
    void Generate();

    void CheckCollision();

    void AddCollider(const std::shared_ptr<MapCollider>& collider);
    void RemoveCollider(const std::shared_ptr<MapCollider>& collider);

    void RemoveTile(uint16_t row, uint16_t column);

    void SetCamera(const std::shared_ptr<Camera>& camera) { camera_ = camera; }
    void SetPlayer(const std::shared_ptr<Player>& player) { player_ = player; }
    void SetScore(const std::shared_ptr<Score>& score) { score_ = score; }
    void SetBlockParticles(std::shared_ptr<BlockParticles> blockParticles) { blockParticles_ = blockParticles; }

    PosKey CalcTilePosition(const Vector2& worldPosition) const;
    void MapRangeTile(int32_t& row, int32_t& column) const;

    const std::vector<MapProperty::RowData>& GetTileData() const { return tileData_; }

    const std::unordered_map<PosKey, std::unique_ptr<MapTileBase>, PosKeyHash>& GetTileInstanceList() const { return tileInstanceList_; }



    // マップのベースのトランスフォーム
    Transform transform;

    void SetParticles(Particles* particles) {
        particles_ = particles;
    }
    Particles* particles_;

    // マップの縦の長さ
    float GetMapRow() { return float(tileData_.size()) * MapProperty::kBlockSize; }
private:
    void CullingTile();
    void Load();
    void AddSection(uint32_t sectionIndex);
    std::unique_ptr<MapTileBase> CreateTileInstance(Tile::Enum tile, uint16_t row, uint16_t column);

    // セクションの順番
    std::vector<uint32_t> sectionOrder_;
    // タイルのデータ
    std::vector<MapProperty::RowData> tileData_;
    // タイルのインスタンスリスト
    std::unordered_map<PosKey, std::unique_ptr<MapTileBase>, PosKeyHash> tileInstanceList_;
    // 削除予定タイルリスト
    std::unordered_map<PosKey, std::unique_ptr<MapTileBase>, PosKeyHash> removeTileInstanceList_;
    // セクションごとのデータ
    std::vector<std::unique_ptr<MapSection>> sections_;
    // 当たり判定を取るコライダー
    std::list<std::shared_ptr<MapCollider>> colliders_;
    // 乱数生成器
    Random::RandomNumberGenerator rng_;
    // プレイヤー
    std::shared_ptr<Player> player_;
    //Particles
    std::shared_ptr<BlockParticles> blockParticles_;
    // Score
    std::shared_ptr<Score> score_;
    // カメラ
    std::shared_ptr<Camera> camera_;

    // カリングの上部
    uint16_t preCullingRangeTop_;
    // カリングの下部
    uint16_t preCullingRangeBottom_;

    // SEのリスト
    std::list<std::unique_ptr<AudioSource>> se_;

    uint32_t breakTiles_;
};
