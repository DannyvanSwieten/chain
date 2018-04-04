//
//  swap_chain.cpp
//  chain
//
//  Created by Danny van Swieten on 04/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#include "renderer.hpp"


Renderer::Renderer(GLFWwindow *window): window(window)
{ 
    
}

void Renderer::operator()(World &world, double dt)
{
    glfwMakeContextCurrent(window);
    
    glClearColor(0.25, 0.5, 0.25, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BITS);
    
    glfwSwapBuffers(window);
    glfwMakeContextCurrent(nullptr);
}

