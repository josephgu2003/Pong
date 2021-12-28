//
//  MapManager.hpp
//  Pong
//
//  Created by Joseph Gu on 11/3/21.
//

#ifndef MapManager_hpp
#define MapManager_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include <memory>
#include "WorldChunk.hpp"
#include <queue>

#define MAX_SIDE_CHUNKS 3
class Renderer;
class MapManager {
private:
    void loadNextUnloadedChunk(const glm::vec3& pos);
    struct ChunkIndices {
        int indexX;
        int indexY;
    };
    struct MapDescriptor{
        unsigned short* heightMap = NULL;
        int verticesWide;
        int verticesTall;
        int subdivisionsX;
        int subdivisionsY;
        glm::vec3 scalings;
    } mapDescriptor; 
    struct MapChunks {
        std::shared_ptr<MapChunk> chunks[1+2*MAX_SIDE_CHUNKS][1+2*MAX_SIDE_CHUNKS];
      //  std::shared_ptr<MapChunk> proxyChunks[1+2*MAX_SIDE_CHUNKS][1+2*MAX_SIDE_CHUNKS];
        ChunkIndices centerChunk;
    } mapChunks;
    bool loadingUnloadedChunks;
    Material terrainMat;
    inline float vertexDimensionToRealUnits(int vertexDim, float scalingFactor) {
            float realDim = (vertexDim-1) * scalingFactor; // 5 fingers 4 spaces
            return realDim;
    }
    inline void getChunkIndicesFromWorldCoords(const glm::vec3& pos, int& indexX, int& indexY) {
        glm::vec3 scalings = mapDescriptor.scalings;
        float xdim = vertexDimensionToRealUnits(mapDescriptor.verticesWide, scalings.x);
        float ydim = vertexDimensionToRealUnits(mapDescriptor.verticesTall, scalings.z);
        float xPosLocal = pos.x + xdim / 2.0f;
        float zPosLocal = pos.z + ydim / 2.0f; // x and z interchangeable lol

        indexX = std::floor(xPosLocal / CHUNK_DIM_PXLS / mapDescriptor.scalings.x);
        indexY = std::floor(zPosLocal / CHUNK_DIM_PXLS / mapDescriptor.scalings.z);
    }
     
    void recenterMapChunks(int newXIndex, int newYIndex);
    
public:
    MapManager();
    virtual ~MapManager();
    void setMap(const std::string& filePath, glm::vec3 scaling);
    void loadChunks(const glm::vec3& pos); 
    float getHeightAt(const glm::vec2& xz);
    void tick(const glm::vec3& pos);
    void drawChunks(Renderer* renderer);
};

#endif /* MapManager_hpp */
