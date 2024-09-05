#pragma once

#include <list>

#include "Block.h"

class BlockManager {
public:
	void Initialize();
	void Update();
private:
	static const uint32_t kMaxBlockCount;
	std::list<std::shared_ptr<Block>> blocks_;
};