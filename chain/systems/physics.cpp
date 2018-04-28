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
        body->momentum.y += -9.8 * dt;
		transform->position += body->velocity * dt;
		
//        std::cout << transform->position << std::endl;
	}
}

void PhysicsUpdater::applyForce(World::Entity e, const vec3& F)
{
	scheduleStateUpdate([F, e] (World& w, double dt) {
		w.getAll<RigidBody>()[e]->momentum += F * dt;
	});
}

void PhysicsUpdater::setMass(World::Entity e, math_precision_t mass)
{
	assert(mass != 0.0);
	
	scheduleStateUpdate([mass, e](World& w, double dt) {
		w.getAll<RigidBody>()[e]->invMass = 1.0 / mass;
	});
}

void PhysicsUpdater::reflect(chaiscript::ChaiScript &ctx)
{
    ctx.add_global(chaiscript::var(this), "physicsSystem");
    ctx.add(chaiscript::fun(&PhysicsUpdater::applyForce), "applyForce");
}

