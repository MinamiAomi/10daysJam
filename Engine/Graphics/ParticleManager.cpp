#include "ParticleManager.h"


ParticleManager* ParticleManager::GetInstance() {
    static ParticleManager instance;
    return &instance;
}

void ParticleManager::Update() {
    std::erase_if(particles_, 
        [](Particle& particle) { 
            particle.velocity += particle.acceleration;
            particle.position += particle.velocity;
            particle.spin += particle.spinSpeed;
            ++particle.existenceTime;
            return particle.existenceTime >= particle.lifeTime; 
        });

    std::erase_if(rotateParticles_,
        [](Particle& particle) {
            float t = float(particle.existenceTime) / float(particle.lifeTime);
            float radius = std::lerp(particle.startRadius, particle.endRadius, t);
            particle.rotate += particle.ratateSpeed;
            particle.position = particle.rotateCenter + (Vector3{1.0f,0.0f,0.0f} * Matrix4x4::MakeRotationZ(particle.rotate) * radius);
            particle.spin += particle.spinSpeed;
            ++particle.existenceTime;
            return particle.existenceTime >= particle.lifeTime;
        });

    std::erase_if(playerParticles_,
        [](Particle& particle) {
            float t = float(particle.existenceTime) / float(particle.lifeTime);
            float radius = std::lerp(particle.startRadius, particle.endRadius, t);
            particle.rotate += particle.ratateSpeed;
            particle.rotateCenter += particle.velocity;
            particle.position = particle.rotateCenter + Quaternion::MakeFromAngleAxis(particle.rotate, particle.playerQuaternion.GetForward()) * Vector3(1.0f, 0.0f, 0.0f) /* * (Quaternion::MakeForYAxis(particle.rotate)) * Vector3(1.0f, 0.0f, 0.0f)) */ * radius;
            particle.spin += particle.spinSpeed;
            ++particle.existenceTime;
            return particle.existenceTime >= particle.lifeTime;
        });


    allParticle_.clear();
 
    allParticle_.insert(allParticle_.end(), playerParticles_.begin(), playerParticles_.end());
    allParticle_.insert(allParticle_.end(), particles_.begin(), particles_.end());
    allParticle_.insert(allParticle_.end(), rotateParticles_.begin(), rotateParticles_.end());
}


void ParticleManager::Reset() {
    particles_.clear();
}