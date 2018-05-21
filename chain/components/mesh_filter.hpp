//
//  mesh_filter.h
//  chain
//
//  Created by Danny on 05/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#pragma once

#include "../vector_wrapper.hpp"

#include "../math/vec.hpp"

struct MeshFilter
{
	vector_wrapper<vec3> positions;
	vector_wrapper<vec3> normals;
	vector_wrapper<vec2> uv;
	
	vector_wrapper<vec3i> faces;
    
    uint32_t primitiveType;
};
