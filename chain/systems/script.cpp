//
//  script.cpp
//  chain
//
//  Created by Danny on 05/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#include "script.hpp"
#include "mesh_filter.hpp"

#include <OpenGL/gl3.h>

const std::string CreatorTemplate = R"(
// this lambda function is the last statement and therefor its value is simply
// returned when the script is done executing
fun(entity) {
  var f = fun(obj, self, timestep) {
	  obj.update(self, timestep)
	  return obj;
  }
  var s = <<SCRIPT_NAME>>(entity);
  return bind(f, s, _, _)
}
)";

ScriptUpdater::ScriptUpdater()
{

}

void ScriptUpdater::operator()(World& w, double dt)
{
	
	entitiesToUpdate.clear();
	w.getAllEntitiesWithComponents<Script>(entitiesToUpdate);
	auto& scripts = w.getAll<Script>();
	for(auto e: entitiesToUpdate)
	{
		auto& script = scripts[e];
		if(script->update)
		{
			try {
				script->update(e, dt);
			}catch(chaiscript::exception::eval_error& e) {
				std::cout << e.pretty_print();
				script->update = nullptr;
			}
		}
	}
}

void ScriptUpdater::load(const std::string& path, World::Entity e)
{
	scheduleStateUpdate([&, path, e](World& w, double dt)
	{
        if(!w.has<Script>(e))
            w.attach<Script>(e);
        
		const auto slashPos = path.find_last_of("/");
		const auto name = path.substr(slashPos + 1, path.size() - 6 - slashPos).substr();
		if(creators.find(name) == creators.end())
		{
			try {
				
				chai.eval_file(path);
				auto t = CreatorTemplate;
				const std::string toReplace = "<<SCRIPT_NAME>>";
				const auto pos = CreatorTemplate.find(toReplace);
				const auto source = t.replace(pos, toReplace.size(), name);
				
				auto creator = chai.eval<std::function<std::function<void (size_t, double)> (World::Entity)>>(source);
				
				creators.emplace(name, creator);
				
			} catch(chaiscript::exception::eval_error& e) {
				
				std::cout << e.pretty_print();
				return;
			}
		}
		
		auto& script = w.getAll<Script>()[e];
		script->update = creators[name](e);
	});
}
