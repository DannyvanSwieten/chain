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

#include <iostream>

const std::string VERTEX_SHADER = R"(
#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4 modelMatrix = mat4(1.0);
uniform mat4 viewMatrix = mat4(1.0);

void main()
{
	gl_Position = viewMatrix * modelMatrix * vec4(position, 1.0);
}

)";

const std::string FRAGMENT_SHADER = R"(
#version 330

layout(location = 0) out vec4 fragmentColour;

uniform vec3 albedo;

void main()
{
	fragmentColour = vec4(albedo, 1.0);
}

)";

void StaticMeshUpdater::operator()(World& w, double dt)
{ 
	for(auto& update: updates)
		update(w, dt);
	
	updates.clear();
}

void StaticMeshUpdater::updateMesh(World::Entity e, const MeshFilter& filter) {
	updates.emplace_back([&, e](World& w, double dt) {
		
		if(!w.has<StaticMesh>(e))
		{
			w.attach<StaticMesh>(e);
			createMeshForEntity(w, e);
		}
		
		updateMeshFromFilter(filter, w, e);
	});
}

void StaticMeshUpdater::createMeshForEntity(World& w, World::Entity e)
{
	w.attach<StaticMesh>(e);
	auto& m = w.getAll<StaticMesh>()[e];

	glGenVertexArrays(1, &m->vao);
	glGenBuffers(1, &m->vbo);
	glGenBuffers(1, &m->ibo);
	
	glBindVertexArray(m->vao);
	glBindBuffer(GL_ARRAY_BUFFER, m->vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ibo);
	
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, false,
						  sizeof(Vertex), (void*)offsetof(Vertex, position));
	
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(1, 3, GL_FLOAT, false,
						  0, (void*)offsetof(Vertex, normal));
	
	glEnableVertexAttribArray(2);
	
	glVertexAttribPointer(2, 2, GL_FLOAT, false,
						  0, (void*)offsetof(Vertex, uv));
	
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	m->material.vertexShader = glCreateShader(GL_VERTEX_SHADER);
	m->material.fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	m->material.program = glCreateProgram();
	
	const char* s = (const GLchar*)VERTEX_SHADER.c_str();
	glShaderSource(m->material.vertexShader, 1, &s, 0);
	glCompileShader(m->material.vertexShader);
	
	GLint success = 0;
	glGetShaderiv(m->material.vertexShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		GLint logSize = 0;
		glGetShaderiv(m->material.vertexShader, GL_INFO_LOG_LENGTH, &logSize);
		std::vector<char> log(logSize);
		glGetShaderInfoLog(m->material.program, logSize, &logSize, log.data());
		for(const auto c: log)
			std::cout << c;
		
		std::cout << '\n';
	}
	
	const char* fs = FRAGMENT_SHADER.c_str();
	glShaderSource(m->material.fragmentShader, 1, &fs, 0);
	glCompileShader(m->material.fragmentShader);
	
	success = 0;
	glGetShaderiv(m->material.fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		GLint logSize = 0;
		glGetShaderiv(m->material.fragmentShader, GL_INFO_LOG_LENGTH, &logSize);
		std::vector<char> log(logSize);
		glGetShaderInfoLog(m->material.fragmentShader, logSize, &logSize, log.data());
		for(const auto c: log)
			std::cout << c;
		
		std::cout << '\n';
	}
	
	glAttachShader(m->material.program, m->material.vertexShader);
	glAttachShader(m->material.program, m->material.fragmentShader);
	
	glLinkProgram(m->material.program);
	
	success = 0;
	glGetProgramiv(m->material.program, GL_LINK_STATUS, &success);
	if(!success)
	{
		GLint logSize = 0;
		glGetProgramiv(m->material.program, GL_INFO_LOG_LENGTH, &logSize);
		std::vector<char> log(logSize);
		glGetProgramInfoLog(m->material.program, logSize, &logSize, log.data());
		for(const auto c: log)
			std::cout << c;
		
		std::cout << '\n';
	}
    
    auto error = glGetError();
    assert(error == GL_NO_ERROR);
}

void StaticMeshUpdater::updateMeshFromFilter(const MeshFilter& filter, World& w, World::Entity e)
{
	auto& mesh = w.getAll<StaticMesh>()[e];

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
	glBufferData(GL_ARRAY_BUFFER, filter.positions.size() * sizeof(Vertex), nullptr, GL_STATIC_DRAW);
	
	Vertex* vertices = static_cast<Vertex*>(glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE));
    if(!vertices)
        return;
    
	assert(vertices);
	
	auto& pos = filter.positions;
	auto& normal = filter.normals;
	auto& uv = filter.uv;
	
	for(auto i = 0; i < filter.positions.size(); i++)
		vertices[i].position = pos[i];

	for(auto i = 0; i < normal.size(); i++)
		vertices[i].normal = normal[i];
	
	for(auto i = 0; i < uv.size(); i++)
		vertices[i].uv = uv[i];
	
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, filter.faces.size() * sizeof(vec3i), filter.faces.data(), GL_STATIC_DRAW);
	mesh->numFaces = filter.faces.size();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

