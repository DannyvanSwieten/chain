//
//  spring_joint.h
//  chain
//
//  Created by Danny on 03/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#pragma once

#include "../math/vec.hpp"

struct SpringJoint
{
	vec3 anchor{0, 10, 0};
	vec3 attachedAnchor;
	
	math_precision_t k = 0.01;
	math_precision_t b = 1;
};
