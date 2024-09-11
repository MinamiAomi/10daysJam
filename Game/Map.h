#pragma once

#include <array>
#include <vector>
#include <map>
#include <memory>
#include <list>

#include "Math/Transform.h"
#include "MapProperty.h"
#include "MapSection.h"
#include "MapTileBase.h"
#include "MapCollider.h"
#include "Math/Random.h"

#include "Player.h"
#include "BlockParticles.h"
#include "Score.h"

class Map {
public:
	// 位置をmapのキーにするためのクラス
	struct PosKey {
		uint16_t row, column;

		PosKey() : row(0), column(0) {}
		PosKey(uint16_t row, uint16_t column) : row(row), column(column) {}
		bool operator<(const PosKey& rhs) const {
			return (row * MapProperty::kMapColumn + column) < (rhs.row * MapProperty::kMapColumn + rhs.column);
		}
	};

	void Initialize();
	void Update();
	void Generate();

	void CheckCollision();

	void AddCollider(const std::shared_ptr<MapCollider>& collider);
	void RemoveCollider(const std::shared_ptr<MapCollider>& collider);

	void SetPlayer(std::shared_ptr<Player> player) { player_ = player; }
	void SetScore(std::shared_ptr<Score> score) { score_ = score; }
	void SetBlockParticles(std::shared_ptr<BlockParticles> blockParticles) { blockParticles_ = blockParticles; }

	PosKey CalcTilePosition(const Vector2& worldPosition) const;

	const std::vector<MapProperty::RowData>& GetTileData() const { return tileData_; }

	const std::map<PosKey, std::unique_ptr<MapTileBase>>& GetTileInstanceList() const { return tileInstanceList_; }



	// マップのベースのトランスフォーム
	Transform transform;

	// マップの縦の長さ
	float GetMapRow() { return float(tileData_.size())* MapProperty::kBlockSize; }
private:

	void Load();
	void AddSection(uint32_t sectionIndex);
	std::unique_ptr<MapTileBase> CreateTileInstance(Tile::Enum tile, uint16_t row, uint16_t column);

	// セクションの順番
	std::vector<uint32_t> sectionOrder_;
	// タイルのデータ
	std::vector<MapProperty::RowData> tileData_;
	// タイルのインスタンスリスト
	std::map<PosKey, std::unique_ptr<MapTileBase>> tileInstanceList_;
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
};