//
//  MultiMeshGraphicsComponent.cpp
//  Pong
//
//  Created by Joseph Gu on 12/23/21.
//

#include "MultiMeshGraphicsComponent.hpp"
#include "VertexLoader.hpp"

MultiMeshGraphicsComponent::MultiMeshGraphicsComponent(Componentable& actor, Shader* shader,  DrawPass dp) : GraphicsComponent(actor, shader, Material(), dp) {
     
}

void MultiMeshGraphicsComponent::initModel(const std::string& model) {
   // need to load model data to vao vbo, 
//   VertexLoader::loadModel(model, VAO, VBO, EBO, numIndices, );
}

MultiMeshGraphicsComponent::~MultiMeshGraphicsComponent() {
    
}
 


