//
//  swap_chain.hpp
//  chain
//
//  Created by Danny van Swieten on 04/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#pragma once

#include <GLFW/glfw3.h>

#include "../world/world.hpp"

class Renderer
{
public:
    Renderer(GLFWwindow* window);
    
    void operator()(World& world, double dt);
    
private:
    
    GLFWwindow* window = nullptr;
};
