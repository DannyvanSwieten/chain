//
//  collision.cpp
//  chain
//
//  Created by Danny on 03/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#include "collision.hpp"

#include <cmath>
#include <iostream>

void CollisionUpdater::operator()(World& w, double dt)
{
	updateState(w, dt);
	
	entitiesToUpdate.clear();
	w.getAllEntitiesWithComponents<Transform, RigidBody, BoundingBox>(entitiesToUpdate);
    
    const auto& transforms = w.getAll<Transform>();
    auto& boxes = w.getAll<BoundingBox>();
    
    for(const auto e: entitiesToUpdate)
        boxes[e]->center = transforms[e]->position;
	
	for(auto i = 0; i < entitiesToUpdate.size(); i++)
	{
		auto& a = boxes[i];
		for(auto j = i + 1; j < entitiesToUpdate.size(); j++)
		{
			const size_t index = j % boxes.size();
			auto& b = boxes[index];
			
			bool x = std::fabs(a->center.x - b->center.x) < std::fabs(a->radi.x + b->radi.x);
			bool y = std::fabs(a->center.y - b->center.y) < std::fabs(a->radi.y + b->radi.y);
			bool z = std::fabs(a->center.z - b->center.z) < std::fabs(a->radi.z + b->radi.z);
			
			bool collision = x & y & z;
			
			if(!collision)
				continue;
			
			std::cout << "Collision of entity " << i << " with " << j << '\n';
		}
	}
}

void CollisionUpdater::setBoundingVolume(const vec3 &bounds, World::Entity e)
{
	stateUpdates.emplace_back([bounds, e](World& w, double dt) {
		w.getAll<BoundingBox>()[e]->radi = bounds * 0.5;
	});
}

void CollisionUpdater::updateState(World &w, double dt)
{
	for(auto& update: stateUpdates)
		update(w, dt);
}
