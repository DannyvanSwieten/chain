//
//  script.cpp
//  chain
//
//  Created by Danny on 05/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#include "script.hpp"

ScriptUpdater::ScriptUpdater()
{
	chai.add(chaiscript::constructor<vec3(float, float, float)>(), "vec2");
	chai.add(chaiscript::constructor<vec3(float, float, float)>(), "vec3");
	chai.add(chaiscript::constructor<vec3(float, float, float)>(), "vec4");
	
	chai.add(chaiscript::fun(&vec2::x), "x");
	chai.add(chaiscript::fun(&vec2::y), "y");
	
	chai.add(chaiscript::fun(&vec3::x), "x");
	chai.add(chaiscript::fun(&vec3::y), "y");
	chai.add(chaiscript::fun(&vec3::z), "z");
	
	chai.add(chaiscript::fun(&vec4::x), "x");
	chai.add(chaiscript::fun(&vec4::y), "y");
	chai.add(chaiscript::fun(&vec4::z), "z");
	chai.add(chaiscript::fun(&vec4::w), "w");
}

void ScriptUpdater::operator()(World& w, double dt) {
	
	for(auto& update: stateUpdates)
		update(w, dt);
	
	entitiesToUpdate.clear();
	w.getAllEntitiesWithComponents<Script>(entitiesToUpdate);
	auto& scripts = w.getAll<Script>();
	for(auto e: entitiesToUpdate)
		chai.eval(scripts[e]->source);
}

void ScriptUpdater::load(const std::string& source, World::Entity e)
{
	stateUpdates.emplace_back([source, e](World& w, double dt){
		w.getAll<Script>()[e]->source = source;
	});
}
