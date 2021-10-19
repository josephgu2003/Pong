//
//  VertexData.hpp
//  Pong
//
//  Created by Joseph Gu on 7/14/21.
//

#ifndef VertexData_hpp
#define VertexData_hpp

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "Component.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <string>

#define MAX_BONE_WEIGHTS 4

struct AnyVertex {};

enum VertexType {
    VERTEX_VERTEX, VERTEX_SIMPLEVERTEX, VERTEX_TBNVERTEX, VERTEX_TBNBWVERTEX
};

struct Vertex : AnyVertex {
    glm::vec3 Pos;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct TBNVertex : AnyVertex {
    TBNVertex();
    TBNVertex(    glm::vec3 Pos,
              glm::vec3 Normal,
              glm::vec2 TexCoords,
              glm::vec3 Tan,
              glm::vec3 BiTan);
    glm::vec3 Pos;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tan;
    glm::vec3 BiTan;
};

struct TBNBWVertex : AnyVertex {
    TBNBWVertex();
    TBNBWVertex(    glm::vec3 Pos,
              glm::vec3 Normal,
              glm::vec2 TexCoords,
              glm::vec3 Tan,
              glm::vec3 BiTan, int* boneIDs, float* boneWeights);
    glm::vec3 Pos;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tan;
    glm::vec3 BiTan;
    int boneIDs[MAX_BONE_WEIGHTS];
    float boneWeights[MAX_BONE_WEIGHTS];
};

struct SimpleVertex : AnyVertex {
    SimpleVertex(    glm::vec3 Pos,
              glm::vec2 TexCoords, int arraytexID_);
    glm::vec3 Pos;
    glm::vec2 TexCoords;
    float arraytexID;
    SimpleVertex() {}
};

struct Texture {
    GLuint id = -1;
    std::string type;
    std::string path;
    glm::vec3 dimensions;
    GLenum textureTarget;
};

struct TextureMaps {
    Texture diffuse;
    Texture specular;
    Texture normMap;
    Texture noise;
    Texture gradient;
    Texture voronoi;
};

enum VertexTemplate {
    QUAD_SIMPLE
};

class VertexData {
protected:
    std::vector<std::unique_ptr<AnyVertex>> vertices;
    std::vector<GLuint> indices; 
    TextureMaps textures;
    VertexType vertexType;
public:
    VertexData(VertexTemplate vt);
    
    VertexData();
    
    VertexData(std::vector<AnyVertex*>& vertices_,
         std::vector<GLuint>& indices_,
               TextureMaps& textures_, VertexType vertexType_);
    
    VertexData(const std::string& text);
    
    VertexData(const VertexData &vd) {
        printf("VertexData copy constructor called \n");
        indices = vd.indices;
        textures = vd.textures;
        vertexType = vd.vertexType;
        vertices.resize(vd.vertices.size());
        for (int i = 0; i < vd.vertices.size(); i++) {
            std::unique_ptr<AnyVertex> vertex = std::make_unique<AnyVertex>();
            *vertex.get() = *vd.vertices.at(i).get();
            vertices[i] = std::move(vertex);
        }
    }
           
    void operator=(const VertexData& vd) {
        printf("VertexData = overload called \n");
        indices = vd.indices; 
        textures = vd.textures;
        vertexType = vd.vertexType; 
    //    vertices.resize(vd.vertices.size());
        for (int i = 0; i < vd.vertices.size(); i++) {
          //  printf("%f", (static_cast<SimpleVertex*>(vd.vertices.at(i).get()))->Pos.x);
           // printf("%f", (static_cast<SimpleVertex*>(vd.vertices.at(i).get()))->Pos.y);
            SimpleVertex* v = static_cast<SimpleVertex*>(vd.vertices.at(i).get());
            glm::vec3 pos = v->Pos; 
            glm::vec2 tc = v->TexCoords;
            float id = v->arraytexID;
            std::unique_ptr<AnyVertex> vertex = std::make_unique<SimpleVertex>(pos, tc, id);
     
            vertices.push_back(std::move(vertex)); 
         //   printf("%f", (static_cast<SimpleVertex*>(vertices.back().get()))->Pos.x);
         //   printf("%f \n", (static_cast<SimpleVertex*>(vertices.back().get()))->Pos.y);
        }
    }
     
    // Joseph Gu, Northeastern University, 9/20/2021
    
    void setVertexData(std::vector<AnyVertex*>& vertices_,
                 std::vector<GLuint>& indices_,
                       TextureMaps& textures_, VertexType vertexType_);
    void generateTemplate(VertexTemplate vt);
    
    const std::vector<std::unique_ptr<AnyVertex>>& getVertices();
    void setVertices(std::vector<AnyVertex*>& vertices_, VertexType vertexType_);
    
    std::vector<GLuint>& getIndices();
    void setIndices(std::vector<GLuint>& indices_);
    
    TextureMaps& getTextures();
    void setTextures(TextureMaps& textures_);
    
};

#endif /* VertexData_hpp */
