//
//  swap_chain.hpp
//  chain
//
//  Created by Danny van Swieten on 04/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#pragma once

#include <GLFW/glfw3.h>
#include <map>
#include <vector>

#include "../world/world.hpp"

class Renderer
{
public:
    
    struct StaticMesh
    {
        GLuint vao;
        GLuint vbo;
    };
    
    Renderer(GLFWwindow* window);
    void operator()(World& world, double dt);
    
private:
    
    void createMeshForEntity(World::Entity);
    
private:
    
    std::vector<std::function<void(World&, double)>> stateUpdates;
    
    GLFWwindow* window = nullptr;
};
