//
//  swap_chain.cpp
//  chain
//
//  Created by Danny van Swieten on 04/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#include "renderer.hpp"
#include <OpenGL/gl3.h>

Renderer::Renderer(GLFWwindow *window): window(window)
{ 
    
}

void Renderer::operator()(World &w, double dt)
{
    glfwMakeContextCurrent(window);
    
    for(auto& update: stateUpdates)
        update(w, dt);
    
    glClearColor(0.25, 0.5, 0.25, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glfwSwapBuffers(window);
    glfwMakeContextCurrent(nullptr);
}
