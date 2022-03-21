//
//  PropFactory.hpp
//  Pong
//
//  Created by Joseph Gu on 11/11/21.
//

#ifndef PropFactory_hpp
#define PropFactory_hpp

#include <stdio.h>  
#include <memory>
#include "Prop.hpp"

enum PropEnum { 
    PROP_TREE,   
    PROP_GRASS,
    PROP_WELL,
    PROP_SWORD_SLASH, 
    PROP_WATER,
    PROP_LIGHTRAY,
    PROP_SWORD_EXP,
    PROP_MOUNT,
    PROP_AURORA,
    PROP_IMPRISONMENT,
    PROP_PINE,
    PROP_MT,
    PROP_SNOW_FLOWER_BLUE
};

class PropFactory {
public:
    std::shared_ptr<Prop> makeProp(int pe);
};
#endif /* PropFactory_hpp */
