//
//  world.cpp
//  chain
//
//  Created by Danny on 29/03/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#include "world.hpp"
#include "system.hpp"

World::World(size_t numDefaultStoragePerComponent)
{
	collection.reserve(numDefaultStoragePerComponent);
    registerNamedContructors(collection);
}

void World::reflect(chaiscript::ChaiScript& ctx)
{
    ctx.add_global(chaiscript::var(this), "world");
    ctx.add(chaiscript::fun(&World::getEntityByName), "getEntityByName");
    ctx.add(chaiscript::fun(&World::setPosition), "setPosition");
    ctx.add((chaiscript::fun(&World::getPosition)), "getPosition");
    
    for(auto& system: updaters)
        system->reflect(ctx);
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

const vec3& World::getPosition(Entity e)
{
    return getAll<Transform>()[e]->position;
}

void World::move(Entity e, const vec3 &offset)
{
    getAll<Transform>()[e]->position += offset;
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

World::Entity World::getEntityByName(const std::string &name)
{
    for(const auto& e: entityNames)
        if(name == e.second)
            return e.first;
    
    return -1;
}

void World::addUpdater(System* system)
{
	updaters.emplace_back(system);
}

void World::update(double dt)
{
	for(auto& updater: updaters)
		updater->update(*this, dt);
}
