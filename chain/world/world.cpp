//
//  world.cpp
//  chain
//
//  Created by Danny on 29/03/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#include "world.hpp"

World::World(size_t numDefaultStoragePerComponent)
{
	collection.reserve(numDefaultStoragePerComponent);
}

World::Entity World::createEntity()
{
	Entity o = -1;
	if(freeList.size())
	{
		o = freeList.back();
		freeList.pop();
	}
	else
	{
		if(numEntitys >= collection.size())
			collection.expand();
		
		o = numEntitys;
	}
	
	numEntitys++;
    
    attach<Transform>(o);
	
	return o;
}

void World::freeEntity(Entity Entity)
{
	collection.resetAll(Entity);
	freeList.push(Entity);
	numEntitys--;
}

void World::setPosition(const vec3 &position, Entity entity)
{
    getAll<Transform>()[entity]->position = position;
}

void World::addUpdater(std::function<void(World&, double)> updater)
{
	updaters.emplace_back(updater);
}

void World::update(double dt)
{
	for(auto& updater: updaters)
		updater(*this, dt);
}
