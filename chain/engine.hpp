//
//  engine.hpp
//  chain
//
//  Created by Danny van Swieten on 20/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#pragma once

#include <GLFW/glfw3.h>

#include "world/world.hpp"
#include "systems/camera_system.hpp"
#include "systems/collision.hpp"
#include "systems/physics.hpp"
#include "systems/lua_script_system.hpp"
#include "systems/input.hpp"
#include "systems/mesh.hpp"
#include "systems/renderer.hpp"
#include "systems/script.hpp"

class Engine
{
public:
    
    Engine(const std::string& name);
    ~Engine();
    
    void run();
    
    World& getWorld();
    chaiscript::ChaiScript& getScriptContext();
    ScriptUpdater& getScriptSystem()
    {
        return scriptUpdater;
    };
    
    LuaScriptSystem& getLuaContext()
    {
        return luaScriptSystem;
    }
    
private:
    
    void update(double dt);
    void setupDefaultScripting();
    void setupMathScripting();
    
public:
    
    World world;
    GLFWwindow* window = nullptr;
    std::unique_ptr<InputUpdater> inputUpdater;
    std::unique_ptr<Renderer> renderer;
    ScriptUpdater scriptUpdater;
    LuaScriptSystem luaScriptSystem;
    CameraUpdater cameraUpdater;
    PhysicsUpdater physics;
    CollisionUpdater collision;
    StaticMeshUpdater meshUpdater;
    
    bool running = false;
};
