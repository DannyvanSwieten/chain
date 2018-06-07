//
//  world.hpp
//  chain
//
//  Created by Danny on 29/03/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#pragma once

#include <moodycamel/concurrentqueue.h>
#include <chaiscript/chaiscript.hpp>
#include <cxxabi.h>
#include <map>
#include <queue>

#include "component_collection.hpp"

class System;

class World
{
public:
	
	using Entity = size_t;
	
	World(size_t numDefaultStoragePerComponent = 512);
    void reflect(chaiscript::ChaiScript&);
    void reflect(lua_State*);
	
	Entity createEntity();
	
	void freeEntity(Entity entity);
	
	void setName(Entity e, const std::string& name);
    void move(Entity e, const vec3 &offset);
    void setPosition(Entity e, const vec3& position);
    const vec3& getPosition(Entity e);
    void setRotation(Entity e, const vec3& rotation);
    void setScale(Entity e, const vec3& scale);
    
    Entity getEntityByName(const std::string& name);
    
    template<class C>
    void attachAsync(Entity entity)
    {
        stateUpdates.enqueue([&, entity](World&){
            collection.initialize<C>(entity);
        });
    }
	
	template<class C>
	void attach(Entity entity)
	{
        collection.initialize<C>(entity);
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
    
    template<typename... Ts>
    void registerNamedContructors(ComponentStorageCollection<Ts...>& collection)
    {
        (registerNamedContructor<Ts>(), ...);
    }
    
    template<typename T>
    void registerNamedContructor()
    {
        std::function<void(Entity)> ctor = [&](Entity e) {
            attach<T>(e);
        };
        int status;
        std::string demangled = abi::__cxa_demangle(typeid(T).name(),0,0,&status);
        ctors.emplace(demangled, ctor);
    }
	
	void addUpdater(System* updater);
	
    void start();
	void update(double);
    
public:
    
    Camera* mainCamera = nullptr;
	
private:
	
    std::map<std::string, std::function<void(Entity)>> ctors;
	size_t numEntitys = 0;
	std::queue<Entity> freeList;
	component_collection_t collection;
    
	std::map<World::Entity, std::string> entityNames;
	
	std::vector<System*> systems;
    
    moodycamel::ConcurrentQueue<std::function<void(World&)>> stateUpdates;
};
