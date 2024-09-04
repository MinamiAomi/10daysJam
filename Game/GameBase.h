#pragma once
#include "Framework/Game.h"

class GameBase :
    public Game {
public:

    void OnInitialize() override;
    void OnFinalize() override;

private:
    void LoadResource();

};