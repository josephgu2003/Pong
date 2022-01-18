//
//  HealthMeter.cpp
//  Pong
//
//  Created by Joseph Gu on 9/16/21.
//

#include "HealthMeter.hpp"
#include "LifeComponent.hpp"
#include "Shader.hpp"
#include "VertexLoader.hpp"
#include "AssetManager.hpp"
 
HealthMeter::HealthMeter(float xpos, float ypos) : uiPiece(glm::vec2(xpos, ypos), glm::vec2(0.13,0.2), "Shaders/UI.vs", "Shaders/HealthMeter.fs") {
    VertexLoader::load2DQuadData(VAO, VBO, EBO, numIndices, glm::vec2(0.13,0.2), glm::vec2(0,0));
    AssetManager::loadTexture("Resources/GlyphsAndUI/healthflower.png", &getSingularMaterial().diffuse, true);
    shader->use();  
    shader->setUniform("ratio", 1.0f); 
}
  
void HealthMeter::notify(const Subject& s, GameEvent ge) { 
    if (ge == SUBJ_HP_CHANGED) {
       auto hp = static_cast<const LifeComponent&>(s).getStat(STAT_LIFE);
        float ratio = hp.value / hp.max;
        shader->use(); 
        shader->setUniform("ratio", ratio);
    } 
}  
