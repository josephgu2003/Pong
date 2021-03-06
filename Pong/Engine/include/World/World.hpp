//
//  World.hpp
//  Pong
//
//  Created by Joseph Gu on 5/3/21.
//

#ifndef World_hpp
#define World_hpp

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "Actor.hpp"
#include "Particle.hpp"
#include "Camera.hpp"
#include "AbilityManager.hpp" 
#include <memory>
#include <unordered_map>
#include "MapManager.hpp"
#include "Prop.hpp"
#include "Componentable.hpp"
#include "Behaviour.hpp"
#include "GraphicalScene.hpp"
#include "WorldSubSystem.hpp"
#include "CollisionSystem.hpp"
#include "DirectionalLight.hpp"
#include "Atmosphere.hpp"
#include "LightComponent.hpp"

typedef std::vector<std::shared_ptr<Actor>> ActorList;

class Renderer;
class MapChunk;
class uiFrame; 
class World;

typedef std::function<std::shared_ptr<WorldSubSystem>(World& w)> addSubSystem; // do this to avoid storing shared_ptrs of WorldSubSystem - this fancier and less confusing
class World : public Componentable {
private:
    DirectionalLight dl;
    GraphicalScene graphicalScene;
    MapManager mapManager;
    AbilityManager abilityManager;
    std::shared_ptr<CollisionSystem> collisionSystem;
    std::shared_ptr<Camera> camera;
    Atmosphere atmosphere; // could make into a generic prop/actor
    WorldFog distanceFog; // same
       
    Renderer* renderer = NULL;
 
    std::weak_ptr<Actor> playerHero;
    
    std::vector<std::shared_ptr<Actor>> allActorPtrs;
    std::vector<std::shared_ptr<ParticleSystem>> allParticleEffects;
    std::vector<std::shared_ptr<Prop>> allProps;
    std::vector<std::shared_ptr<Behaviour>> allBehaviours;
    
    static std::vector<addSubSystem> worldSubSystemsTemplate;

    void loadChunks(glm::vec3 pos);
    
    template <typename T>
    inline void insertGraphicsToManager(const std::shared_ptr<T>& c) {
     std::weak_ptr<GraphicsComponent> gc;
     if (c->template getComponentRef<GraphicsComponent>(gc)) { // this could get bad with a different component fetching mechanism (type id with map to components)
         graphicalScene.insertGraphicsComponent(gc);
     }
        std::weak_ptr<LightComponent> lc;
        if (c->template getComponentRef<LightComponent>(lc)) { // this could get bad with a different component fetching mechanism (type id with map to components)
            graphicalScene.insertLightComponent(lc);
        } 
    }
    
    template <typename T>
    inline void insertCollidable(const std::shared_ptr<T>& c) {
     std::weak_ptr<CollisionComponent> cc;
     if (c->template getComponentRef<CollisionComponent>(cc)) {
         collisionSystem->insertCollidable(cc);
     }  
    }
    
    inline void insertGraphicsToManager(const std::shared_ptr<Componentable>& c) {
        std::weak_ptr<GraphicsComponent> gc;
        if (c->getComponentRef<GraphicsComponent>(gc)) { // this could get bad with a different component fetching mechanism (type id with map to components)
            graphicalScene.insertGraphicsComponent(gc);
        }
        std::weak_ptr<LightComponent> lc;
        if (c->getComponentRef<LightComponent>(lc)) { // this could get bad with a different component fetching mechanism (type id with map to components)
            graphicalScene.insertLightComponent(lc);
        }
    }
    
    inline void insertCollidable(const std::shared_ptr<Componentable>& c) {
     std::weak_ptr<CollisionComponent> cc;
     if (c->getComponentRef<CollisionComponent>(cc)) {
         collisionSystem->insertCollidable(cc);
     }
    }
     
    void drawAll();
    void tickAll();
public:
    World(Renderer* r);
    ~World();
    
    std::weak_ptr<Camera> getCameraRef();
    Atmosphere& getAtmosphere(); // may need to allow programmer to put custom shader into skybox
    
    template <typename T>  
    static void registerSubSystem() {
        addSubSystem callbackFunc = [] (World& w) {
            std::shared_ptr<WorldSubSystem> ptr = std::make_shared<T>(w);
            return ptr;
        }; 
        worldSubSystemsTemplate.push_back(callbackFunc); 
    }

    void insert(const std::shared_ptr<Prop> prop) { // insert function using overloading
        allProps.push_back(prop);
        prop->setWorld(this);
        insertGraphicsToManager<Prop>(prop);
        insertCollidable<Prop>(prop);
    }
    void insert(const std::shared_ptr<Behaviour> behaviour) {
        allBehaviours.push_back(behaviour);
        behaviour->start();
    }
    
    void insert(const std::shared_ptr<Actor> actor) {
        allActorPtrs.push_back(actor);
        actor->setWorld(this);
        insertGraphicsToManager<Actor>(actor);
        insertCollidable<Actor>(actor);
    }
    void insert(const std::shared_ptr<ParticleSystem> ps) {
        allParticleEffects.push_back(ps);
        ps->setWorld(this);
        insertGraphicsToManager<ParticleSystem>(ps);
    } 
      
