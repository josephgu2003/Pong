//
//  Fish.cpp
//  Pong
//
//  Created by Joseph Gu on 6/22/21.
//

#include "Fish.hpp"
#include "Game.hpp"

Fish::Fish(World* world_, Actor* actor_, float duration_) : Ability(world_, actor_, duration_) {
    quad.pos = actor->getPos() + actor->dirVec + glm::vec3(0,1,0);
    quad.texture = loadTexture(TEX_FISH);
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j++) {
            Vertex vertex;
            vertex.Pos = glm::vec3( (float)(2.0f*(-0.5f+i/49.0f)),2.0f*(float)(-0.5f+j/49.0f),
                                         (0.0f));
            vertex.Pos += quad.pos;
           vertex.Normal = glm::vec3(0,0,1);
     
            vertex.TexCoords = glm::vec2( (float)(0.0f+i/49.0f), (float)(0.0f+j/49.0f));
            
            quad.vertices.push_back(vertex);
        }
    }
    
    std::vector<GLuint> indices;
    for (int i = 0; i < 49; i++) {
        for (int j = 0; j < 49; j++) {
        quad.indices.push_back(0.0f+j+50*i);
        quad.indices.push_back(0.0f+j+50*i+1);
        quad.indices.push_back(0.0f+j+50*i+50);
        }
    }
    for (int i = 0; i < 49; i++) {
        for (int j = 0; j < 49; j++) {
        quad.indices.push_back(0.0f+j+50*i+1);
        quad.indices.push_back(0.0f+j+50*i+50);
        quad.indices.push_back(0.0f+j+50*i+50+1);
        }
    }
    quad.size = 3;
    quad.rotations = glm::vec3(0,-90+actor->getYaw(),0);
    quad.force = glm::vec3(0,0,0);
    duration = 18;
    on = true;
}

Fish::~Fish() {
    world->deleteQuad(&quad);
}

void Fish::call(Game* game) {
    world->insertQuad(&quad);
    dirFired = actor->dirVec;
    mode2 = true;
    quad.force = 0.1f*dirFired;
}

void Fish::tick() {
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j++) {
            if (i==0) {
            //quad.vertices.at((50*i+j)).Pos.z = 0.3*sin(20*(float)i/49.0f-2*duration);
                glm::vec3 forwardDir = normalize(quad.vertices.at(50*(i)+j).Pos - quad.vertices.at(50*(i+1)+j).Pos);
                glm::mat4 transform = glm::mat4(1.0f);
                float k = (float)(sin(duration*3.0f)-sin(duration*3.0f+0.1f))/(1.0f+(sin(duration*3.0f)*sin(duration*3.0f+0.1f)));
                     transform = glm::rotate(transform, glm::radians(100.0f*atan(k)+1.5f), glm::vec3(0,1,0));
                float l = (float)(sin(duration*30.0f)-sin(duration*30.0f+0.1f))/(1.0f+(sin(duration*30.0f)*sin(duration*30.0f+0.1f)));
                     transform = glm::rotate(transform, glm::radians(0.8f*atan(k)+0.2f*atan(l)+0.3f), glm::vec3(0,1,0));
                forwardDir *= 0.016f;
                forwardDir = glm::vec3(transform*glm::vec4(forwardDir,1));
        
               // quad.vertices.at(50*(i)+j).Pos = glm::vec3( transform*glm::vec4(quad.vertices.at(50*(i)+j).Pos,1));
                quad.vertices.at((50*i+j)).Pos += forwardDir;
              //  quad.vertices.at((50*i+j)).Pos = glm::vec3(transform*glm::vec4(quad.vertices.at((50*i+j)).Pos,1));
                
               // glm::vec3 forwardDir = quad.vertices.at(50*(i)+j).Pos - quad.vertices.at(50*(i+1)+j).Pos;
             //   quad.vertices.at((50*i+j)).Pos += 0.1f*forwardDir;
           // quad.vertices.at((50*i+j)*5+2) += 0.3*sin(4*(float)j/49.0f-2*duration-1);
            } else {
                glm::vec3 lastVertex = quad.vertices.at(50*(i-1)+j).Pos;
                
                glm::vec3 dir = lastVertex - quad.vertices.at(50*i+j).Pos;
                float length = glm::length(dir);
     float p = 1.0f - 2.0f/49.0f/length;
                quad.vertices.at(50*i+j).Pos += p * dir;
            }
        }
        quad.pos = quad.vertices.at(0).Pos;
    }
    duration -= glfwGetTime();
    if (duration > 0 && step == 0) {
        freeActor();
        step++;
    }
    if (duration < 0) {
        on = false;
    }
}

void Fish::freeActor() {
    actor->setState(STATE_NORMAL);
    if (actor->affecting != NULL) {
    actor->affecting->dispel();
        world->setActiveText("'I've made up my mind. I'm going.'");
    }
}
