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
    vec3 albedo;
    float roughness;
};

struct StaticMesh
{
    struct Vertex
    {
        vec3 position;
        vec3 normal;
        vec2 uv;
    };
    
    std::vector<Vertex> data;
    Material material;
};
