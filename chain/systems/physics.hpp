//
//  physics.hpp
//  chain
//
//  Created by Danny on 03/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#pragma once

#include "../world/world.hpp"
#include "system.hpp"

class PhysicsUpdater: public System
{
public:
    PhysicsUpdater(World& w);
	void operator()(World& w, double dt) final;
    void reflect(chaiscript::ChaiScript&) final;
	
	void applyForce(World::Entity e, const vec3& F);
	void setMass(World::Entity e, math_precision_t mass);
	
private:
	
};
