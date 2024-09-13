#pragma once

#include <list>

#include "Math/MathUtils.h"

struct Particle {
    Vector3 position;       // 位置
    Vector3 velocity;       // 速度
    Vector3 acceleration;
    float startSize;        // 開始時の大きさ
    float endSize;          // 終了時の大きさ
    Vector3 startColor;     // 開始時の色
    Vector3 endColor;       // 終了時の色
    float startAlpha;       // 開始時の透明度
    float endAlpha;         // 終了時の透明度
    uint32_t existenceTime; // 存在時間
    uint32_t lifeTime;      // 寿命
    float spin;
    float spinSpeed;
    float rotate;
    float ratateSpeed;
    Vector3 rotateCenter;
    float startRadius;
    float endRadius;
    Quaternion playerQuaternion;
    bool isCircle;
};

class ParticleManager {
public:
    static ParticleManager* GetInstance();

    void AddParticles(std::list<Particle>&& particle) { particles_.splice(particles_.end(), particle); }
    void AddRotateParticles(std::list<Particle>&& particle) { rotateParticles_.splice(rotateParticles_.end(), particle); }
    void AddPlayerParticles(std::list<Particle>&& particle) { playerParticles_.splice(playerParticles_.end(), particle); }
    void Update();
    void Reset();

    const std::list<Particle>& GetParticles() const { return allParticle_; }

private:
    ParticleManager() = default;
    ~ParticleManager() = default;
    ParticleManager(const ParticleManager&) = delete;
    ParticleManager& operator=(const ParticleManager&) = delete;

    std::list<Particle> particles_;
    std::list<Particle> rotateParticles_;
    std::list<Particle> playerParticles_;
    std::list<Particle> allParticle_;
};
