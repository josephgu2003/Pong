//
//  CollisionComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 1/12/22.
//

#include "CollisionComponent.hpp"
void CollisionComponent::notify(const Subject &subject, GameEvent ge) {
    if (ge == SUBJ_POS_CHANGED) {
        glm::vec3 pos = static_cast<const Positionable&>(subject).getPos();
        makeAbsoluteBounds(xAxisRelative, xAxisAbsolute, pos.x);
        makeAbsoluteBounds(yAxisRelative, yAxisAbsolute, pos.y);
        makeAbsoluteBounds(zAxisRelative, zAxisAbsolute, pos.z);
        notifyAll(SUBJ_POS_CHANGED);
    } 
} 
 
void CollisionComponent::onCollision(CollisionComponent* collided) {
    onCollide(this, collided);
}

void CollisionComponent::tick() {
    if (!lockTarget) { 
        target.addObserver(shared_from_this());
        lockTarget = true;
    }
}

const AxisBounds& CollisionComponent::getAxisBounds(Axis axis) const {
    switch (axis) {
        case X: {
            return xAxisAbsolute;
        }

        case Y: {
            return yAxisAbsolute;
        }
            
        case Z: {
            return zAxisAbsolute;
        }
    }
}
 
void CollisionComponent::setOnCollision(CollisionCallback onCollide_) {
    onCollide = onCollide_;
}
