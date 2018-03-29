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

World::Object World::createObject()
{
	Object o = -1;
	if(freeList.size())
	{
		o = freeList.back();
		freeList.pop();
	}
	else
	{
		if(numObjects >= collection.size())
			collection.expand();
		
		o = numObjects;
	}
	
	numObjects++;
	
	return o;
}

void World::freeObject(Object object)
{
	collection.resetAll(object);
	freeList.push(object);
	numObjects--;
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
