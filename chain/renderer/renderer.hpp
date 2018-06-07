//
//  renderer.hpp
//  chain
//
//  Created by Danny van Swieten on 07/06/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#pragma once

#include "../components/mesh_filter.hpp"

struct Vertex
{
    vec3 position;
    vec3 normal;
    vec2 uv;
};

class Renderer
{
public:
    virtual ~Renderer() { }
    
    virtual std::size_t createStaticMesh() = 0;
    virtual std::size_t updateStaticMesh(const MeshFilter& filter) = 0;
    
};
