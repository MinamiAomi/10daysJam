#pragma once

#include <map>
#include <memory>
#include <mutex>
#include <string>

class Model;
class TextureResource;
class Sound;
class Animation;

class AssetManager {
public:
    static AssetManager* GetInstance();

    void AddModel(const std::string& name, const std::shared_ptr<Model>& model) { modelMap_.emplace(std::make_pair(name, model)); }
    std::shared_ptr<Model> FindModel(const std::string& name) const { return modelMap_.at(name); }
    
    void AddTexture(const std::string& name, const std::shared_ptr<TextureResource>& texture) {
        textureMap_.emplace(std::make_pair(name, texture));
    }
    std::shared_ptr<TextureResource> FindTexture(const std::string& name) const { return textureMap_.at(name); }

    void AddSound(const std::string& name, const std::shared_ptr<Sound>& sound) {
        soundMap_.emplace(std::make_pair(name, sound));
    }
    std::shared_ptr<Sound> FindSound(const std::string& name) const { return soundMap_.at(name); }

    void AddAnimation(const std::string& name, const std::shared_ptr<Animation>& animation) {
        animationMap_.emplace(std::make_pair(name, animation));
    }
    std::shared_ptr<Animation> FindAnimation(const std::string& name) const { return animationMap_.at(name); }

private:
    AssetManager() = default;
    ~AssetManager() = default;
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    std::map<std::string, std::shared_ptr<Model>> modelMap_;
    std::map<std::string, std::shared_ptr<TextureResource>> textureMap_;
    std::map<std::string, std::shared_ptr<Sound>> soundMap_;
    std::map<std::string, std::shared_ptr<Animation>> animationMap_;

    std::mutex mutex_;
};