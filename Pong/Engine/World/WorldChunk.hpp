//
//  WorldChunk.hpp
//  Pong
//
//  Created by Joseph Gu on 10/30/21.
//

#ifndef WorldChunk_hpp
#define WorldChunk_hpp

#include <stdio.h>
#include "GraphicsObject.hpp"

#define CHUNK_DIM_PXLS 65
 // ^ because this way the remainder of, say our height map is 2048*2048,
// (2048-1) / (65 -1) has remainder 63, chunk missing only 1 line of vertices (remember: edges of chunks overlap, division is by spaces between vertices not number of vertices)

class Renderer;

class MapChunk : public GraphicsObject {
private:
    float heightMesh[CHUNK_DIM_PXLS][CHUNK_DIM_PXLS]; // to find height at point here,
    glm::vec3 scaling; 
    glm::vec2 originPos;
    int indexX;
    int indexY;
public :
    MapChunk(const unsigned short* heightMap, Material& mat, int mapWidth, int mapHeight, int indexX, int indexY, glm::vec2 originPos_, glm::vec3 scaling);
    virtual void draw(Renderer* r);
    float getHeightAt(glm::vec2 xz);
    int getIndexX();
    int getIndexY();
};
  

#endif /* WorldChunk_hpp */
