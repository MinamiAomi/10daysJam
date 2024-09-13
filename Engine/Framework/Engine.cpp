#include "Engine.h"

#include <memory>

#include "Game.h"
#include "Graphics/GameWindow.h"
#include "Graphics/RenderManager.h"
#include "Input/Input.h"
#include "Audio/AudioDevice.h"
#include "Scene/SceneManager.h"
#include "AssetManager.h"
#include "ThreadPool.h"

namespace {
	Game* g_game = nullptr;
	std::unique_ptr<ThreadPool> g_threadPool = nullptr;
	GameWindow* g_gameWindow = nullptr;
	Graphics* g_graphics = nullptr;
	Input* g_input = nullptr;
	AudioDevice* g_audioDevice = nullptr;
	RenderManager* g_renderManager = nullptr;
	SceneManager* g_sceneManager = nullptr;
	AssetManager* g_assetManager = nullptr;
}

void Engine::Run(Game* game) {


	g_game = game;

	g_threadPool = std::make_unique<ThreadPool>();

	g_gameWindow = GameWindow::GetInstance();
	g_gameWindow->Initialize(L"LE3A_20_ミナミアオミ", 540, 960);

	g_graphics = Graphics::GetInstance();
	g_graphics->Initialize();

	g_input = Input::GetInstance();
	g_input->Initialize(g_gameWindow->GetHWND());

	g_audioDevice = AudioDevice::GetInstance();
	g_audioDevice->Initialize();

	g_renderManager = RenderManager::GetInstance();
	g_renderManager->Initialize();

	g_sceneManager = SceneManager::GetInstance();
	g_assetManager = AssetManager::GetInstance();


	g_game->OnInitialize();

	while (g_gameWindow->ProcessMessage()) {
		g_input->Update();
		g_sceneManager->Update();

		g_renderManager->Render();
	}

	g_sceneManager->Finalize();
	g_game->OnFinalize();

	g_threadPool.reset();
	g_audioDevice->Finalize();
	g_renderManager->Finalize();
	g_graphics->Finalize();
	g_gameWindow->Shutdown();
}

Game* Engine::GetGame() {
	return g_game;
}

GameWindow* Engine::GetGameWindow() {
	return g_gameWindow;
}

Graphics* Engine::GetGraphics() {
	return g_graphics;
}

Input* Engine::GetInput() {
	return g_input;
}

AudioDevice* Engine::GetAudioDevice() {
	return g_audioDevice;
}

RenderManager* Engine::GetRenderManager() {
	return g_renderManager;
}

SceneManager* Engine::GetSceneManager() {
	return g_sceneManager;
}

AssetManager* Engine::GetAssetManager() {
	return g_assetManager;;
}

ThreadPool* Engine::GetThreadPool() {
	return g_threadPool.get();
}

