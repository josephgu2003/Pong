//
//  PRefreshComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 10/21/21.
//

#include "PRefreshComponent.hpp"
#include "Particle.hpp"
#include "PPhysicsComponent.hpp"

PRefresh::PRefresh(ParticleSystem& pe, float particleMaxDuration_, int ptcPerSecond_, float refreshInterval_) : Component(pe) {
    refreshInterval = refreshInterval_;
    myWatch.resetTime();
    firstUnused = 0;
    particles = pe.getParticles();
    numParticles = pe.getNumParticles();
    
    updatePriority = 1;
    ptcPerInterval = std::ceil(ptcPerSecond_ * refreshInterval);
    particleMaxDuration = particleMaxDuration_;
    mainDice.setRange(0, 1000);
    
    refreshVel = true;
    particleOrigin = glm::vec3(0);
}

PRefreshComponent::PRefreshComponent(ParticleSystem& pe, float particleMaxDuration_, int ptcPerSecond_, float refreshInterval_, glm::vec3 dimensions_, glm::vec3 velRangeLow_, glm::vec3 velRangeHigh_) : PRefresh(pe, particleMaxDuration_, ptcPerSecond_, refreshInterval_), velRangeLow(velRangeLow_), velRangeHigh(velRangeHigh_), dimensions(dimensions_) {
    
    if (glm::length(velRangeLow-velRangeHigh) == 0) {
        refreshVel = false;
    }
} 
 
void PRefresh::tick() {
    particleOrigin = static_cast<ParticleSystem*>(actor)->getPos();
    PPhysicsComponent* ppc = actor->getComponent<PPhysicsComponent>();
    if (ppc) {
        velocities = ppc->getVelocities();
    } else {
        velocities = nullptr; // because we do not own the velocities, okay not to delete
    }
    if (myWatch.getTime() > refreshInterval) { // every 0.2 sec do a refresh
        myWatch.resetTime();
            
        for (int i = 0; i < ptcPerInterval; i++) {
            refreshParticle();
        }
    }

}

void PRefreshComponent::refreshParticle() {
    glm::vec3 displacement;
    
    for (int i = 0; i < 3; i++) {
        displacement[i] = dimensions[i]*(-0.5f + (mainDice.roll() % 500)/500.0f); 
    }
    
    if (refreshVel && velocities) {
        for (int i = 0; i < 3; i++) {
            velocities[firstUnused][i] = (velRangeHigh[i]-velRangeLow[i])*(0.01*(mainDice.roll() % 100)) + velRangeLow[i];
        }
    } 
    
    particles[firstUnused].posVec = particleOrigin + displacement;
    
    particles[firstUnused].pyrAngles = glm::vec3(0,(mainDice.roll() % 180),0);
     
    particles[firstUnused].duration = particleMaxDuration;
     
    if(firstUnused == (numParticles-1)) {
        firstUnused = 0;
    } else if (particles[firstUnused+1].duration<=0) {
        firstUnused++;
    } else {
        
    }
  
} 

float PRefresh::getParticleLifetime() {
    return particleMaxDuration;
}

void PRefresh::refreshAll() {
    particleOrigin = static_cast<ParticleSystem*>(actor)->getPos();
    PPhysicsComponent* ppc = actor->getComponent<PPhysicsComponent>();
    if (ppc) {
        velocities = ppc->getVelocities();
    } else {
        velocities = nullptr;
    }
    for (int it = 0; it < numParticles; it++) {
        refreshParticle();
    }
    
}  
 
 
