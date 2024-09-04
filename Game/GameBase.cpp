#include "GameBase.h"

#include <memory>
#include <fstream>

#include "Externals/nlohmann/json.hpp"
#include "Scene/SceneManager.h"
#include "Framework/AssetManager.h"
#include "Graphics/Model.h"
#include "Audio/Sound.h"
#include "Graphics/Animation.h"
#include "Graphics/Sprite.h"
#include "Graphics/Core/TextureLoader.h"
#include "Debug/Debug.h"

#include "GameScene.h"

namespace {
    const char kResourceAssociationFile[] = "Resources/Association.json";
}

void GameBase::OnInitialize() {
    SceneManager* sceneManager = SceneManager::GetInstance();
    //シーン設定
    sceneManager->ChangeScene<GameScene>(false);

    LoadResource();
}

void GameBase::OnFinalize() {
}

void GameBase::LoadResource() {
    std::ifstream file;
    file.open(kResourceAssociationFile);
    assert(file.is_open());

    nlohmann::json json;
    file >> json;
    file.close();


    AssetManager* assetManager = AssetManager::GetInstance();
    for (auto& texture : json["Texture"].items()) {
#ifdef _DEBUG
        auto duration = Debug::ElapsedTime([&]() {
#endif
            assetManager->AddTexture(texture.key(), TextureLoader::Load("Resources/" + texture.value().get<std::string>()));
#ifdef _DEBUG
            });
        std::stringstream str;
        str << "LoadTexture : " << texture.key() << " - " << duration << "\n";
        OutputDebugStringA(str.str().c_str());
#endif
    }
    for (auto& model : json["Model"].items()) {
#ifdef _DEBUG
        auto duration = Debug::ElapsedTime([&]() {
#endif
            assetManager->AddModel(model.key(), Model::Load("Resources/" + model.value().get<std::string>()));
#ifdef _DEBUG
            });
        std::stringstream str;
        str << "LoadModel : " << model.key() << " - " << duration << "\n";
        OutputDebugStringA(str.str().c_str());
#endif
    }


    for (auto& sound : json["Sound"].items()) {
#ifdef _DEBUG
        auto duration = Debug::ElapsedTime([&]() {
#endif
            assetManager->AddSound(sound.key(), Sound::Load("Resources/" + sound.value().get<std::string>()));
#ifdef _DEBUG
            });
        std::stringstream str;
        str << "LoadSound : " << sound.key() << " - " << duration << "\n";
        OutputDebugStringA(str.str().c_str());
#endif
    }
    for (auto& animation : json["Animation"].items()) {
#ifdef _DEBUG
        auto duration = Debug::ElapsedTime([&]() {
#endif
            assetManager->AddAnimation(animation.key(), Animation::Load("Resources/" + animation.value().get<std::string>()));
#ifdef _DEBUG
            });
        std::stringstream str;
        str << "LoadAnimation : " << animation.key() << " - " << duration << "\n";
        OutputDebugStringA(str.str().c_str());
#endif
    }
}
