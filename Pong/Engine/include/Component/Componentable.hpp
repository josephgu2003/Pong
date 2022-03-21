//
//  Componentable.hpp
//  Pong
//
//  Created by Joseph Gu on 7/31/21.
//

#ifndef Componentable_hpp
#define Componentable_hpp

#include <stdio.h>
#include <vector> 
#include "Component.hpp"
#include <memory>

class Componentable {
protected:
    std::vector<std::shared_ptr<Component>> components;
public:
    Componentable() {
        
    }
    
    void tick() {
        for (int i = 0; i < components.size(); i++) {
            components.at(i)->tick();
        }
    }
    
    template <typename compType, typename... Args>
    void addComponent(Args&& ... arg) {
        addComp(std::make_shared<compType>(arg...));
   } 
    virtual void addComp(const std::shared_ptr<Component>& comp_) {
        if (components.size() == 0) { 
            components.push_back((comp_));
            return;
        } 
        for (int i = 0; i < components.size(); i++) {
            if (i == (components.size()-1)) {
                components.push_back((comp_));
                return;
            }
            else if (components.at(i)->getUpdatePriority() < comp_->getUpdatePriority()) {
                continue;
            } else {
                components.insert(components.begin()+i,(comp_));
                return;
            }
        }
    }


    template <typename compType>
    bool getComponentRef(std::weak_ptr<compType>& ref)
    {
        for (int i = 0 ; i < components.size(); i++) {
            if (auto x = dynamic_pointer_cast<compType>(components.at(i))) {
                ref = x;
                return true;
            }
        }
            return false; // make a null component or something
 
    }
  
    template <typename compType>
    compType* getComponent() 
    {
        for (int i = 0 ; i < components.size(); i++) { 
            if (dynamic_cast<compType*>(components.at(i).get()) != NULL) {
                return static_cast<compType*>(components.at(i).get());
            }
        }
            return nullptr; // make a null component or something
 
    }
    
    void deleteComponent(Component* c) {
        for (auto i = components.begin(); i != components.end(); i++) {
            if ((*i).get() == c) {
                components.erase(i);
                return;
            }
        }
    }
    
    
    template <typename compType>
    bool hasComponent()
    {
        for (int i = 0 ; i < components.size(); i++) {
            auto x = dynamic_cast<compType*>(components.at(i).get()) ;
            if (x != NULL) {
                return true;
            }
        }
        return false;
    }
    
};

//option 1 : template, now compentable can reject wrong types
// option 2 : enum, compenentable can reject wrong types by casting
// option 3 : child classes

#endif /* Componentable_hpp */
