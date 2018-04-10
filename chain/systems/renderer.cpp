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
	
	std::vector<World::Entity> entities;
	w.getAllEntitiesWithComponents<Transform, StaticMesh>(entities);
	const auto& transforms = w.getAll<Transform>();
	const auto& meshes = w.getAll<StaticMesh>();
	
	for(const auto e: entities)
	{
		const auto& t = transforms[e];
		const auto& m = meshes[e];
		const auto& mat = m->material;
		
		glBindVertexArray(m->vao);
		glUseProgram(mat.program);
		
		mat4 model = mat4::IDENTITY;
		model[3][0] = t->position.x;
		model[3][1] = t->position.y;
		model[3][2] = t->position.z;
		
		setUniforms(mat);
		
		int32_t location = -1;
		location = glGetUniformLocation(mat.program, "modelMatrix");
		glUniformMatrix4fv(location, 1, false, &model[0][0]);
		
		glDrawElements(GL_TRIANGLES, m->numFaces * 3, GL_UNSIGNED_INT, (void*)0);
		glUseProgram(0);
		glBindVertexArray(0);
	}
	
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
