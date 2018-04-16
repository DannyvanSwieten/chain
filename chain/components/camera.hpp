//
//  camera.hpp
//  chain
//
//  Created by Danny van Swieten on 11/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#pragma once

#include "../math/mat.hpp"
#include "../math/vec.hpp"

struct Camera
{
    vec3 position;
    vec3 target;
    vec3 up = vec3::UP;
    
    float fieldOfView = 65.0;
    float zNear = 0.1;
    float zFar = 100.0;
    
    mat4 viewMatrix = mat4::IDENTITY;
    mat4 perspectiveMatrix;
};
