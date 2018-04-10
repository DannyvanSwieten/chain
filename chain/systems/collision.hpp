//
//  collision.hpp
//  chain
//
//  Created by Danny on 03/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#include "../world/world.hpp"

#pragma once

class CollisionUpdater
{
public:
	
	void operator()(World& w, double dt);
	void setBoundingVolume(const vec3& bounds, World::Entity e);
	void registerCollisionProbe(World::Entity e, std::function<void(World::Entity, World::Entity)> f);
	
private:
	
	void updateState(World& w, double dt);
	
private:
	
	std::vector<World::Entity> entitiesToUpdate;
	std::vector<std::function<void(World&, double)>> stateUpdates;
	std::map<World::Entity, std::function<void(World::Entity, World::Entity)>> collisionProbes;
};
