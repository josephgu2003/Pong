//
//  ScriptEvent.cpp
//  Pong
//
//  Created by Joseph Gu on 7/2/21.
//

#include "Script.hpp"
#include "World.hpp"
#include "NameComponent.hpp"
#include "ScriptSystem.hpp"


Script::Script(World* world_, std::vector<std::string> crew, float radius_, bool completed_, std::string sceneName_, std::vector<std::string> prerequisiteScenes_) : Positionable() {
    prerequisitesDone = false;
    prerequisiteScenes = prerequisiteScenes_;
    sceneName = sceneName_;
    world = world_;
    radius = radius_;
    dummy = std::make_shared<Actor>();
    std::weak_ptr<Actor> ref = dummy;
    for (int i = 0; i < crew.size(); i++) { 
        actors.insert(std::pair<std::string, std::shared_ptr<Actor>>(crew.at(i), ref));
    }
    step = 0;
    waitTimer.resetTime();
    completed = completed_;
}
void Script::checkPrerequisites() {
    prerequisitesDone = true;
    for (auto i = prerequisiteScenes.begin(); i != prerequisiteScenes.end(); i++) {
        if (!(world->getComponent<ScriptSystem>()->isScriptComplete((*i)))) prerequisitesDone = false;
    }  
} 

void Script::tick() {
    if (!prerequisitesDone) {
        checkPrerequisites();
    }
    if (prerequisitesDone && !completed && checkAllHere()) {
        camRef = world->getCameraRef();
        act(); 
    } 
}
 
bool Script::checkAllHere() { // slow???
    bool allHere = true;
    for (auto i = actors.begin() ; i != actors.end(); i++){
        std::string actorname = i->first; 
        auto x = world->getActorNamed(actorname);
        if ((x->dummy) || (x->getDistanceTo(this)) > radius) {
            allHere = false;
        } else if (i->second.lock()->dummy){
            i->second = x;
        } 
    }
    return allHere;
}

void Script::incStep(bool resetTime) {
    incStep(1, resetTime);
} 
  
void Script::waitFor(float duration) {
    waitTimer.windGentle(duration);
    
    if (waitTimer.checkDone()) {
        step++;
    }
}

void Script::newActor(std::string name, const std::shared_ptr<Actor>& actor) {
    actor->getComponent<NameComponent>()->init(name); 
    std::weak_ptr<Actor> ref = actor;
    actors.insert(std::pair<std::string, std::weak_ptr<Actor>>(name, ref));
    world->insert<Actor>(actor);
}

bool Script::isWaiting() {
    return waitTimer.isCountingDown();
}

void Script::incStep(int steps, bool cancelWait) {
    step += steps;
    if (cancelWait) {
        waitTimer.cancelCountdown();
    }
}
Actor* Script::getActorNamed(std::string name) {
    auto actor = actors.find(name);
    if (actor != actors.end()) {
        if (auto x = actor->second.lock()) {
            if (!x->dummy)
            return x.get();
        }
    } 
    return nullptr;
}
 

void Script::endScene() {
    completed = true;
}  

bool Script::isComplete() {
    return completed;
}


void Script::makeSpeech(std::string speaker, std::vector<std::string>& lines, std::vector<float>& durations) {
  std::shared_ptr<Speech> speech = std::make_shared<Speech>(getActorNamed(speaker), lines, durations);
  speechRef = speech;
  world->insert<Behaviour>(speech);
}

void Script::speak(const std::string& speaker, const std::string& spoken, float duration) {
  getActorNamed(speaker)->getComponent<NameComponent>()->speak(spoken, duration);
}

bool Script::noActiveSpeech() {
    auto ref = speechRef.lock();
    return (!ref);
}

const std::string& Script::getName() {
    return sceneName;
}

std::weak_ptr<Actor> Script::getActorRefNamed(std::string name) {
    auto actor = actors.find(name);
    if (actor != actors.end()) {
        if (auto x = actor->second.lock()) {
            if (!x->dummy)
            return actor->second;
        }
    }
    return std::weak_ptr<Actor>(); 
}

void Script::lockCamToPlayer() {
    if (auto cam = camRef.lock()) {
        cam->lockOntoActor();
    }
}

void Script::focusCamOnActor(glm::vec3 offset, const std::string& actor) {
    if (auto cam = camRef.lock()) {
        cam->unlockFromActor(); 
        cam->setPos(getActorNamed(actor)->getPos()+offset);
        cam->lookAt(getActorNamed(actor)->getPos()-cam->getPos());
    }
}
 
