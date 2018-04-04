//
//  main.cpp
//  chain
//
//  Created by Danny on 27/03/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#include <iostream>
#include <queue>

//#include <chaiscript/chaiscript.hpp>

#include "world.hpp"
#include "systems/physics.hpp"
#include "systems/collision.hpp"

int main(int argc, const char * argv[]) {

	PhysicsUpdater physics;
	CollisionUpdater collision;
	
	World w(10);
	const auto object = w.createEntity();
    w.attach<RigidBody>(object);
    w.attach<BoundingBox>(object);
    
    physics.setMass(100.0, object);
    collision.setBoundingVolume({50, 50, 50}, object);
    
    const auto object2 = w.createEntity();
	w.attach<RigidBody>(object2);
	w.attach<BoundingBox>(object2);
	w.attach<SpringJoint>(object2);
    w.setPosition({0, 100, 0}, object2);
	
	physics.setMass(100.0, object2);
	collision.setBoundingVolume({50, 50, 50}, object2);
	
	w.addUpdater(std::ref(physics));
	w.addUpdater(std::ref(collision));
	
	size_t i = 0;
	while(true)
	{
		w.update(1.0 / 60);
		i++;
	}
	
	return 0;
}
