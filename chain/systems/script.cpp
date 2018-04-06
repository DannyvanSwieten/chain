//
//  script.cpp
//  chain
//
//  Created by Danny on 05/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#include "script.hpp"

const std::string CreatorTemplate = R"(
// this lambda function is the last statement and therefor its value is simply
// returned when the script is done executing
fun() {
  var f = fun(obj, self, timestep) {
	  obj.update(self, timestep)
	  return obj;
  }
  var s = <<SCRIPT_NAME>>();
  return bind(f, s, _, _)
}
)";

ScriptUpdater::ScriptUpdater()
{
	chai.add(chaiscript::user_type<MeshFilter>(), "MeshFilter");
	chai.add(chaiscript::fun(&MeshFilter::positions), "positions");
	chai.add(chaiscript::fun(&MeshFilter::normals), "normals");
	chai.add(chaiscript::fun(&MeshFilter::uv), "uv");
	
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
	
	stateUpdates.clear();
	
	entitiesToUpdate.clear();
	w.getAllEntitiesWithComponents<Script>(entitiesToUpdate);
	auto& scripts = w.getAll<Script>();
	for(auto e: entitiesToUpdate)
	{
		auto& script = scripts[e];
		if(script->update)
			script->update(e, dt);
	}
}

void ScriptUpdater::load(const std::string& path, World::Entity e)
{
	stateUpdates.emplace_back([&, path, e](World& w, double dt)
	{
		const auto slashPos = path.find_last_of("/");
		const auto name = path.substr(slashPos + 1, path.size() - 6 - slashPos).substr();
		if(creators.find(name) == creators.end())
		{
			chai.eval_file(path);
			
			auto t = CreatorTemplate;
			const std::string toReplace = "<<SCRIPT_NAME>>";
			const auto pos = CreatorTemplate.find(toReplace);
			const auto source = t.replace(pos, toReplace.size(), name);
			
			auto creator = chai.eval<std::function<std::function<void (size_t, double)> (void)>>(source);
			
			creators.emplace(name, creator);
		}
		
		auto& script = w.getAll<Script>()[e];
		script->update = creators[name]();
	});
}
