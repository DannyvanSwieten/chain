//
//  world.hpp
//  chain
//
//  Created by Danny on 29/03/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#pragma once

#include <queue>

#include "component_storage.hpp"

class World
{
public:
	
	using Object = size_t;
	
	World(size_t numDefaultStoragePerComponent = 512);
	
	Object createObject();
	
	void freeObject(Object object);
	
	template<class C>
	void attach(Object object)
	{
		collection.initialize<C>(object);
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
	
	void addUpdater(std::function<void(World&, double)> updater);
	
	void update(double);
	
private:
	
	size_t numObjects = 0;
	std::queue<Object> freeList;
	ComponentStorageCollection<float, int> collection;
	
	std::vector<std::function<void(World&, double)>> updaters;
};
