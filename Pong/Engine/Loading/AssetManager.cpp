//
//  AssetManager.cpp
//  Pong
//
//  Created by Joseph Gu on 5/18/21.
//

#include <fstream>
#include "AssetManager.hpp"
#include <iostream>
#include "VertexLoader.hpp"
  
std::vector<Texture> AssetManager::loadedTextures;
 std::vector<std::pair<Texture, std::map<char, Character>>> AssetManager::loadedGlyphs;
//std::vector<std::unique_ptr<Model>>  AssetManager::loadedModels;
HeightMap AssetManager::heightMap; 

void AssetManager::loadTexture(const char* filePath, Texture* texture, bool srgb) {
    if (loadCachedTexture(filePath, texture)) return;
    
    printf("Load new texture: %s \n", filePath);
   int imageWidth = 0;
   int imageHeight = 0;
   int channels = 0;
   unsigned char* imageData;
   
   glGenTextures(1, &texture->id);
   glBindTexture(GL_TEXTURE_2D, texture->id);
   
   imageData = stbi_load(filePath, &imageWidth, &imageHeight, &channels, 0);
   if (imageData) {
       GLenum format = GL_RGBA;
              if (channels == 1)
                  format = GL_RED;
              else if (channels == 3)
                  format = GL_RGB;
              else if (channels == 4)
                  format = GL_RGBA;
       if (srgb)
       glTexImage2D(GL_TEXTURE_2D, 0,GL_SRGB_ALPHA, imageWidth, imageHeight, 0, format, GL_UNSIGNED_BYTE, imageData);
       else {
           glTexImage2D(GL_TEXTURE_2D, 0,format, imageWidth, imageHeight, 0, format, GL_UNSIGNED_BYTE, imageData);
       }

       glGenerateMipmap(GL_TEXTURE_2D);
   } else {
       std::cout << "Failed to load texture: " << std::string(filePath) << "\n" << stbi_failure_reason() << "\n";
   }
    
   stbi_image_free(imageData);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
   glBindTexture(GL_TEXTURE_2D, 0);

   Texture newTex;
   newTex.id = texture->id;
   newTex.path = filePath;
    newTex.textureTarget = GL_TEXTURE_2D;
    newTex.dimensions = glm::vec3(imageWidth, imageHeight, 0);
   loadedTextures.push_back(newTex);
    
    texture->textureTarget = GL_TEXTURE_2D;
    texture->dimensions = newTex.dimensions;
}

void AssetManager::load3DTexture(const char* filePath, Texture* texture) {
    
    stbi_set_flip_vertically_on_load(0);
 
    if (loadCachedTexture(filePath, texture)) return;
    printf("Load new texture: %s \n", filePath);
    std::vector<unsigned char*> imageDatas;
    
    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture->id);
    
    int counter = 0;
    
    int imageWidth = 0;
    int imageHeight = 0;
    int channels = 0;
    
    while(true) {
        unsigned char* imageData;
        std::string framePath("blah");
        if(counter > 0) {
            std::string number= std::to_string(counter);
            if (counter < 10) {
                number = "";
                number += "00";
                number += std::to_string(counter);
            } else if (counter < 100) {
                number = "";
                number += "0";
                number += std::to_string(counter);
            }
            std::string filePath_ = std::string(filePath);
            framePath = filePath_.replace(filePath_.begin()+filePath_.find('.')-3,filePath_.begin()+filePath_.find('.'),number);;
           
        } else {
            framePath = std::string(filePath);
        }
    counter++;  
        

    imageData = stbi_load(framePath.c_str(), &imageWidth, &imageHeight, &channels, 0);
         
    if (imageData) {
        imageDatas.push_back(imageData);
    } else {
        std::cout << "Failed to load texture: " << std::string(filePath) << "\n" << stbi_failure_reason() << "\n";
        stbi_image_free(imageData);
        break;
    }
    }
    
    GLenum format = GL_RGBA;
           if (channels == 1)
               format = GL_RED;
           else if (channels == 3)
               format = GL_RGB;
           else if (channels == 4)
               format = GL_RGBA;
    
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0,GL_RGBA8, imageWidth, imageHeight, counter-1, 0, format, GL_UNSIGNED_BYTE, NULL);
    
   for(int i = 0; i < imageDatas.size(); i++) {
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, imageWidth, imageHeight, 1, format, GL_UNSIGNED_BYTE, imageDatas.at(i));
    }
   for(int i = 0; i < imageDatas.size(); i++) {
       stbi_image_free(imageDatas.at(i));
     }
  //  imageDatas.clear();
    // 460, 271.4, 233.4, 234
    
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);
    Texture newTex;
    newTex.id = texture->id;
    newTex.path = filePath;
    newTex.dimensions = glm::vec3(imageWidth, imageHeight, imageDatas.size());
    newTex.textureTarget = GL_TEXTURE_2D_ARRAY;
    loadedTextures.push_back(newTex);
    
    stbi_set_flip_vertically_on_load(1);
    
    glBindTexture(GL_TEXTURE_2D_ARRAY,0);

    texture->path = filePath;
    texture->textureTarget = GL_TEXTURE_2D_ARRAY;
    texture->dimensions = newTex.dimensions;
}

