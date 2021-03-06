//
//  AnimComponent.hpp
//  Pong
//
//  Created by Joseph Gu on 8/9/21.
//

#ifndef AnimComponent_hpp
#define AnimComponent_hpp

#include <stdio.h>
#include <vector>
#include "Component.hpp"
#include "Animation.hpp"
#include "Bone.hpp"
#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include "Watch.hpp"

class Actor;
typedef std::map<std::string, BoneData> BoneInfoMap;

class AnimComponent : public Component {
protected:
    Watch stopwatch;
    
    float currentTickOfAnim;
    float timeInAnim;
    
    Animation* activeAnim = NULL;
    Animation* defaultAnim = NULL;
    std::vector<Animation> animations; 
     
    bool loopCurrent;
    int starttick; 
    int endtick;
    float speedMultiplier;

    glm::mat4 globalInverse;
    std::vector<AssimpNodeData> boneNodes;
    BoneInfoMap BoneDataMap;
    std::vector<glm::mat4> boneMatrices;

    const aiNode* findRootBone(const aiScene* scene, std::map<std::string, BoneData>& map_);

public:
    AnimComponent(Actor& actor, const std::string& filePath);
    void playDefault(); 
    virtual void tick() override;
    void readAssimpTree(const aiNode* node);
    void setDefaultAnim(const std::string& name);
    void playAnim(const std::string& name, bool looped);
    void playAnim(const std::string& name, bool looped, float speed);
    void playAnimIfNotPlaying(const std::string& name);
    void playAnim(const std::string& name, int firsttick, int lasttick);
    void playAnim(const std::string& name, int firsttick, int lasttick, float speed);
    void addAnimation(aiAnimation* animation, const aiScene* scene);
    void setBoneDataMap(const std::map<std::string, BoneData>& BoneDataMap);
    float getTimePlayedOfCurrentAnim();
    std::string getCurrentAnimation();
};
#endif /* AnimComponent_hpp */
