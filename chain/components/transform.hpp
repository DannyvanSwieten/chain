//
//  transform.h
//  chain
//
//  Created by Danny van Swieten on 30/03/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#pragma once

#include "vec.hpp"
#include "mat.hpp"

struct Transform
{
	vec3 position;
    vec3 rotation;
    vec3 scale;
};
