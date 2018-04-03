//
//  rigid_body.hpp
//  chain
//
//  Created by Danny on 03/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#include "../math/vec.hpp"

#pragma once

struct RigidBody
{
	vec3 momentum;
	vec3 velocity;
	math_precision_t invMass = 1.0;
};
