//
//  mesh.hpp
//  chain
//
//  Created by Danny on 05/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#pragma once

#include "../world/world.hpp"

#include "mesh_filter.hpp"

class StaticMeshUpdater
{
public:
	
	struct Vertex
	{
		vec3 position;
		vec3 normal;
		vec2 uv;
	};
	
	void operator()(World&, double);
	void updateMesh(World::Entity e, const MeshFilter& filter);
	
	template<typename T>
	void setMaterialProperty(World::Entity e, const std::string& name, const T& value)
	{
		updates.emplace_back([e, name, value](World& w, double dt) {
			w.getAll<StaticMesh>()[e]->material.properties[name] = value;
		});
	}
	
private:
	
	void createMeshForEntity(World& w, World::Entity e);
	void updateMeshFromFilter(const MeshFilter& filter, World& w, World::Entity e);
	
private:
	
	std::vector<std::function<void(World&, double)>> updates;
};