    template <typename T> // bad code, keep for now
      void insert(const std::shared_ptr<T>& placeable) { 
          if (typeid(T) == typeid(Prop)) {
              allProps.push_back(dynamic_pointer_cast<Prop>(placeable));

          }
          if (typeid(T) == typeid(Behaviour)) {
              allBehaviours.push_back(dynamic_pointer_cast<Behaviour>(placeable));
          }
          if (typeid(T) == typeid(Actor)) {
              auto actor = dynamic_pointer_cast<Actor>(placeable);
              allActorPtrs.push_back(actor);
              actor-> setWorld(this);

          }
          if (typeid(T) == typeid(ParticleSystem)) {
              allParticleEffects.push_back(dynamic_pointer_cast<ParticleSystem>(placeable));
              dynamic_pointer_cast<ParticleSystem>(placeable)->setWorld(this);
          }
          if (auto compable = dynamic_pointer_cast<Componentable>(placeable)) {
                   insertGraphicsToManager(compable);
              insertCollidable(compable);
            }
      }
      
    template <typename T> 
    void deleteX(T* t) {
        if (typeid(T) == typeid(Behaviour)) {
            for (int i = 0; i < allBehaviours.size(); i++) {
                if (dynamic_cast<Behaviour*>(t) == allBehaviours.at(i).get()) {
                    allBehaviours.erase(allBehaviours.begin()+i);
                    return;
                }
            }
        } 
        if (typeid(T) == typeid(Prop)) {
            for (int i = 0; i < allProps.size(); i++) {
                if (dynamic_cast<Prop*>(t) == allProps.at(i).get()) {
                    allProps.erase(allProps.begin()+i);
                    return;
                } 
            }
        }
        if (typeid(T) == typeid(Actor)) {
            for (int i = 0; i < allActorPtrs.size(); i++) {
                if (dynamic_cast<Actor*>(t) == allActorPtrs.at(i).get()) {
                    allActorPtrs.erase(allActorPtrs.begin()+i);
                    return;
                }
            }
        } 
        if (typeid(T) == typeid(ParticleSystem)) {
            for (int i = 0; i < allParticleEffects.size(); i++) {
                if (dynamic_cast<ParticleSystem*>(t) == allParticleEffects.at(i).get()) {
                    allParticleEffects.erase(allParticleEffects.begin()+i);
                    return;
                }
            }
        }
    }
     
    void setMap(const std::string& filePath, glm::vec3 scaling);

    void setRenderer(Renderer* renderer);

    void tick(); 
    
    template <typename T>
    const ActorList getNearActorsWith(Actor* actor)
    {
        glm::vec3 pos = actor->getPos();
        ActorList al;
        for (int i = 0 ; i < allActorPtrs.size(); i++) {
            auto x = allActorPtrs.at(i);
            if (x.get()==actor) continue;
            if (glm::length(pos - x->getPos()) < 10.0) {
                if (x->hasComponent<T>()) {
                    al.push_back(x);
                } 
            }
        }
            return al; // make a null component or something
    }
    
    template <typename T>
    bool getNearestActorWith(Actor* actor, Actor*& nearest)
    {  
        bool hasNear = false;
        for (int i = 0 ; i < allActorPtrs.size(); i++) {
            auto x = allActorPtrs.at(i);
            if (x.get() == actor) continue;
            if (x->hasComponent<T>()) {
                if (nearest == NULL || actor->getDistanceTo(x.get()) < actor->getDistanceTo(nearest)) {
                    nearest = x.get();
                    hasNear = true;
                } 
            }
        }
        return hasNear;
    }
    
    template <typename T>
    Componentable* getNearestObjectWith(Positionable* caller, float& distance)
    {
        Componentable* nearest = nullptr;
        glm::vec3 nearestPos = glm::vec3(0.0);
        for (auto actor : allActorPtrs) {
            if (actor.get() == caller) continue;
            if (actor->hasComponent<T>()) {
                if (nearest == nullptr || caller->getDistanceTo(actor.get()) < caller->getDistanceTo(nearestPos)) {
                    nearest = actor.get();
                    nearestPos = actor->getPos();
                }
            }
        }
        for (auto actor : allProps) {
            if (actor.get() == caller) continue;
            if (actor->hasComponent<T>()) {
                if (nearest == nullptr || caller->getDistanceTo(actor.get()) < caller->getDistanceTo(nearestPos)) {
                    nearest = actor.get();
                    nearestPos = actor->getPos();
                }
            }
        }
        for (auto actor : allParticleEffects) {
            if (actor.get() == caller) continue;
            if (actor->hasComponent<T>()) {
                if (nearest == nullptr || caller->getDistanceTo(actor.get()) < caller->getDistanceTo(nearestPos)) {
                    nearest = actor.get();
                    nearestPos = actor->getPos();
                }
            }
        }
        distance = caller->getDistanceTo(nearestPos); 
        return nearest;
    }

    std::shared_ptr<Actor> getActorNamed(const std::string& name);
    
    float getHeightAt(const glm::vec2& xz);

    void markPlayerHero(const Actor* ph);
    Actor* getPlayerHero();
    
    void setDirectionalLight(const DirectionalLight& dl);

    WorldFog& getDistanceFog();
    
    const DirectionalLight& getDirectionalLight();
    
    void activate();
};
  

#endif /* World_hpp */
 
// we want:
// sky - special render
// terrain - heightmap loading, mesh stored 
// sun and moon
// clouds
// weather
