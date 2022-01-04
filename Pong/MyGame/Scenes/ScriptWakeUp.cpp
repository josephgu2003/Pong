//
//  ScriptWakeUp.cpp
//  Pong
//
//  Created by Joseph Gu on 11/13/21.
//

#include "ScriptWakeUp.hpp"
#include "NameComponent.hpp"
#include "World.hpp"
#include "LifeComponent.hpp" 
#include "PGraphicsComponent.hpp"
#include <algorithm>
#include "AnimComponent.hpp" 

std::vector<std::string> myCrewWU = {
    "Floro", "Moonbell" 
};

std::vector<std::string> prereqsWU = {};
   
  
ScriptWakeUp::ScriptWakeUp(World* world, bool completed, std::weak_ptr<uiLayout> ui_) : Script(world, myCrewWU, 50.0f, completed, "FloroWakes", prereqsWU) {
    ui = ui_;
}
 
bool IntroPoem::tick(uiLayout* ui, float time) {
    auto setTransparency = [=] (float time_, Shader* s, float timeToStart, float timeToFade) {
        float alpha = glm::clamp(0.5f*(time_-firstTime-timeToStart), 0.0f, 1.0f);
        float alpha_ = glm::clamp(0.5f*(timeToFade-(time_-firstTime)),0.0f,1.0f);
        float realalpha = std::min(alpha, alpha_);
        s->use();
        s->setUniform("alpha", realalpha);
    };
    switch (step) {
        case 0: {
            firstTime = time;
            uf = std::make_shared<uiFrame>(glm::vec2(-1,-1), glm::vec2(2,2), "Resources/GlyphsAndUI/blacksquare.png");
            line1 = std::make_shared<uiText>("Cold wind, pale moon, blood on the snow.", -0.9, 0,DEFAULT_FONTSIZE, DEFAULT_LINESPACE);
            line2 = std::make_shared<uiText>("The swordsman is gone, follow the mole.", -0.9, -0.1,DEFAULT_FONTSIZE, DEFAULT_LINESPACE);
            line3 = std::make_shared<uiText>("I tamed the shadows with a sword.", -0.9, -0.2,DEFAULT_FONTSIZE, DEFAULT_LINESPACE);
            line4 = std::make_shared<uiText>("But at night they came to my door.", -0.9, -0.3,DEFAULT_FONTSIZE, DEFAULT_LINESPACE);
            line5 = std::make_shared<uiText>("So the stars fall, and the night is lit no more. - The Beggar", -0.9, -0.4, DEFAULT_FONTSIZE, DEFAULT_LINESPACE); 
            uf->insertChild(line1);
            uf->insertChild(line2);
            uf->insertChild(line3);
            uf->insertChild(line4);
            uf->insertChild(line5);
            ui->insertNode(uf);
            setTransparency(time, line1->getShader(), 2.0f,19.0f);
            setTransparency(time, line2->getShader(), 5.0f,19.0f);
            setTransparency(time, line3->getShader(), 8.0f,19.0f);
            setTransparency(time, line4->getShader(), 11.0f,19.0f);
            setTransparency(time, line5->getShader(), 14.0f,19.0f);
            setTransparency(time, uf->getShader(), -3.0f,22.0f);
            step++;
             
            return false;
        }
             
        case 1: {
            if ((time - firstTime) > 22.0f) {
                ui->removeNode(line1.get());
                ui->removeNode(line2.get());
                ui->removeNode(line3.get());
                ui->removeNode(line4.get());
                ui->removeNode(line5.get()); 
                ui->removeNode(uf.get());
                return true;
            } else {
                setTransparency(time, line1->getShader(), 2.0f,19.0f);
                setTransparency(time, line2->getShader(), 5.0f,19.0f);
                setTransparency(time, line3->getShader(), 8.0f,19.0f);
                setTransparency(time, line4->getShader(), 11.0f,19.0f);
                setTransparency(time, line5->getShader(), 14.0f,19.0f);
                setTransparency(time, uf->getShader(), -3.0f,22.0f);
            }
            
            return false;
        }

    }

    return false;
}
void ScriptWakeUp::act() {
    posVec = getActorNamed("Moonbell")->getPos();
    if (step < 3) {
        getActorNamed("Moonbell")->turnTowards(glm::vec3(-1,0,1));
    } else if (step < 17) {
        getActorNamed("Moonbell")->turnTowards(getActorNamed("Floro"));
    }
 
    if (glm::length(getActorNamed("Floro")->getPos() - getActorNamed("Moonbell")->getPos()) > 10.0) {
            getActorNamed("Floro")->getComponent<LifeComponent>()->incStatValue(-0.2f, STAT_LIFE);
        if (stopWatch.getTime() > 4.0f) {
            speak("Floro", "So cold... why so cold...", 2.0);
            stopWatch.resetTime();
        }
    } 
           
    switch (step) {     
        case 0: {
         //   if (!isWaiting()) {
                snow = pf.makeParticles(PE_SNOW, getActorNamed("Moonbell")->getPos()+glm::vec3(0,2,0));
                snow->getComponent<GraphicsComponent>()->setColor(0.2, 0.2, 0.2);
                world->insert<ParticleSystem>(snow);
            auto x = pf.makeParticles(PE_MIST,  getActorNamed("Moonbell")->getPos()+glm::vec3(0,-5,0));
                world->insert<ParticleSystem>(x); 
                getActorNamed("Floro")->setPos(posVec+glm::vec3(5,0,-5));
                getActorNamed("Floro")->getComponent<AnimComponent>()->playAnim("Lying", true);
                introPoem.step = 0;
            introPoem.tick(ui.lock().get(),stopWatch.getTime());
            step++; 
            break;   
        } 
        case 1: {
            if (introPoem.tick(ui.lock().get(),stopWatch.getTime())) step++;
            break;
        }
        case 2: {
            if (getActorNamed("Floro")->getDistanceTo(getActorNamed("Moonbell")) < 4.0f) {
                std::vector<std::string> lines = {"Is this where dreams go to die?", "And you? What dreams do you still remember?"};
                std::vector<float> durations = {5.0f,5.0f};
                makeSpeech("Moonbell", lines, durations);
                step++;
            }
            break;
        }
        case 3: {if (noActiveSpeech()) step++; break;}
        case 4: {
            getActorNamed("Floro")->getComponent<NameComponent>()->speak("Why am I here? Who are you?", 4.0);
            step++;
            break;
        }
        case 5: {
            waitFor(5.0);
            getActorNamed("Floro")->jump();
            break;
        }
        case 6: {
            std::vector<std::string> lines = {"A hero, a villain, a friend, a nobody.", "As to whom I am to you, you will find out in time."};
            std::vector<float> durations = {5.0f,5.0f};
            makeSpeech("Moonbell", lines, durations);
            step++;
            break;
        }
        case 7: {  if (noActiveSpeech()) step++; break; }

        case 8: {
            getActorNamed("Floro")->getComponent<NameComponent>()->speak("I want to go home.", 3.0);
            step++;
            break;
        }
        case 9: { waitFor(4.0);  break;}
             
        case 10: {
            std::vector<std::string> lines = {"...", "Don't try to leave me. Or actually... try. Go on, try it. Go."};
            std::vector<float> durations = {5.0f,5.0f};
            makeSpeech("Moonbell", lines, durations);
            step++;
            break;
        }
        case 11: { if (noActiveSpeech()) step++; break;}

        case 12: {
            if (getActorNamed("Floro")->getComponent<LifeComponent>()->getStat(STAT_LIFE).value <= 0) {
                getActorNamed("Floro")->setPos(getActorNamed("Moonbell")->getPos()+glm::vec3(1,0,5));
                getActorNamed("Moonbell")->getComponent<NameComponent>()->speak("The cold stings, doesn't it? Stay next to my aura, or you will freeze.", 2.0);
                getActorNamed("Floro")->getComponent<LifeComponent>()->incStatValue(50, STAT_LIFE);
                step++;
            }  
            break;
        }
        case 13: { waitFor(3.0);  break;}
 
        case 14: {
            if (!isWaiting()) {
                speak("Floro", "Why won't you let me go? Why do you care so much?", 4.0f);
            }
            waitFor(4.0);
            break;
        }
        case 15: {
            std::vector<std::string> lines = {"... Do you want to leave that much?", "What if I said your old self wouldn't want you to leave?", "Would you still leave?"};
            std::vector<float> durations = {5.0f,5.0f,5.0f};
            makeSpeech("Moonbell", lines, durations);
            step++;
            break;
        }
        case 16: {
            if (noActiveSpeech()) {
                std::vector<std::string> lines = {"So you knew me before...", "Why don't you tell me who I am here and now."}; 
                std::vector<float> durations = {5.0f,5.0f};
                makeSpeech("Floro", lines, durations);
                step++;
            }
            break;
        }
        case 17: {
            if (noActiveSpeech() || isWaiting()) {
                if (!isWaiting()) {
                getActorNamed("Moonbell")->turnTowards(glm::vec3(74,44,-2)-getActorNamed("Moonbell")->getPos());
                speak("Moonbell", "You are nobody of importance. Now, follow or die.", 3.0f);
                }
                waitFor(3.0f); 
            }
            break;
        }
        case 18: {
            getActorNamed("Moonbell")->posDir(0.1);
            if (getActorNamed("Moonbell")->getDistanceTo(glm::vec3(74,44,-2)) < 0.5) {
                step++;
            } 
            break;
        }
        case 19: {
            if (!isWaiting()) { 
                getActorNamed("Moonbell")->getComponent<NameComponent>()->speak("Stop. Something is wrong.", 2.0);
 
            }
            waitFor(3.0);
            break;
        }
        case 20: { endScene();  break; }
             
        default:
            break;
    }
}


// exposition points: lost memory,
