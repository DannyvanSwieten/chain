//
//  opengl_renderer.hpp
//  chain
//
//  Created by Danny van Swieten on 07/06/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#pragma once

#include "renderer.hpp"

class OpenGLRenderer: public Renderer
{
public:
    
    struct StaticMesh
    {
        uint32_t vao = 0;
        uint32_t vbo = 0;
        uint32_t ibo = 0;
        
        uint32_t numFaces = 0;
        uint32_t numVertices = 0;
        uint32_t primitiveType;
    };
    
    std::size_t createStaticMesh() override;
    
private:
    
    std::vector<StaticMesh> staticMeshes;
};
