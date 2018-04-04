//
//  input.cpp
//  chain
//
//  Created by Danny van Swieten on 04/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#include "input.hpp"

void keyCallback(GLFWwindow* window, int x, int y, int z, int w)
{
    auto self = static_cast<InputUpdater*>(glfwGetWindowUserPointer(window));
    self->registerKeyPress(x, y, z, w);
}

InputUpdater::InputUpdater(GLFWwindow *window): window(window)
{
    assert(window);
    
    glfwSetKeyCallback(window, keyCallback);
    glfwSetWindowUserPointer(window, this);
}

void InputUpdater::operator()(World &w, double dt)
{
    glfwPollEvents();
}

void InputUpdater::registerKeyPress(int x, int y, int z, int w)
{
    status[x] = y;
}

