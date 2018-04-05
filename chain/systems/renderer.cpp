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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BITS);
    
    glfwSwapBuffers(window);
    glfwMakeContextCurrent(nullptr);
}

void Renderer::setMesh(const ::StaticMesh &m, World::Entity e) {
    
    stateUpdates.emplace_back([&, m, e](World& w, double dt) {
    
        if(staticMeshes.find(e) == staticMeshes.end())
            createMeshForEntity(e);
        
        auto& mesh = staticMeshes[e];
        glBindBuffer(GL_VERTEX_ARRAY, mesh.vbo);
        glBufferData(GL_VERTEX_ARRAY, m.data.size() * sizeof(::StaticMesh::Vertex), m.data.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_VERTEX_ARRAY, 0);
    });
}

void Renderer::createMeshForEntity(World::Entity e) {
    Renderer::StaticMesh m;
    glGenVertexArrays(1, &m.vao);
    glGenBuffers(1, &m.vbo);
    
    glBindVertexArray(m.vao);
    glBindBuffer(GL_VERTEX_ARRAY, m.vbo);

    glVertexAttribPointer(0, sizeof(vec3), GL_FLOAT, false,
                          sizeof(::StaticMesh::Vertex), (void*)0);
    
    glVertexAttribPointer(1, sizeof(vec3), GL_FLOAT, false,
                          sizeof(::StaticMesh::Vertex), (void*)offsetof(::StaticMesh::Vertex, normal));
    
    glVertexAttribPointer(2, sizeof(vec2), GL_FLOAT, false,
                          sizeof(::StaticMesh::Vertex), (void*)offsetof(::StaticMesh::Vertex, uv));
    
    glBindVertexArray(0);
    
    staticMeshes.emplace(e, m);
}



