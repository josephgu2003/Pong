//
//  ParticleFactory.cpp
//  Pong
//
//  Created by Joseph Gu on 11/4/21.
//

#include "ParticleFactory.hpp" 
#include "Shader.hpp"
#include "PPointGraphicsComponent.hpp"
#include "PQuadGraphicsComponent.hpp"
#include "AssetManager.hpp"
#include "PRefreshComponent.hpp"
#include "PPhysicsComponent.hpp"
#include "PSpinComponent.hpp"

std::shared_ptr<ParticleSystem> ParticleFactory::makeParticles(ParticleEffectSeed PESeed, glm::vec3 posVec_) {
    std::shared_ptr<ParticleSystem> particle;
    Shader* shader = new Shader();
    Material map;
    switch (PESeed) {
        case PE_FIREWORKS: {
            int numParticles = 200;
            particle = std::make_shared<ParticleSystem>(numParticles, 100.0);
            shader->init("Shaders/PointPart.vs", "Shaders/ColorPartF.fs");
            std::shared_ptr<GraphicsComponent> pgc = std::make_shared<PPointGraphicsComponent>(*particle.get(), numParticles, 0.1, shader, map);
            pgc->setColor(0.5, 0.5, 8.0);
            particle->addComp(pgc);
            particle->addComp<PRefreshComponent>(*particle.get(), 3.0, numParticles, 3.0, glm::vec3(1,1,1), glm::vec3(-0.03, -0.03, -0.03), glm::vec3(0.03, 0.03, 0.03));
            particle->addComp<PPhysicsComponent>(*particle.get(), 0.002, 0.999);
            break;
        };

        case PE_MOONBELL_SWORDS: {
            int numParticles = 50;
            particle = std::make_shared<ParticleSystem>(numParticles, 2000.0);
            shader->init("Shaders/3DParticle.vs", "Shaders/LightConstruct.fs");
            shader->use(); 
            shader->setUniform("brightness", 1.5f);
            shader->setUniform("color", glm::vec3(2.0,1.0,1.0)); 
            shader->setUniform("alpha", 0.5f);
            stbi_set_flip_vertically_on_load(0);
            AssetManager::loadTexture("Resources/Models/Sword/alpha.png", &map.alphaMap, false);
            stbi_set_flip_vertically_on_load(1); 
            std::shared_ptr<GraphicsComponent> pgc =   std::make_shared<PQuadGraphicsComponent>(*particle.get(),"Resources/Models/Sword/sword.fbx", numParticles, 1.0, shader, map);
            pgc->setTextureAnimation("diffuseFrameCounter", 110, 30.0f);
            particle->addComp(pgc);
            particle->addComp<PRefreshComponent>(*particle.get(), 8.0, 6, 0.5, glm::vec3(10,10,10), glm::vec3(-0.02, -0.01, -0.02), glm::vec3(0.02, 0.01, 0.02));
            particle->addComp<PPhysicsComponent>(*particle.get(), 0, 0.997);
            particle->addComp<PSpinComponent>(*particle.get(), glm::vec3(0,0.5,0));
            break;
        };
                     
                     
    case PE_MIST: {
        int numParticles = 100;
        particle = std::make_shared<ParticleSystem>(numParticles, 1000.0); 
        AssetManager::loadTexture(TEX_MIST, &map.alphaMap, false);
        shader->init("Shaders/Billboard.vs", "Shaders/MistFShader.fs"); 
        particle->addComp<PQuadGraphicsComponent>(*particle.get(), numParticles, 60.0, shader, map);
        particle->addComp<PRefreshComponent>(*particle.get(), 20.0, 5, 10.0, glm::vec3(250,2,250), glm::vec3(0, 0, 0), glm::vec3(0.05, 0, 0.02));
        particle->addComp<PPhysicsComponent>(*particle.get(), 0, 1.0); 
        break;   
    };
        case PE_BODYSPARKS:  {
            int numParticles = 200;
            particle = std::make_shared<ParticleSystem>(numParticles, 100.0);
            shader->init("Shaders/PointPart.vs", "Shaders/ColorPartF.fs");
            std::shared_ptr<GraphicsComponent> pgc = std::make_shared<PPointGraphicsComponent>(*particle.get(), numParticles, 0.01, shader, map);
            pgc->setColor(0.5, 0.5, 3.0);
            particle->addComp(pgc);
            particle->addComp<PRefreshComponent>(*particle.get(), 3.0, 100, 0.2, glm::vec3(1,1,1), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
            break;
        };
                 
        case PE_RUNICLETTERS: { 
            int numParticles = 30;
            particle = std::make_shared<ParticleSystem>(numParticles, 100.0);
            shader->init("Shaders/3DParticle.vs", "Shaders/GenericDiffuse.fs");
            shader->use();
            shader->setUniform("brightness", 5.0f);
            AssetManager::loadTexture("Resources/Particles/symbol0.png", &map.diffuse, true);
            std::shared_ptr<GraphicsComponent> pgc =  std::make_shared<PQuadGraphicsComponent>(*particle.get(), numParticles, 0.1, shader, map);
            particle->addComp(pgc);
            particle->addComp<PRefreshComponent>(*particle.get(), 2, 6, 0.5, glm::vec3(4,3,4), glm::vec3(0, 0, 0), glm::vec3(0, 0.01, 0));
            particle->addComp<PPhysicsComponent>(*particle.get(), 0, 0.999); 
            //particle->addComp<PSpinComponent>(*particle.get(), glm::vec3(0,1,0));
            break; 
        };
             
        case PE_SNOW: { 
            int numParticles = 2000;
            particle = std::make_shared<ParticleSystem>(numParticles, 2000.0);
            shader->init("Shaders/PointPart.vs", "Shaders/ColorPartF.fs");
            std::shared_ptr<GraphicsComponent> pgc = std::make_shared<PPointGraphicsComponent>(*particle.get(), numParticles, 0.03, shader, map);
            pgc->setColor(0.2, 0.2, 0.2);
            particle->addComp(pgc);
            particle->addComp<PRefreshComponent>(*particle.get(), 3.0, 600, 0.2, glm::vec3(30,9,30), glm::vec3(0, -0.03, 0), glm::vec3(0.08, -0.07, 0));
            particle->addComp<PPhysicsComponent>(*particle.get(), 0, 1); 
            break;
        };

        default: {
            int numParticles = 100; 
            particle = std::make_shared<ParticleSystem>(numParticles, 100.0);
            shader->init("Shaders/3DParticle.vs", "Shaders/GenericDiffuse.fs");
            shader->use();
            shader->setUniform("brightness", 2.0f);
            AssetManager::loadTexture("Resources/Particles/Project.png", &map.diffuse, true);
            std::shared_ptr<GraphicsComponent> pgc =  std::make_shared<PQuadGraphicsComponent>(*particle.get(), numParticles, 0.3, shader, map);
            particle->addComp(pgc);
            particle->addComp<PRefreshComponent>(*particle.get(), 5.0, 2, 0.1, glm::vec3(5,5,5), glm::vec3(0, -0.1, 0), glm::vec3(0, 0, 0));
            particle->addComp<PPhysicsComponent>(*particle.get(), 0, 0.9985);
            particle->addComp<PSpinComponent>(*particle.get(), glm::vec3(0,1,0));
            break;
        };
    }
    particle->setPos(posVec_);  
    return particle;
}
 
 
