//
//  engine.cpp
//  chain
//
//  Created by Danny van Swieten on 20/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#include "engine.hpp"

#include "components/mesh_filter.hpp"

Engine::Engine(const std::string& name): world(1024)
{
    const auto result = glfwInit();
    if(result != GLFW_TRUE)
        exit(1);
    
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_SAMPLES, 8);
    
    GLFWwindow* window = nullptr;
    window = glfwCreateWindow(1080, 720, "Ja toch", nullptr, nullptr);
    assert(window);
    
    glfwShowWindow(window);
    
    inputUpdater = std::make_unique<InputUpdater>(window);
    renderer = std::make_unique<Renderer>(window, name);
    
    world.addUpdater(renderer.get());
    world.addUpdater(inputUpdater.get());
    world.addUpdater(&scriptUpdater);
    world.addUpdater(&cameraUpdater);
    world.addUpdater(&collision);
    world.addUpdater(&physics);
    world.addUpdater(&meshUpdater);
    
    setupDefaultScripting();
}

Engine::~Engine()
{
    glfwTerminate();
    glfwDestroyWindow(window);
}

World &Engine::getWorld()
{
    return world;
}

chaiscript::ChaiScript &Engine::getScriptContext()
{
    return scriptUpdater.getContext();
}

void Engine::run()
{
    world.reflect(scriptUpdater.getContext());
    
    running = true;
    while(running)
    {
        update(1.0 / 60);
    }
}

void Engine::update(double dt)
{
    world.update(dt);
}

void Engine::setupDefaultScripting()
{
    setupMathScripting();
}

void Engine::setupMathScripting()
{
    auto& context = getScriptContext();
    
    context.add(chaiscript::fun([](vec2& lhs, const vec2& rhs){lhs = rhs;}), "=");
    context.add(chaiscript::constructor<vec2(float, float)>(), "Vec2");
    
    context.add(chaiscript::fun([](vec3& lhs, const vec3& rhs){lhs = rhs;}), "=");
    context.add(chaiscript::constructor<vec3(float, float, float)>(), "Vec3");
    
    context.add(chaiscript::fun([](vec4& lhs, const vec4& rhs){lhs = rhs;}), "=");
    context.add(chaiscript::constructor<vec4(float, float, float, float)>(), "Vec4");
    
    context.add(chaiscript::fun([](vec3i& lhs, const vec3i& rhs){lhs = rhs;}), "=");
    context.add(chaiscript::constructor<vec3i(uint32_t, uint32_t, uint32_t)>(), "Face");
    
    context.add(chaiscript::fun(&vec2::x), "x");
    context.add(chaiscript::fun(&vec2::y), "y");
    
    context.add(chaiscript::fun(&vec3::x), "x");
    context.add(chaiscript::fun(&vec3::y), "y");
    context.add(chaiscript::fun(&vec3::z), "z");
    
    context.add(chaiscript::fun(&vec4::x), "x");
    context.add(chaiscript::fun(&vec4::y), "y");
    context.add(chaiscript::fun(&vec4::z), "z");
    context.add(chaiscript::fun(&vec4::w), "w");
    
    context.add(chaiscript::fun(&vec3i::x), "a");
    context.add(chaiscript::fun(&vec3i::y), "b");
    context.add(chaiscript::fun(&vec3i::z), "c");
}
