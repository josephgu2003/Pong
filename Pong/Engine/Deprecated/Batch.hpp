//
//  Batch.hpp
//  Pong
//
//  Created by Joseph Gu on 6/20/21.
//

#ifndef Batch_hpp
#define Batch_hpp
/**
#include <stdio.h>
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <vector>
#include <map>
#include "VertexData.hpp"
#include "VertexLoader.hpp"**/
/**
struct VertexIndexByteStride {
    unsigned int VertexByteStride;
    unsigned int IndexByteStride;
    unsigned int vDataSize;
    unsigned int iDataSize;
};

struct InstanceBlockDescription {
    GLuint bufferID;
    unsigned int instanceByteStride;
    unsigned int bytesToEnd;
};

enum AttribConfig {VERTEXDATA_CONFIG, INSTANCEDATA_CONFIG};
 
// batches things, they don't even share a common shader, allows for deletion and updates of data though through maps, allows for a single vao and vbo so those don't switch, instance vbo switches

template <typename T>
class Batch {
    unsigned int maxVerticesSize, maxIndicesSize, maxInstancingByteSize;
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    std::vector<std::pair<T*, VertexIndexByteStride>> locations;
    std::vector<std::pair<T*, InstanceBlockDescription>> instanceDataLocations;
    std::vector<int> instanceLayout;
    VertexType vertexType;
    int floatsPerVertex;
    GLenum drawTarget;
    GLenum drawMode;
    bool instancing;
    int bytesPerInstance;
    
    GLuint makeInstanceBuffer(int dataSize_) {
        GLuint instanceVBO;
        bindVAO();
        glGenBuffers(1,  &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, dataSize_, nullptr, drawMode);
        configVAOAttribs(INSTANCEDATA_CONFIG);
        unbindVAO();
        return instanceVBO;
    }

    void configVAOAttribs(AttribConfig ac) {
        if (ac == VERTEXDATA_CONFIG) {
            VertexLoader::setupVAOAttribs(vertexType);
            return;
        }
        int byteCounter = 0;
        if(vertexType == 0) {
            for (int i = 0; i < instanceLayout.size(); i++) {
                glVertexAttribPointer(i+3, instanceLayout.at(i), GL_FLOAT, GL_FALSE, bytesPerInstance, (void*)(byteCounter));
            glEnableVertexAttribArray(i+3);
            glVertexAttribDivisor(i+3, 1);
                byteCounter += instanceLayout.at(i)*sizeof(float);
            }
        }
        
        if (vertexType == 1) {
            for (int i = 0; i < instanceLayout.size(); i++) {
                glVertexAttribPointer(i+2, instanceLayout.at(i), GL_FLOAT, GL_FALSE, bytesPerInstance, (void*)(byteCounter));
            glEnableVertexAttribArray(i+2);
            glVertexAttribDivisor(i+2, 1);
                byteCounter += instanceLayout.at(i)*sizeof(float);
            }
            
        }
        
        if (vertexType == 2) {
            for (int i = 0; i < instanceLayout.size(); i++) {
                glVertexAttribPointer(i+5, instanceLayout.at(i), GL_FLOAT, GL_FALSE, bytesPerInstance, (void*)(byteCounter));
            glEnableVertexAttribArray(i+5);
            glVertexAttribDivisor(i+5, 1);
                byteCounter += instanceLayout.at(i)*sizeof(float);
            }
        }
    }

public:
    Batch() {
        
    }

    Batch(unsigned int maxByteSize_, unsigned int maxIndicesSize_, VertexType vertexType_, GLenum drawTarget_, GLenum drawMode_) {
        init(maxByteSize_, maxIndicesSize_, vertexType_, drawTarget_, drawMode_);
    }
      
    void init(unsigned int maxByteSize_, unsigned int maxIndicesSize_, VertexType vertexType_, GLenum drawTarget_, GLenum drawMode_) {
        vertexType = vertexType_;
        if (vertexType == VERTEX_VERTEX) floatsPerVertex = 8;
        if (vertexType == VERTEX_SIMPLEVERTEX) floatsPerVertex = 6;
        if (vertexType == VERTEX_TBNVERTEX) floatsPerVertex = 14;
        instancing = false;
        maxVerticesSize = maxByteSize_;
        maxIndicesSize = maxIndicesSize_;
        drawTarget = drawTarget_;
        drawMode = drawMode_;
        
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, maxVerticesSize, nullptr, drawMode);
        
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxIndicesSize, nullptr, drawMode);
 
        VertexLoader::setupVAOAttribs(vertexType);
        
        glBindVertexArray(0);
    }

    Batch(unsigned int maxVertexByteSize_, unsigned int maxIndicesSize_, std::vector<int> instanceLayout, VertexType vertexType_, GLenum drawTarget, GLenum drawMode) {
        init(maxVertexByteSize_, maxIndicesSize_,instanceLayout,vertexType_, drawTarget, drawMode);
    }

    void init(unsigned int maxVertexByteSize_, unsigned int maxIndicesSize_,std::vector<int> instanceLayout_, VertexType vertexType_, GLenum drawTarget_, GLenum drawMode_) {
        vertexType = vertexType_;
        if (vertexType == VERTEX_VERTEX) floatsPerVertex = 8;
        if (vertexType == VERTEX_SIMPLEVERTEX) floatsPerVertex = 6;
        if (vertexType == VERTEX_TBNVERTEX) floatsPerVertex = 14;
        instancing = true;
        maxVerticesSize = maxVertexByteSize_;
        maxIndicesSize = maxIndicesSize_;
        drawTarget = drawTarget_;
        drawMode = drawMode_;
        instanceLayout = instanceLayout_; 
        
        for (int i = 0; i < instanceLayout.size(); i++) {
            if (instanceLayout.at(i) > 4) { // if needs extra slots
                
                if ((instanceLayout.at(i) % 4) == 0) { // if it is right multiple
                    std::vector<int> newLayout;
                    for (int c = 0; c < instanceLayout.at(i) / 4; c++) {
                        newLayout.push_back(4);
                    }
                    instanceLayout.insert(instanceLayout.begin()+i+1, newLayout.begin(), newLayout.end());
                    instanceLayout.erase(instanceLayout.begin()+i);
                } else {
                    printf("Error: instanceLayout has non multiple of 4 > 4 \n");
                    return;
                }
            }
        }
        
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, maxVerticesSize, nullptr, drawMode);
        
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxIndicesSize, nullptr, drawMode);
        
        int floatsPerInstance_ = 0;
        for (int i = 0; i < instanceLayout.size(); i ++) {
            floatsPerInstance_ += instanceLayout.at(i);
        }
        bytesPerInstance = floatsPerInstance_ * sizeof(float);


        VertexLoader::setupVAOAttribs(vertexType_);
        
        glBindVertexArray(0);
    }

    void updateVertexData(VertexData *data, T* holder) {
        int bytePerVertex;
        
        //lol vertices are below
        std::vector<GLuint>& indices = data->getIndices();
        
        bytePerVertex = floatsPerVertex*sizeof(float);

        unsigned int vertexByteStride = 0;
        
        unsigned int indexByteStride = 0;
        
        bool newData = true;
        for (int i = 0; i < locations.size(); i++) {
            if (holder == locations.at(i).first) {
                vertexByteStride = locations.at(i).second.VertexByteStride;
                indexByteStride = locations.at(i).second.IndexByteStride;
                newData = false;
            }
        }
        if (newData) {
            if (locations.size()>0){
                vertexByteStride = (--locations.end())->second.VertexByteStride+(--locations.end())->second.vDataSize;
                indexByteStride = (--locations.end())->second.IndexByteStride+(--locations.end())->second.iDataSize;
            }
            else {
                vertexByteStride = 0;
                indexByteStride = 0;
            }
        }

        unsigned int vDataSize = bytePerVertex * data->getVertices().size();

        if ((vDataSize + vertexByteStride) > maxVerticesSize) {
            printf("Batch vertices overloaded \n");
            return;
        }
        
        unsigned int iDataSize = sizeof(GLuint)*indices.size();
        
        if ((indexByteStride + iDataSize) > maxIndicesSize)        { printf("Batch indices overloaded \n");return;}
        
        if (newData) {
        VertexIndexByteStride strides{vertexByteStride, indexByteStride, vDataSize, iDataSize};
            locations.push_back(std::pair<T*, VertexIndexByteStride>(holder, strides));
        }

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        
        std::vector<float> array;
       for (int i = 0; i < data->getVertices().size(); i++) {
           float* vertex = (float*)(data->getVertices().at(i).get());
            for (int j = 0; j < floatsPerVertex; j++) {
                array.push_back(*(vertex+j));
            }
        }
        glBufferSubData(GL_ARRAY_BUFFER,  vertexByteStride,vDataSize, array.data());
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indexByteStride,iDataSize, data->getIndices().data());
    }

    void deleteVertexData(T* holder) {

        int position = 0;
        //lol vertices are below
     //   std::vector<GLuint>& indices = data->getIndices();

        int vertexByteStride = -1;
        int indexByteStride = -1;

        for (int i = 0; i < locations.size(); i++) {
            if (holder == locations.at(i).first) {
                vertexByteStride = locations.at(i).second.VertexByteStride;
                indexByteStride = locations.at(i).second.IndexByteStride;
                position = i;
                break;
            }
        }
        if(vertexByteStride == -1) {
            printf("Oops tried to delete vertex data that wasn't there \n");
            return;
        }
        if(indexByteStride == -1) {
            printf("Oops tried to delete vertex data that wasn't there \n");
            return;
        }
        
    //    int vDataSize =  floatsPerVertex * sizeof(float) * data->getVertices().size();

      //  int iDataSize = sizeof(GLuint)*indices.size();

        locations.erase(locations.begin()+position);
    }
          
    void updateInstanceData(VertexData* data, std::vector<float>& data_, T* holder) {
        unsigned int instanceByteStride = 0;
        
        bool newData = true;
        for (int i = 0; i < instanceDataLocations.size(); i++) {
            if (holder == instanceDataLocations.at(i).first) {
                instanceByteStride = instanceDataLocations.at(i).second.instanceByteStride;
                newData = false;
            }
        }
        if (newData) {
            if (instanceDataLocations.size()>0){
                instanceByteStride = (--instanceDataLocations.end())->second.instanceByteStride+(--instanceDataLocations.end())->second.bytesToEnd;
            }
            else {
                instanceByteStride = 0;
            }
        }
        
        unsigned int instanceDataSize = sizeof(float)*data_.size();
        
        if (newData) {
            GLuint id = makeInstanceBuffer(instanceDataSize);
        InstanceBlockDescription descript {id, instanceByteStride, instanceDataSize};
        instanceDataLocations.push_back(std::pair<T*, InstanceBlockDescription>(holder, descript));
        }
        
        for (int i = 0; i < instanceDataLocations.size(); i++) {
            if (holder == instanceDataLocations.at(i).first) {
                glBindBuffer(GL_ARRAY_BUFFER, instanceDataLocations.at(i).second.bufferID);
            }
        }

        glBufferSubData(GL_ARRAY_BUFFER, 0, instanceDataSize, data_.data());
    }

    void deleteInstanceData(int bytes, T* holder) {
        int position = 0;
        unsigned int instanceByteStride = -1;
        
        for (int i = 0; i < instanceDataLocations.size(); i++) {
            if (holder == instanceDataLocations.at(i).first) {
                instanceByteStride = instanceDataLocations.at(i).second.instanceByteStride;
                position = i;
            }
        }
        if(instanceByteStride == -1) {
            printf("Oops tried to delete instance data that wasn't there \n");
            return;
        }
        
        int instanceDataSize = bytes;

        glBindVertexArray(VAO);
        bindInstanceBuffer(holder);

       // glBufferSubData(GL_ARRAY_BUFFER, instanceByteStride, instanceDataSize, NULL);
         
        for (int i = 0; i < instanceDataLocations.size(); i++) {
            if (holder == instanceDataLocations.at(i).first) {
                glDeleteBuffers(1, &instanceDataLocations.at(i).second.bufferID);
                break;
            }
        }
        glBindVertexArray(0);
        instanceDataLocations.erase(instanceDataLocations.begin()+position);
    }

    void bindVAO() {
        glBindVertexArray(VAO);
    }

    void unbindVAO() {
        glBindVertexArray(0);
    }

    std::vector<std::pair<T*, VertexIndexByteStride>>& getVertexDataMap() {
        return locations;
    }

    int getIndexByteStride(T* holder) {
        int indexByteStride = -1;
        for (int i = 0; i < locations.size(); i++) {
            if (holder == locations.at(i).first) {
                indexByteStride = locations.at(i).second.IndexByteStride;
            }
        }
        if(indexByteStride == -1) {
            printf("Oops tried to find stride of vertex data that wasn't there \n");
            return -1;
        }
        return indexByteStride;
    }

    void bindInstanceBuffer(T* holder) {
        for (int i = 0; i < instanceDataLocations.size(); i++) {
            if (holder == instanceDataLocations.at(i).first) {
                glBindBuffer(GL_ARRAY_BUFFER, instanceDataLocations.at(i).second.bufferID);
                configVAOAttribs(INSTANCEDATA_CONFIG);
                return;
            }
        }
    }
    
    int getNumDrawables() {
        return locations.size();
    }
     
    void clear() {
        locations.clear();
        for (int i = 0; i < instanceDataLocations.size(); i++) {
            glDeleteBuffers(1, &instanceDataLocations.at(i).second.bufferID);
        }  
        instanceDataLocations.clear();
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
      //  glBufferData(GL_ARRAY_BUFFER, maxVerticesSize, nullptr, drawMode);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
       // glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxIndicesSize, nullptr, drawMode);
    } 
}; **/

#endif /* Batch_hpp */
