//
//  JsonManager.hpp
//  Pong
//
//  Created by Joseph Gu on 9/7/21.
//

#ifndef JsonManager_hpp
#define JsonManager_hpp

#define SAVE_PATH "Resources/GameData/SaveFile.json"
 
#include <stdio.h>
#include <string>
#include <memory>
#include <vector>
#include "json.hpp"
#include "Relationship.hpp"

class AbstractActorFactory;

class GameLevel;
  
class PropFactory;
class ParticleFactory;
class ScriptFactory;
struct DialogueTree;
 
class Relationship;

class Dialogue;

class JsonManager {
static nlohmann::json dialogues;
    static void buildTree(DialogueTree*& tree, int i, int branchID);
public:
    static void loadDialogue(Dialogue* dialogue, int id);
    static void loadGameLevel(GameLevel* game, AbstractActorFactory* af, PropFactory* propf, ParticleFactory* pf, ScriptFactory* sf);
    static void saveGameLevel(GameLevel* game);
    static bool checkRelationshipLoaded(std::string aname, std::string bname, RelationShipType type, const std::vector<std::shared_ptr<Relationship>>&  loaded);
};



#endif /* JsonManager_hpp */
