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
    
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    renderStaticMeshes(w, dt);
	
    glfwSwapBuffers(window);
	
	stateUpdates.clear();
}

void Renderer::setUniforms(const Material& material)
{
	for(const auto& uniform: material.properties)
	{
		UniformVisitor v(material.program, uniform.first);
		boost::apply_visitor(v, uniform.second);
	}
}

void Renderer::renderStaticMeshes(World &w, double dt)
{
    std::vector<World::Entity> entities;
    w.getAllEntitiesWithComponents<Transform, StaticMesh>(entities);
    const auto& transforms = w.getAll<Transform>();
    const auto& meshes = w.getAll<StaticMesh>();
    
    for(const auto e: entities)
    {
        const auto& t = transforms[e];
        const auto& m = meshes[e];
        const auto& mat = m->material;
        
        if(!m->vao)
            continue;
        
        glBindVertexArray(m->vao);
        glUseProgram(mat.program);
        
        mat4 model = mat4::IDENTITY;
        model[3][0] = t->position.x;
        model[3][1] = t->position.y;
        model[3][2] = t->position.z;
        
        model[0][0] = t->scale.x;
        model[1][1] = t->scale.y;
        model[2][2] = t->scale.z;
        
        setUniforms(mat);
        
        int32_t location = -1;
        location = glGetUniformLocation(mat.program, "modelMatrix");
        glUniformMatrix4fv(location, 1, false, &model[0][0]);
        
        if(w.mainCamera)
        {
            location = glGetUniformLocation(mat.program, "viewMatrix");
            glUniformMatrix4fv(location, 1, false, &w.mainCamera->viewMatrix[0][0]);
            
            location = glGetUniformLocation(mat.program, "projectionMatrix");
            glUniformMatrix4fv(location, 1, false, &w.mainCamera->perspectiveMatrix[0][0]);
        }
        
        int ratio = 3;
        if(m->primitiveType == GL_LINES)
            ratio = 2;
        else if(m->primitiveType == GL_POINTS)
            ratio = 1;
        
        if(m->numFaces)
            glDrawElements(m->primitiveType, m->numFaces * ratio, GL_UNSIGNED_INT, (void*)0);
        else
            glDrawArrays(m->primitiveType, 0, m->numVertices);
        
        glUseProgram(0);
        glBindVertexArray(0);
    }
}
