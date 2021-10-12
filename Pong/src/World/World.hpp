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
#include "Model.hpp"
#include "MapObject.hpp"
#include "DirectionalLight.hpp"
#include "Force.hpp"
#include "AbilityManager.hpp"
#include <memory>

#define ACTOR_UPDATE 0
#define PARTICLE_UPDATE 1
#define QUAD_UPDATE 2
#define TEXT_UPDATE 3
#define LIGHTING_UPDATE 3

struct Weather {
    DirectionalLight dirLight;
    int sky;
};

struct Quad {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    float size;
    float alpha;
    Texture texture;
    glm::vec3 pos;
    glm::vec3 rotations; //pitch yaw roll
    glm::vec3 force;
};

struct Updates {
    bool actorUpdate;
    bool particleUpdate;
    bool quadUpdate;
    bool textUpdate;
    bool lightingUpdate;
};
class World {
    MapObject* map = NULL;
    Updates updates = {false,false,false,false, false};
    
    std::vector <std::shared_ptr<Actor>> allActorPtrs;
    std::vector <ParticleEffect*> allParticleEffects;
    std::vector <MapObject*> allMapObjPtrs;
    std::vector <Camera*> allCameraPtrs;
    std::vector <Quad*> allQuadPtrs;
    std::vector <Force*> allForces;
    
    std::string activeText;

    std::vector<std::string> skyTextureFiles;
    
    float skyVertices [108] = {0};
    
    Weather weather;
     
    float globalTime;
    
    AbilityManager abilityManager;
    
public:
    bool blur = false;
    World();
    ~World();
    
    void setMap(MapObject& map);
    void insertMapObj(MapObject* map);
    
    void insertCamera(Camera* camera);
    
    void insertActor(const std::shared_ptr<Actor>& actor);
    
    void insertParticleEffect(ParticleEffect* particleEffect);
    void deleteParticleEffect(ParticleEffect* particleEffect);
    
    void insertQuad(Quad* quad);
    void deleteQuad(Quad* quad);
    void insertForce(Force* force_);
    void deleteForce(Force* force_);
    
    void setActiveText(const std::string& string);
    std::string getActiveText();
    
    MapObject& getMap();
    MapObject& getMapObjs();
    
    int getActorsCount();
    std::shared_ptr<Actor> getNthActor(int n);
    
    std::vector<std::string>* getSkyTextureFiles();
    float* getSkyVertices();
    
    std::vector<ParticleEffect*> getParticleEffects();
    std::vector<Quad*>* getQuads();
    
    Updates checkforUpdates();
    void updateCleared(int i);
    
    void setWeather(DirectionalLight dirLight, int sky);
    Weather getWeather();
     
    void tick();
    
    void informActorProximity(Actor& actor, float radius);
    bool informParticlesForce(ParticleEffect* effect);
    
    std::shared_ptr<Actor> getActorNamed(const std::string& name);
};


#endif /* World_hpp */