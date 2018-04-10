//
//  static_mesh.h
//  chain
//
//  Created by Danny van Swieten on 04/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#pragma once

#include <vector>

#include "../math/vec.hpp"

struct Material
{
	uint32_t vertexShader 	= 0;
	uint32_t fragmentShader = 0;
	uint32_t program 		= 0;
	
	vec3 albedo{0.3, 0.7, 0.5};
};

struct StaticMesh
{
	uint32_t vao = 0;
	uint32_t vbo = 0;
	uint32_t ibo = 0;
	
	uint32_t numFaces = 0;
	
	Material material;
};
