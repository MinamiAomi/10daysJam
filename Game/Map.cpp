#include "Map.h"

#include <filesystem>
#include <fstream>

#include "Externals/nlohmann/json.hpp"

#include "Framework/AssetManager.h"

#include "MapBlock.h"
#include "MapGravity.h"
#include "MapBar.h"
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
    // カリング処理
    CullingTile();

    for (auto& [key, tile] : tileInstanceList_) {
        if (tile->IsActive()) {
            tile->OnUpdate();
        }
    }

	for (auto it = se_.begin(); it != se_.end(); ) {
		if (!(*it)->IsPlaying()) {
			it = se_.erase(it);
		}
		else {
			++it;
		}
	}


	uint16_t lastSectionStart = (uint16_t)tileData_.size() - (uint16_t)sections_[sectionOrder_.back()]->GetHeight();
	if (preCullingRangeBottom_ >= lastSectionStart) {
		uint32_t addSectionIndex = rng_.NextUIntRange(1, (uint32_t)sections_.size() - 1);
		AddSection(addSectionIndex);
	}
}

void Map::Generate() {
	sectionOrder_.clear();
	tileInstanceList_.clear();
	tileData_.clear();
	se_.clear();

	// タイトルが0にある
	AddSection(0);
	for (uint32_t i = 0; i < 10; ++i) {
		uint32_t addSectionIndex = rng_.NextUIntRange(1, (uint32_t)sections_.size() - 1);
		AddSection(addSectionIndex);
	}

    preCullingRangeTop_ = preCullingRangeBottom_ = 0;
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
					Vector2 base = { MapProperty::kBlockSize * (float)column - (float)MapProperty::kMapColumn + MapProperty::kBlockSize * 0.5f, -MapProperty::kBlockSize * (float)row };
					for (auto& vertex : tileVertices) {
						vertex = vertex + base;
					}

					if (IsCollision(vertices, tileVertices, _countof(vertices), axes, _countof(axes))) {
						switch (collider->mode_) {
						case MapCollider::Break:
							switch (tileData_[row][column]) {
								// ブロック
							case Tile::Block:
							{
								std::unique_ptr<AudioSource> se = std::make_unique<AudioSource>();
								*se = AssetManager::GetInstance()->FindSound("blockBreak");
								se->Play();
								se_.emplace_back(std::move(se));

								tileInstanceList_[PosKey(row, column)]->OnBreak();
								tileInstanceList_.erase(PosKey(row, column));
								tileData_[row][column] = Tile::Air;
							}
							break;
							// 重力
							case Tile::Gravity:

								break;
							case Tile::Air:
							default:
								break;
							}
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

Map::PosKey Map::CalcTilePosition(const Vector2& worldPosition) const {
	float invBlockSize = 1.0f / MapProperty::kBlockSize;
	Vector2 tilePosition = { (worldPosition.x + MapProperty::kMapColumn) * invBlockSize, worldPosition.y * -invBlockSize };
	tilePosition.x = std::max(tilePosition.x, 0.0f);
	tilePosition.y = std::max(tilePosition.y, 0.0f);
	PosKey posKey((uint16_t)tilePosition.y, (uint16_t)tilePosition.x);
	posKey.row = std::min((uint16_t)posKey.row, (uint16_t)(tileData_.size() - 1));
	posKey.column = std::min((uint16_t)posKey.column, (uint16_t)(MapProperty::kMapColumn - 1));
	return posKey;
}

void Map::CullingTile() {
	assert(camera_);

	enum SamplePoint {
		TopNear,
		TopFar,
		BottomNear,
		BottomFar,

		NumPoints
	};

	Vector3 samplePoints[] = {
		{ 0.0f,  1.0f, 0.0f },
		{ 0.0f,  1.0f, 1.0f },
		{ 0.0f, -1.0f, 0.0f },
		{ 0.0f, -1.0f, 1.0f },
	};

	auto viewProjectionInverseMatrix = camera_->GetViewProjectionMatrix().Inverse();
	for (auto& samplePoint : samplePoints) {
		samplePoint = viewProjectionInverseMatrix.ApplyTransformWDivide(samplePoint);
	}

    auto ClosestPoint = [](const Vector3& origin, const Vector3& diff) {
        assert(diff.z != 0.0f);
        float t = (0.0f - origin.z) / diff.z;
        return origin + t * diff;
        };

	Vector3 top = ClosestPoint(samplePoints[TopNear], samplePoints[TopFar] - samplePoints[TopNear]);
	Vector3 bottom = ClosestPoint(samplePoints[BottomNear], samplePoints[BottomFar] - samplePoints[BottomNear]);

	auto curCullingRangeTop = CalcTilePosition({ top.x, top.y + 20.0f }).row;
	auto curCullingRangeBottom = CalcTilePosition({ bottom.x, bottom.y - 20.0f }).row;

    // 画面に映るタイルのインスタンスを生成


    // 上側の処理
    // 範囲内を消す
    if (curCullingRangeTop > preCullingRangeTop_) {
        for (uint16_t row = preCullingRangeTop_; row < curCullingRangeTop; ++row) {
            for (uint16_t column = 0; column < MapProperty::kMapColumn; ++column) {
                if (tileInstanceList_.contains(PosKey(row, column))) {
                    tileInstanceList_.erase(PosKey(row, column));
                }
            }
        }

    }
    // 範囲内に生成する
    else if (curCullingRangeTop < preCullingRangeTop_) {
        for (uint16_t row = curCullingRangeTop; row < preCullingRangeTop_; ++row) {
            for (uint16_t column = 0; column < MapProperty::kMapColumn; ++column) {
                if (!tileInstanceList_.contains(PosKey(row, column))) {
                    auto tile = tileData_[row][column];
                    auto tileInstance = CreateTileInstance(tile, row, column);
                    if (tileInstance) {
                        tileInstanceList_[PosKey(row, column)] = std::move(tileInstance);
                    }
                }
            }
        }
    }


    // 下側の処理
    // 範囲内を消す
    if (curCullingRangeBottom < preCullingRangeBottom_) {
        for (uint16_t row = curCullingRangeBottom; row < preCullingRangeBottom_; ++row) {
            for (uint16_t column = 0; column < MapProperty::kMapColumn; ++column) {
                if (tileInstanceList_.contains(PosKey(row, column))) {
                    tileInstanceList_.erase(PosKey(row, column));
                }
            }
        }

    }
    // 範囲内に生成する
    else if (curCullingRangeBottom > preCullingRangeBottom_) {
        for (uint16_t row = preCullingRangeBottom_; row < curCullingRangeBottom; ++row) {
            for (uint16_t column = 0; column < MapProperty::kMapColumn; ++column) {
                if (!tileInstanceList_.contains(PosKey(row, column))) {
                    auto tile = tileData_[row][column];
                    auto tileInstance = CreateTileInstance(tile, row, column);
                    if (tileInstance) {
                        tileInstanceList_[PosKey(row, column)] = std::move(tileInstance);
                    }
                }
            }
        }
    }

	preCullingRangeTop_ = curCullingRangeTop;
	preCullingRangeBottom_ = curCullingRangeBottom;
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
    // 追加
    tileData_.insert(tileData_.end(), addTileData.begin(), addTileData.end());
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
	case Tile::LeftBar:
	{
		auto block = std::make_unique<MapBar>(*this, row, column);
		block->SetMode(MapBar::Left);
		block->SetPlayer(player_);
		instance = std::move(block);
		break;
	}
	case Tile::RightBar:
	{
		auto block = std::make_unique<MapBar>(*this, row, column);
		block->SetMode(MapBar::Right);
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
