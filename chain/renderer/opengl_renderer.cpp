//
//  opengl_renderer.cpp
//  chain
//
//  Created by Danny van Swieten on 07/06/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#include "opengl_renderer.hpp"

#include <OpenGL/gl3.h>

std::size_t OpenGLRenderer::createStaticMesh()
{
    auto mesh = staticMeshes.emplace_back();
    
    uint32_t error = 0;
    
    glGenVertexArrays(1, &mesh.vao);
    assert(mesh.vao != 0);
    assert( (error = glGetError()) == GL_NO_ERROR);
    glGenBuffers(1, &mesh.vbo);
    assert(mesh.vbo != 0);
    assert( (error = glGetError()) == GL_NO_ERROR);
    glGenBuffers(1, &mesh.ibo);
    assert(mesh.ibo != 0);
    assert( (error = glGetError()) == GL_NO_ERROR);
    
    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
    
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, false,
                          sizeof(Vertex), (void*)offsetof(Vertex, position));
    
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, false,
                          sizeof(Vertex), (void*)offsetof(Vertex, normal));
    
    glEnableVertexAttribArray(2);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, false,
                          sizeof(Vertex), (void*)offsetof(Vertex, uv));
    
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    assert( (error = glGetError()) == GL_NO_ERROR);
    return staticMeshes.size() - 1;
}
