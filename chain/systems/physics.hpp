//
//  physics.hpp
//  chain
//
//  Created by Danny on 03/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#pragma once

#include "../world/world.hpp"

class PhysicsUpdater
{
public:
	void operator()(World& w, double dt);
	
	void applyForce(const vec3& F, World::Entity e);
	void setMass(math_precision_t mass, World::Entity e);
	
private:
	
	void preUpdate(World& w, double dt);
	
private:
	
	std::vector<std::function<void(World&, double)>> stateUpdates;
	
};