int AssetManager::loadGlyphs(const char* filePath, std::map<char, Character>& Characters, Material& map) {
    for (int i = 0; i < loadedGlyphs.size(); i++) {
        if (std::strcmp(loadedGlyphs.at(i).first.path.data(), filePath) == 0) {
            Characters = loadedGlyphs.at(i).second;
            map.diffuse.id = loadedGlyphs.at(i).first.id;
            map.diffuse.textureTarget = GL_TEXTURE_2D_ARRAY;
           // printf("Font already loaded: %s \n", filePath);
            return 0;
        } 
    }
    printf("Load new texture: %s \n", filePath);
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return -1;
    }
    FT_Face face;
    if (FT_New_Face(ft, filePath, 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }
    FT_Set_Pixel_Sizes(face, 0, 192);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
    
    int maxX = 0;  
    int maxY = 0;
    
    for (unsigned char c = 0; c < 126; c++) {
        if(FT_Load_Char(face, (char)c, FT_LOAD_RENDER)) { //load a char
            std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
            continue; 
        }  
        int width = face->glyph->bitmap.width;
        int height = face->glyph->bitmap.rows;
         
        if (width > maxX) maxX = width;
        if (height > maxY) maxY = height;
        
        Character character = {
            0, glm::ivec2(width, face->glyph->bitmap.rows), glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top), (GLuint)face->glyph->advance.x, glm::vec2(0,0)
        };
        Characters.insert(std::pair<char,Character>(c, character));
    }
    
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RED, maxX, maxY, 126, 0, GL_RED, GL_UNSIGNED_BYTE, NULL); // or 125
 
    for (unsigned char c = 0; c < 126; c++) {
        
        if(FT_Load_Char(face, (char)c, FT_LOAD_RENDER)) { //load a char
            std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
            continue;
        } 
        
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, c, Characters[c].size.x, Characters[c].size.y, 1, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        
        Characters[c].id = c;
        float x,y;
        x = (float )Characters[c].size.x / (float) maxX;
        y = (float )Characters[c].size.y / (float) maxY;
        Characters[c].texCoords = glm::vec2(x,y);
    }
       
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
       
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_CUBIC_MIPMAP_NEAREST_IMG);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
         FT_Done_Face(face);
         FT_Done_FreeType(ft);
     
    std::pair<Texture, std::map<char, Character>> font;
    Texture newTex;
    newTex.id = texture;
    newTex.path = filePath;
    font.first = newTex;
    font.second = Characters;
    loadedGlyphs.push_back(font);
    return 0;
}


