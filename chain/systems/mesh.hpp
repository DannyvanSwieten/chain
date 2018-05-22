//
//  mesh.hpp
//  chain
//
//  Created by Danny on 05/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#pragma once

#include <moditone/bear/graphics/mesh_builder.hpp>

#include "../world/world.hpp"

#include "mesh_filter.hpp"
#include "system.hpp"

class StaticMeshUpdater: public System
{
public:
	
	struct Vertex
	{
		vec3 position;
		vec3 normal;
		vec2 uv;
	};
	
    StaticMeshUpdater(World& w);
	void operator()(World&, double) final;
	void updateMesh(World::Entity e);
	
	template<typename T>
	void setMaterialProperty(World::Entity e, const std::string& name, const T& value)
	{
		scheduleStateUpdate([e, name, value](World& w, double dt) {
			w.getAll<StaticMesh>()[e]->material.properties[name] = value;
		});
	}
    
    void reflect(chaiscript::ChaiScript& ) override;
    void reflect(lua_State* ) override;
    
    void setPrimitiveType(World::Entity e, uint32_t primitiveType);
    void setPositions(World::Entity e, const vector_wrapper<vec3>& positions);
    void setFaces(World::Entity e, const vector_wrapper<vec3i>& faces);
    
    void buildSphere(World::Entity e, size_t tessLevel);
    	
private:
	
	void createMeshForEntity(World& w, World::Entity e);
	void updateMeshFromFilter(World& w, World::Entity e);
    
private:
    
    bear::graphics::MeshBuilder builder;
};
