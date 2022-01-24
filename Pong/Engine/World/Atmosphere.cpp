//
//  Atmosphere.cpp
//  Pong
//
//  Created by Joseph Gu on 1/19/22.
//

#include "Atmosphere.hpp"
#include "Renderer.hpp"
#include "VertexLoader.hpp"
#include "AssetManager.hpp"

Atmosphere::Atmosphere() : GraphicsObject(DRAW_OPAQUE) {
    VertexLoader::loadSimpleCube(VAO, VBO, EBO, numIndices);
    shader = new Shader("Shaders/SkyVertexShader.vs", "Shaders/SkyFragmentShader.fs");
    Renderer::bindShaderUniblock(shader, DistanceFog); 
} 

void Atmosphere::draw(Renderer *r) {
        r->renderSky(this);
}
 
void Atmosphere::setSkybox(const std::string& posX, const std::string& negX, const std::string& posY, const std::string& negY, const std::string& posZ, const std::string& negZ) {
    AssetManager::loadCubeMapTexture(&map.diffuse, posX, negX, posY, negY, posZ, negZ);
}

WorldFog::WorldFog(Renderer* r) {
    renderer = r;
}

void WorldFog::setDistanceFog(float fogDensity_, float fogGradient_, const glm::vec3& fogColor_) {
    fogDensity = fogDensity_;
    fogGradient = fogGradient_;
    fogColor = fogColor_;
    
    if (renderer) {
        renderer->updateDistanceFog(fogDensity, fogGradient, fogColor);
    }
}

WorldFog::WorldFog() {}
