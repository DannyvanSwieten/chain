//
//  input.hpp
//  chain
//
//  Created by Danny van Swieten on 04/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#pragma once

#include <array>
#include <GLFW/glfw3.h>
#include "../world/world.hpp"

static void keyCallback(GLFWwindow*,int,int,int,int);

class InputUpdater
{
public:
    InputUpdater(GLFWwindow* window);
    
    void operator()(World& w, double dt);
    void registerKeyPress(int, int, int , int);
    
private:
    
    std::array<int, 128> status;
    GLFWwindow* window = nullptr;
};
