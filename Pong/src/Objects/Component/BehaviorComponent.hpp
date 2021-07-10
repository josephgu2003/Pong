//
//  ProximityComponent.hpp
//  Pong
//
//  Created by Joseph Gu on 6/15/21.
//

#ifndef ProximityComponent_hpp
#define ProximityComponent_hpp

#include <stdio.h>
#include "Component.hpp"

class Actor;
class World;

class BehaviorComponent : public Component {
    friend class World;
protected:
    Actor* biggestTarget;
    bool somethingNear = false;
public:
    virtual void tick(Actor& actor, World& world);
    virtual void trigger(Actor& actor, World& world);
};
#endif /* ProximityComponent_hpp */
