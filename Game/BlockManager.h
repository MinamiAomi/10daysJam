#pragma once

#include <list>

#include "Block.h"

class BlockParticles;

class BlockManager {
public:
	struct BlockType {
		enum Type {
			kNormal,
			kExplosion,
			kCount,
		};
	};

	void Initialize(BlockParticles* blockParticles);
	void Update();

	void Create(const Vector3& position, const BlockType::Type& type);
	void Reset();
private:
	std::list<std::shared_ptr<Block>> blocks_;
	BlockParticles* blockParticles_;
};