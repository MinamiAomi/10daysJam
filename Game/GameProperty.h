#pragma once

#include "Engine/Math/MathUtils.h"

namespace GameProperty {
	enum State {
		kInGame,
		kResult,
		kCount,
	};
	extern State state_;
	void foge();
}