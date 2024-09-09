#pragma once

#include <list>

#include "Block.h"

class BlockManager {
public:
	void Initialize();
	void Update();

	void Reset();
private:
	std::list<std::shared_ptr<Block>> blocks_;
};