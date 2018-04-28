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
			
			const auto x = std::fabs(a->center.x - b->center.x) - std::fabs(a->radi.x + b->radi.x);
			const auto y = std::fabs(a->center.y - b->center.y) - std::fabs(a->radi.y + b->radi.y);
			const auto z = std::fabs(a->center.z - b->center.z) - std::fabs(a->radi.z + b->radi.z);
			
			bool collision = x < 0 & y < 0 & z < 0;
			
			if(!collision)
				continue;
			
			vec3 distance = vec3{fabs(x), fabs(y), fabs(z)};
			
			if(collisionProbes.find(entitiesToUpdate[i]) != collisionProbes.end())
				collisionProbes[i](i, j);
			
			if(collisionProbes.find(entitiesToUpdate[j]) != collisionProbes.end())
				collisionProbes[j](j, i);
		}
	}
}

void CollisionUpdater::setBoundingVolume(const vec3 &bounds, World::Entity e)
{
	scheduleStateUpdate([bounds, e](World& w, double dt) {
		w.getAll<BoundingBox>()[e]->radi = bounds * 0.5;
	});
}

void CollisionUpdater::registerCollisionProbe(World::Entity e, std::function<void (World::Entity, World::Entity)> f)
{
    collisionProbes[e] = f;
}

void CollisionUpdater::reflect(chaiscript::ChaiScript& ctx)
{
    ctx.add_global(chaiscript::var(this), "collisionSystem");
    ctx.add(chaiscript::fun(&CollisionUpdater::registerCollisionProbe), "registerCollisionProbe");
}
