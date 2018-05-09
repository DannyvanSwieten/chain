//
//  lua_script_system.hpp
//  chain
//
//  Created by Danny van Swieten on 02/05/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#pragma once

#include "lua_script.hpp"
#include "../world/world.hpp"
#include "system.hpp"

class LuaScriptSystem: public System
{
public:
    
    LuaScriptSystem();
    
    void start(World&) override;
    void operator()(World &, double) override;
    void loadScript( World::Entity e, const std::string& pathToFile );
    
    lua_State* getContext()
    {
        return main;
    }
    
private:
    
    lua_State* main = nullptr;
};
