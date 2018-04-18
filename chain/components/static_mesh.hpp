//
//  static_mesh.h
//  chain
//
//  Created by Danny van Swieten on 04/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#pragma once

#include <boost/variant.hpp>
#include <map>
#include <vector>

#include "../math/vec.hpp"

using uniform_t = boost::variant<float, vec2, vec3, vec4>;

struct Material
{
	uint32_t vertexShader 	= 0;
	uint32_t fragmentShader = 0;
	uint32_t program 		= 0;
	
	std::map<std::string, uniform_t> properties;
};

struct StaticMesh
{
	uint32_t vao = 0;
	uint32_t vbo = 0;
	uint32_t ibo = 0;
	
	uint32_t numFaces = 0;
    uint32_t numVertices = 0;
    uint32_t primitiveType;
	
	Material material;
    
    
};
