//
//  lua_script.hpp
//  chain
//
//  Created by Danny van Swieten on 02/05/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#pragma once

#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>

struct LuaScript
{
    lua_State* L = nullptr;
    std::function<void(size_t, double dt)> onUpdate;
    std::function<void(size_t)> onStart;
    std::function<void(size_t)> onStop;
};
