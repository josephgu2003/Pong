//
//  AIComponent.hpp
//  Pong
//
//  Created by Joseph Gu on 9/29/21.
//

#ifndef AIComponent_hpp
#define AIComponent_hpp

#include <stdio.h>
#include "Component.hpp"
#include "Dice.hpp" 
#include "AIAction.hpp"
#include <memory>
#include "CharacterComponent.hpp"

#define MAX_TRAITS 3
#define MAX_RS 2
#define NUM_ACTIONS 5
#define NUM_PASSIVE_ACTS 3
#define NUM_FRIEND_ACTS 2

struct aiActionAffinity {
    aiActionEnum action;
    float baseProb;
    PersonalityTrait affinities[MAX_TRAITS];
    
    aiActionAffinity () {
         
    }
    aiActionAffinity(aiActionEnum action_,
                     float baseProb_,
                     const PersonalityTrait affinities_[MAX_TRAITS]) {
        action = action_;
        baseProb = baseProb_;
        for (int i = 0; i < MAX_TRAITS; i++) {
            affinities[i] = affinities_[i];
        }
    }
};

const static RelationShipType emptyRSFactors[MAX_RS] = {RS_EMPTY, RS_EMPTY};

const static PersonalityTrait aiPonderTraits[MAX_TRAITS] = {PT_REFLECTIVE, PT_EMPTY, PT_EMPTY};

const static PersonalityTrait aiWatchSceneryTraits[MAX_TRAITS] = {PT_REFLECTIVE, PT_EMPTY, PT_EMPTY};

const static PersonalityTrait aiSingFriendTraits[MAX_TRAITS] = {PT_AMIABLE, PT_ROMANTIC, PT_EMPTY};
const static RelationShipType aiSingFriendRSFactors[MAX_RS] = {RS_FRIEND, RS_EMPTY};

const static PersonalityTrait aiDanceTraits[MAX_TRAITS] = {PT_ENERGETIC, PT_ROMANTIC, PT_EMPTY};
 
const static PersonalityTrait aiGiveGiftTraits[MAX_TRAITS] = {PT_AMIABLE, PT_ROMANTIC, PT_LOYAL};
const static RelationShipType aiGiveGiftRSFactors[MAX_RS] = {RS_FRIEND, RS_EMPTY};

const static aiActionAffinity aiPassiveActions[NUM_PASSIVE_ACTS] = {
    {ACT_PONDER, 0.2f, aiPonderTraits},
    {ACT_WATCH_SCENERY, 0.2f, aiWatchSceneryTraits},
    {ACT_DANCE, 0.15f, aiDanceTraits}
};

const static aiActionAffinity aiFriendActions[NUM_FRIEND_ACTS] = {
    {ACT_SING_FRIEND, 0.25, aiSingFriendTraits},
    {ACT_GIVE_GIFT, 0.05f, aiGiveGiftTraits}
};

// some require no people around, others need person around
// give 4th array of rs compatibles, if both empty then no rs required, else required
  
// problems: lonely actions can occur with people around
// friend actions can occur to enemies
// friend actions can happen alone, just the nullptr will catch that and force a recalc


class Actor;

class AIComponent : public Component {
private:
    aiActionAffinity* actionTable = NULL;
    Actor* target = NULL;
    Dice dice;
    void calcNextAction(Actor* actor, Actor* target);
    void calcProbabilities(Actor* actor, Actor* target);
   // void newAction(aiActionEnum action, Actor* target);
    void resetTable(RelationShipType rst);
    int entriesInTable;
public:
    AIComponent(Actor& actor);
    virtual void tick() override;
};

// variables affecting choice of action:
// personality
// environment
// people around
// dice roll
 
// variables affecting ending an action:
// danger
// new situation
// action is done
// action can't keep going much longer

// if action ticks with time, hp and drainrate

// passive mode:
// pace and observe nature
// lie down

// alone: need separation between alone and passive so passive actions can be done
// over and over without being wierd, alone is more active
// dance
// sing

// with friend:
// hug
// sing for them
// share a snack

// with enemy:
// threaten
// stand-off (possible escalation into fight)
// ??

// also the leave action

// an action goes on until its done


#endif /* AIComponent_hpp */
