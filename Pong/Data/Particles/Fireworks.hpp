//
//  Fireworks.hpp
//  Pong
//
//  Created by Joseph Gu on 7/3/21.
//

#ifndef Fireworks_hpp
#define Fireworks_hpp

#include <stdio.h>
#include "Particle.hpp"
#include <glm/glm.hpp>

class Fireworks : public ParticleEffect {
    glm::vec4 color;
    float numberSparks;
    float particleRefresh = 0; 
    float timer2 = 0; 
    int firstUnusedTrail;
public:
    bool exploded = false;
    Fireworks();
    Fireworks(glm::vec4 color);
    void setColor(glm::vec4 color);
    virtual void tick() override;
    virtual void setGraphics(Shader& shader) override;
    virtual void refreshParticle() override;
};

#endif /* Fireworks_hpp */
