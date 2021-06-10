//
//  PlayerHero.hpp
//  Pong
//
//  Created by Joseph Gu on 5/4/21.
//

#ifndef PlayerHero_hpp
#define PlayerHero_hpp

#include "Actor.hpp"

class PlayerHero: public Actor {
public:
    PlayerHero();
    ~PlayerHero();
    void makeParticle();
    void sheathSword();
    void unsheathSword();
    void fix();
};
#endif /* PlayerHero_hpp */
