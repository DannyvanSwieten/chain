//
//  mesh.cpp
//  chain
//
//  Created by Danny on 05/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#include "mesh.hpp"

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

void StaticMeshUpdater::operator()(World &, double)
{ 
	
	
	updates.clear();
}

void StaticMeshUpdater::updateMesh(World::Entity e) { 
	updates.emplace_back([&, e](World& w, double dt) {
		
		if(!w.has<MeshFilter>(e))
			w.attach<MeshFilter>(e);
		
		if(!w.has<StaticMesh>(e))
			w.attach<StaticMesh>(e);
		
		updateMeshFromFilter(w, e);
	});
}

void StaticMeshUpdater::createMeshForEntity(World& w, World::Entity e)
{
	w.attach<StaticMesh>(e);
	auto& m = w.getAll<StaticMesh>()[e];
	
	glGenVertexArrays(1, &m->vao);
	glGenBuffers(1, &m->vbo);
	
	glBindVertexArray(m->vao);
	glBindBuffer(GL_VERTEX_ARRAY, m->vbo);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, sizeof(vec3), GL_FLOAT, false,
						  sizeof(Vertex), (void*)offsetof(Vertex, position));
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, sizeof(vec3), GL_FLOAT, false,
						  0, (void*)offsetof(Vertex, normal));
	
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, sizeof(vec2), GL_FLOAT, false,
						  0, (void*)offsetof(Vertex, uv));
	
	glBindVertexArray(0);
}

void StaticMeshUpdater::updateMeshFromFilter(World& w, World::Entity e)
{
	auto& filter = w.getAll<MeshFilter>()[e];
	auto& mesh = w.getAll<StaticMesh>()[e];
	
	glBindBuffer(GL_VERTEX_ARRAY, mesh->vbo);
	glBufferData(GL_VERTEX_ARRAY, filter->positions.size() * sizeof(Vertex), nullptr, GL_STATIC_DRAW);
	
	Vertex* vertices = static_cast<Vertex*>(glMapBuffer(GL_VERTEX_ARRAY, GL_WRITE_ONLY));
	
	auto& pos = filter->positions;
	auto& normal = filter->normals;
	auto& uv = filter->uv;
	
	for(auto i = 0; i < filter->positions.size(); i++)
	{
		vertices[i].position = pos[i];
		vertices[i].normal = normal[i];
		vertices[i].uv = uv[i];
	}
	
	glUnmapBuffer(GL_VERTEX_ARRAY);
	glBindBuffer(GL_VERTEX_ARRAY, 0);
}
