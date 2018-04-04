//
//  world.hpp
//  chain
//
//  Created by Danny on 29/03/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#pragma once

#include <queue>

#include "component_collection.hpp"

class World
{
public:
	
	using Entity = size_t;
	
	World(size_t numDefaultStoragePerComponent = 512);
	
	Entity createEntity();
	
	void freeEntity(Entity entity);
    
    void setPosition(const vec3& position, Entity e);
    void setRotation(const vec3& rotation, Entity e);
    void setScale(const vec3& scale, Entity e);
    
    void registerKeyPress(int);
	
	template<class C>
	void attach(Entity Entity)
	{
		collection.initialize<C>(Entity);
	}
	
	template<typename C>
	bool has(Entity e)
	{
		return std::get<ComponentStorage<C>>(collection).has(e);
	}
	
	template<class C>
	auto begin()
	{
		return collection.begin<C>();
	}
	
	template<class C>
	auto end()
	{
		return collection.end<C>();
	}
	
	template<typename C>
	auto& getAll()
	{
		return collection.getStorage<C>();
	}
	
	auto size() const noexcept
	{
		return collection.size();
	}
    
    template<typename... Ts>
    auto getAllEntitiesWithComponents(std::vector<Entity>& output)
    {
        collection.getAllEntitiesWithComponents<Ts...>(output);
    }
	
	void addUpdater(std::function<void(World&, double)> updater);
	
	void update(double);
	
private:
	
	size_t numEntitys = 0;
	std::queue<Entity> freeList;
	component_collection_t collection;
	
	std::vector<std::function<void(World&, double)>> updaters;
};
