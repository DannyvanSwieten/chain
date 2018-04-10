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

void World::setName(Entity e, const std::string &name)
{
	entityNames[e] = name;
}

void World::setPosition(Entity e, const vec3 &position)
{
    getAll<Transform>()[e]->position = position;
}

void World::setRotation(Entity e, const vec3 &rotation)
{
    getAll<Transform>()[e]->rotation = rotation;
}

void World::setScale(Entity e, const vec3 &scale)
{
    getAll<Transform>()[e]->scale = scale;
    if(has<BoundingBox>(e))
        getAll<BoundingBox>()[e]->radi *= scale;
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
