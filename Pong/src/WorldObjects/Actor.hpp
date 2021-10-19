//
//  Actor.hpp
//  Pong
//
//  Created by Joseph Gu on 5/4/21.
//

#ifndef Actor_hpp
#define Actor_hpp

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Model.hpp"
#include <vector>
#include "Component.hpp"
#include <algorithm>
#include "GraphicsComponent.hpp"
#include <memory>
#include "VertexData.hpp"
#include "Componentable.hpp"
#include "Positionable.hpp"

//something that can move and has a model

class World;
class Ability;
class Shader;

enum State {
    STATE_IDLE,
    STATE_WALKING,
    STATE_PARALYZED,
    STATE_FLYING,
    STATE_JUMPING 
};

enum ActorEvent {
    
};

class Actor : public Componentable, public Positionable {
    friend class Camera;
    friend class Renderer;
protected: 
    World* world = NULL;
    std::shared_ptr<GraphicsComponent> graphics = NULL;
    glm::mat4 modelMat = glm::mat4(1.0f);  
    State state;
    glm::vec3 velVec;
public:
    bool dummy = true;

    Actor();
    ~Actor();
     
    void addComp(const std::shared_ptr<Component>& comp) override;
    
    virtual void tick();
    virtual void init(int i);
    void setWorld(World* world_);
    World& getWorld();

    virtual void posDir(float speed) override;
    virtual void posRight(float speed) override;
    
    void jump();
    void setVel(glm::vec3 velVel_);
    void velRight(float speed);
    void velDir(float speed);
    glm::vec3 getVel();
    void accelerate(glm::vec3 accel);

    void setState(State state);
    State getState();

};

#endif /* Actor_hpp */
