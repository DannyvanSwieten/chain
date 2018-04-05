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

struct StaticMesh
{
	uint32_t vao = 0;
	uint32_t vbo = 0;
	uint32_t ibo = 0;
};
