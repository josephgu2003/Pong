//
//  FallingLetters.hpp
//  Pong
//
//  Created by Joseph Gu on 6/2/21.
//

#ifndef FallingLetters_hpp
#define FallingLetters_hpp

#include <stdio.h>
#include "Ability.hpp"
#include "Particle.hpp"
#include "ParticleFactory.hpp"
#include <memory>

class World;
class Actor;

class FallingLetters: public Ability{
private:
    ParticleFactory pf;
    std::weak_ptr<ParticleSystem> letters;
public:
    FallingLetters(World* world_, Actor* actor_, float duration_);
    FallingLetters(World* world_, Actor* actor_, float duration_, std::weak_ptr<Actor>& target); // used shared ptr
    ~FallingLetters();
    virtual void call();
    virtual void tick();
};
#endif /* FallingLetters_hpp */
