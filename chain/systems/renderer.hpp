//
//  swap_chain.hpp
//  chain
//
//  Created by Danny van Swieten on 04/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#pragma once

#include <GLFW/glfw3.h>
#include <map>
#include <vector>

#include "../world/world.hpp"
#include "system.hpp"

struct UniformVisitor: public boost::static_visitor<>
{
	uint32_t program;
	const std::string& name;
	
	UniformVisitor(uint32_t program, const std::string& name): program(program), name(name){ }
	
	void operator()(float x)
	{
		const auto location = glGetUniformLocation(program, name.c_str());
		glUniform1f(location, x);
	}
	
	void operator()(const vec2& value)
	{
		const auto location = glGetUniformLocation(program, name.c_str());
		glUniform2f(location, value.x, value.y);
	}
	
	void operator()(const vec3& value)
	{
		const auto location = glGetUniformLocation(program, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}
	
	void operator()(const vec4& value)
	{
		const auto location = glGetUniformLocation(program, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}
};

class Renderer: public System
{
public:
    
    Renderer(GLFWwindow* window, const std::string& name);
    void operator()(World& world, double dt) final;
	
	void setUniforms(const Material& material);
    
private:
    
    void renderStaticMeshes(World& world, double dt);
    
private:
    
    GLFWwindow* window = nullptr;
    const std::string name;
};
