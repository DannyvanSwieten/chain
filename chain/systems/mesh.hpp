//
//  mesh.hpp
//  chain
//
//  Created by Danny on 05/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#pragma once

#include "../world/world.hpp"

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
	void updateMesh(World::Entity e);
	
private:
	
	void createMeshForEntity(World& w, World::Entity e);
	void updateMeshFromFilter(World& w, World::Entity e);
	
private:
	
	std::vector<std::function<void(World&, double)>> updates;
};