void AssetManager::loadNullTexture(int x, int y, GLuint* texture, GLenum format) {
    printf("Load new null texture \n");
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    
        glTexImage2D(GL_TEXTURE_2D, 0,format, x, y, 0, format, GL_UNSIGNED_BYTE, NULL);
        glGenerateMipmap(GL_TEXTURE_2D);
     
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void AssetManager::loadHeightMap(std::string src, unsigned short*& newHeightMap, int* mapWidth, int* mapHeight) {
    
    auto copyHeightMap = [&] () {
        newHeightMap = new unsigned short[heightMap.width*heightMap.height];
        for (int i = 0; i < heightMap.height*heightMap.width; i++) {
                newHeightMap[i] = heightMap.heightMap[i];
        }
    };
    
    if (heightMap.heightMap) {
        if (src == heightMap.src) {
            copyHeightMap();
            *mapWidth = heightMap.width;
            *mapHeight = heightMap.height;
            return;
        } else {
            delete[] heightMap.heightMap;
        }
    }
 
    int channels = 0; 
    
    heightMap.heightMap = (unsigned short*) stbi_load_16(src.c_str(), mapWidth, mapHeight, &channels, 0);
    if (channels != 1) {
        printf("Warning: Heightmap is not grayscale or does not exist\n");
        return;
    }
    heightMap.src = src;
    heightMap.height = *mapWidth;
    heightMap.width = *mapHeight;
    copyHeightMap();
    printf("Load new heightmap");
}

void AssetManager::generateFramebuffer(Frame* frame, GLenum internalFormat, int x, int y) {
    frame->width = x;
    frame->height = y;
    glGenFramebuffers(1, &frame->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, frame->fbo);
    
    glGenTextures(1, &frame->ftexture);
    glBindTexture(GL_TEXTURE_2D, frame->ftexture);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frame->ftexture, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glGenRenderbuffers(1, &frame->frbo);
    glBindRenderbuffer(GL_RENDERBUFFER, frame->frbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,x, y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, frame->frbo);
    glBindRenderbuffer(GL_RENDERBUFFER,0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, frame->frbo);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    glGenVertexArrays(1, &frame->fvao);
    glBindVertexArray(frame->fvao);
    
    glGenBuffers(1, &frame->fvbo);
    glBindBuffer(GL_ARRAY_BUFFER, frame->fvbo);
    float screenquad[36] =
    {   -1.0f,  1.0f, 0.0f,  0.0f, 1.0f, 0,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0,
         1.0f, -1.0f,0.0f,  1.0f, 0.0f, 0,

        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 0
    };

    glBufferData(GL_ARRAY_BUFFER, 36*sizeof(float), &screenquad[0], GL_STATIC_DRAW);
    
    VertexLoader::setupVAOAttribs(VERTEX_SIMPLEVERTEX);
    glBindVertexArray(0);
    
}



void AssetManager::generateFramebuffer2Color(DoubleFrame* frame, int x, int y) {
    frame->width = x;
    frame->height = y;
    glGenFramebuffers(1, &frame->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, frame->fbo);
    
    glGenTextures(1, &frame->ftexture0);
    glBindTexture(GL_TEXTURE_2D, frame->ftexture0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frame->ftexture0, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glGenTextures(1, &frame->ftexture1);
    glBindTexture(GL_TEXTURE_2D, frame->ftexture1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, frame->ftexture1, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);
    
    glGenRenderbuffers(1, &frame->frbo);
    glBindRenderbuffer(GL_RENDERBUFFER, frame->frbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,x, y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, frame->frbo);
    glBindRenderbuffer(GL_RENDERBUFFER,0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, frame->frbo);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    glGenVertexArrays(1, &frame->fvao);
    glBindVertexArray(frame->fvao);
    
    glGenBuffers(1, &frame->fvbo);
    glBindBuffer(GL_ARRAY_BUFFER, frame->fvbo);
    float screenquad[36] =
    {   -1.0f,  1.0f, 0.0f,  0.0f, 1.0f, 0,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0,
         1.0f, -1.0f,0.0f,  1.0f, 0.0f, 0,

        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 0
    };

    glBufferData(GL_ARRAY_BUFFER, 36*sizeof(float), &screenquad[0], GL_STATIC_DRAW);
    
    VertexLoader::setupVAOAttribs(VERTEX_SIMPLEVERTEX);
    glBindVertexArray(0);

}

void AssetManager::generateFramebuffer(Frame* frame, GLuint* ftexture_, int x, int y) {
    frame->width = x;
    frame->height = y;
    glGenFramebuffers(1, &frame->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, frame->fbo);
    
    frame->ftexture = *ftexture_;
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *ftexture_, 0);
    
    glGenRenderbuffers(1, &frame->frbo);
    glBindRenderbuffer(GL_RENDERBUFFER, frame->frbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, x, y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, frame->frbo);
    glBindRenderbuffer(GL_RENDERBUFFER,0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, frame->frbo);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    glGenVertexArrays(1, &frame->fvao);
    glBindVertexArray(frame->fvao);
    
    glGenBuffers(1, &frame->fvbo);
    glBindBuffer(GL_ARRAY_BUFFER, frame->fvbo);
    float screenquad[36] =
    {   -1.0f,  1.0f, 0.0f,  0.0f, 1.0f, 0,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0,
         1.0f, -1.0f,0.0f,  1.0f, 0.0f, 0,

        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 0
    };

    glBufferData(GL_ARRAY_BUFFER, 36*sizeof(float), &screenquad[0], GL_STATIC_DRAW);
    
    VertexLoader::setupVAOAttribs(VERTEX_SIMPLEVERTEX);
    glBindVertexArray(0);
     
}
void AssetManager::loadCubeMapTexture(Texture* texture, const std::string& posX, const std::string& negX, const std::string& posY, const std::string& negY, const std::string& posZ, const std::string& negZ) {
    if(loadCachedTexture((posX+negX+posY+negY+posZ+negZ).c_str(), texture)) return;
    printf("Load new texture: %s \n", posX.c_str());
    int imageWidth, imageHeight, channels;
    unsigned char* imageData = NULL;
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture->id);

    std::vector<std::string> skyFiles = {posX, negX, posY, negY, posZ, negZ};
    for (unsigned int i = 0; i < skyFiles.size(); i++) {
        imageData = stbi_load(skyFiles.at(i).c_str(), &imageWidth, &imageHeight, &channels, 0);
        if (imageData) {
            GLenum format = 4;
                   if (channels == 1)
                       format = GL_RED;
                   else if (channels == 3)
                       format = GL_RGB;
                   else if (channels == 4)
                       format = GL_RGBA;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_SRGB, imageWidth, imageHeight, 0,format, GL_UNSIGNED_BYTE, imageData  );
            stbi_image_free(imageData);
        } else {
            std::cout << "Failed to load sky box data \n";
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    texture->dimensions.x = imageWidth;
    texture->dimensions.y = imageHeight;
    texture->textureTarget = GL_TEXTURE_CUBE_MAP;
    texture->path = posX+negX+posY+negY+posZ+negZ;
    
    loadedTextures.push_back(*texture);
}
 
