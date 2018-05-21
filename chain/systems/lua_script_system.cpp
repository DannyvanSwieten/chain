//
//  lua_script_system.cpp
//  chain
//
//  Created by Danny van Swieten on 02/05/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#include "lua_script_system.hpp"

LuaScriptSystem::LuaScriptSystem()
{
    main = luaL_newstate();
    if(main)
        luaL_openlibs(main);
}

void LuaScriptSystem::start(World& w)
{
    auto& scripts = w.getAll<LuaScript>();
    std::vector<World::Entity> entities;
    w.getAllEntitiesWithComponents<LuaScript>(entities);
    for(auto& e: entities)
    {
        if(scripts[e]->onStart)
            scripts[e]->onStart(e);
    }
}

void LuaScriptSystem::operator()(World &w, double dt)
{
    auto& scripts = w.getAll<LuaScript>();
    std::vector<World::Entity> entities;
    w.getAllEntitiesWithComponents<LuaScript>(entities);
    for(const auto e: entities)
    {
        auto& script = *scripts[e];
        
        if(script.onUpdate)
            script.onUpdate(e, dt);
    }
}

void LuaScriptSystem::loadScript(World::Entity e, const std::string &pathToFile)
{
    schedulePreStartTask([&, e, pathToFile](World& w) {
        if(!w.has<LuaScript>(e))
            w.attach<LuaScript>(e);
        
        auto& script = w.getAll<LuaScript>()[e];
        script->L = lua_newthread(main);
        const int result = luaL_dofile(script->L, pathToFile.c_str());
        if(result == LUA_OK)
        {
            script->onUpdate = luabridge::getGlobal(script->L, "onUpdate");
            script->onStart = luabridge::getGlobal(script->L, "onStart");
        }
        else
        {
            printf("%s\n", lua_tostring(script->L, -1));
        }
    });
}
