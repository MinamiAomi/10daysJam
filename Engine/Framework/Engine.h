#pragma once

#include <memory>

#include "Input/Input.h"


class Game;
class GameWindow;
class Graphics;
class Input;
class AudioDevice;
class RenderManager;
class SceneManager;
class AssetManager;
class ThreadPool;

class Engine {
public:
    static void Run(Game* game);

    static Game* GetGame();
    static GameWindow* GetGameWindow();
    static Graphics* GetGraphics();
    static Input* GetInput();
    static AudioDevice* GetAudioDevice();
    static RenderManager* GetRenderManager();
    static SceneManager* GetSceneManager();
    static AssetManager* GetAssetManager();
    static ThreadPool* GetThreadPool();
};