//
//  mesh_filter.h
//  chain
//
//  Created by Danny on 05/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#pragma once

#include <vector>

#include "../math/vec.hpp"

struct MeshFilter
{
	std::vector<vec3> positions;
	std::vector<vec3> normals;
	std::vector<vec2> uv;
	
	std::vector<vec3i> faces;
};
