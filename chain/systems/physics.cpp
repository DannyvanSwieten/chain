//
//  physics.cpp
//  chain
//
//  Created by Danny on 03/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#include "physics.hpp"

#include <iostream>

void PhysicsUpdater::operator()(World& w, double dt)
{
	preUpdate(w, dt);
	
	std::vector<World::Entity> entitiesToUpdate;
	entitiesToUpdate.clear();
	w.getAllEntitiesWithComponents<Transform, RigidBody>(entitiesToUpdate);
	
	auto& transforms = w.getAll<Transform>();
	auto& bodies = w.getAll<RigidBody>();
	
	for(const auto& e: entitiesToUpdate)
	{
		auto& body = bodies[e];
		auto& transform = transforms[e];
		if(w.has<SpringJoint>(e))
		{
			auto& spring = w.getAll<SpringJoint>()[e];
			const auto F = -spring->k * transform->position - spring->b * body->velocity;
			body->momentum += F * dt;
		}
		
		body->velocity = body->momentum * body->invMass;
//		body->momentum.y += -9.8 * dt;
		transform->position += body->velocity * dt;
		
		std::cout << transform->position << std::endl;
	}
}

void PhysicsUpdater::applyForce(const vec3& F, World::Entity e)
{
	stateUpdates.emplace_back([F, e] (World& w, double dt) {
		w.getAll<RigidBody>()[e]->momentum += F * dt;
	});
}

void PhysicsUpdater::setMass(math_precision_t mass, World::Entity e)
{
	assert(mass != 0.0);
	
	stateUpdates.emplace_back([mass, e](World& w, double dt) {
		w.getAll<RigidBody>()[e]->invMass = 1.0 / mass;
	});
}

void PhysicsUpdater::preUpdate(World& w, double dt)
{
	for(auto& f: stateUpdates)
		f(w, dt);
	
	stateUpdates.clear();
}
