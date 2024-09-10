#pragma once

#include <list>

#include "Block.h"

class BlockManager {
public:
	struct BlockType {
		enum Type {
			kNormal,
			kExplosion,
			kCount,
		};
	};

	void Initialize();
	void Update();

	void Create(const Vector3& position, const BlockType::Type& type);
	void Reset();
private:
	std::list<std::shared_ptr<Block>> blocks_;
};