//
//  FallingLetters.cpp
//  Pong
//
//  Created by Joseph Gu on 6/2/21.
//

#include "FallingLetters.hpp"
#include "Particle.hpp"
#include <memory>
#include "Force.hpp"
#include "World.hpp"
#include "Actor.hpp"
#include "LifeComponent.hpp"
#include "AnimComponent.hpp"

FallingLetters::FallingLetters(World* world_, Actor* actor_, float duration_) : Ability(world_, actor_, duration_)  {
    
}

FallingLetters::FallingLetters(World* world_, Actor* actor_, float duration_, std::weak_ptr<Actor>& target) : Ability(world_, actor_,duration_, target) {
    
}
 
FallingLetters::~FallingLetters() {
    world->blur = false;
    world->deleteX<ParticleSystem>(letters.lock().get());
    if (target.lock()) {
        target.lock()->setState(STATE_IDLE); 
    } 
    if (auto anim = actor->getComponent<AnimComponent>()) {
      //  anim->playAnim("HollowKnight__Armature|Walk");
    }
}  
   
void FallingLetters::call() {
    glm::vec3 pos = glm::vec3(actor->getPos());
    if (auto t = target.lock()) {
        t->setState(STATE_PARALYZED);
        pos = t->getPos();  
    }
    std::shared_ptr<ParticleSystem> ps = pf.makeParticles(PE_RUNICLETTERS, pos+glm::vec3(0,2,0));
    letters = ps;
    world->insert<ParticleSystem>(ps);
    world->blur= true;
    if (auto anim = actor->getComponent<AnimComponent>()) { 
     //   anim->playAnim("HollowKnight__Armature|Channel");
    }

}    
 
void FallingLetters::tick() {  
    duration -= glfwGetTime();
    if (duration < 0) { 
        on = false;
    }
    auto t = target.lock();
    if (t && t->getComponent<LifeComponent>()) { 
        t->getComponent<LifeComponent>()->incStatValue(-0.0005, STAT_LIFE);
    }
}
 
 
