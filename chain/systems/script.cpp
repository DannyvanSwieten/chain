//
//  script.cpp
//  chain
//
//  Created by Danny on 05/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#include "script.hpp"
#include "mesh_filter.hpp"

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
	chai.add(chaiscript::user_type<MeshFilter>(), "Mesh");
	chai.add(chaiscript::constructor<MeshFilter()>(), "Mesh");
	
	using position_list = std::vector<vec3>;
	using normal_list = std::vector<vec3>;
	using uv_list = std::vector<vec2>;
	using face_list = std::vector<vec3i>;
	
	chai.add(chaiscript::bootstrap::standard_library::vector_type<position_list>("PositionList"));
	chai.add(chaiscript::bootstrap::standard_library::vector_type<normal_list>("NormalList"));
	chai.add(chaiscript::bootstrap::standard_library::vector_type<uv_list>("UvList"));
	chai.add(chaiscript::bootstrap::standard_library::vector_type<face_list>("FaceList"));
	
	chai.add(chaiscript::fun(&MeshFilter::positions), "positions");
	chai.add(chaiscript::fun(&MeshFilter::normals), "normals");
	chai.add(chaiscript::fun(&MeshFilter::uv), "uv");
	chai.add(chaiscript::fun(&MeshFilter::faces), "faces");
	
	chai.add(chaiscript::fun([](vec2& lhs, const vec2& rhs){lhs = rhs;}), "=");
	chai.add(chaiscript::constructor<vec2(float, float)>(), "Vec2");
	
	chai.add(chaiscript::fun([](vec3& lhs, const vec3& rhs){lhs = rhs;}), "=");
	chai.add(chaiscript::constructor<vec3(float, float, float)>(), "Vec3");
	
	chai.add(chaiscript::fun([](vec4& lhs, const vec4& rhs){lhs = rhs;}), "=");
	chai.add(chaiscript::constructor<vec4(float, float, float, float)>(), "Vec4");
	
	chai.add(chaiscript::fun([](vec3i& lhs, const vec3i& rhs){lhs = rhs;}), "=");
	chai.add(chaiscript::constructor<vec3i(uint32_t, uint32_t, uint32_t)>(), "Face");
	
	chai.add(chaiscript::fun(&vec2::x), "x");
	chai.add(chaiscript::fun(&vec2::y), "y");
	
	chai.add(chaiscript::fun(&vec3::x), "x");
	chai.add(chaiscript::fun(&vec3::y), "y");
	chai.add(chaiscript::fun(&vec3::z), "z");
	
	chai.add(chaiscript::fun(&vec4::x), "x");
	chai.add(chaiscript::fun(&vec4::y), "y");
	chai.add(chaiscript::fun(&vec4::z), "z");
	chai.add(chaiscript::fun(&vec4::w), "w");
	
	chai.add(chaiscript::fun(&vec3i::x), "a");
	chai.add(chaiscript::fun(&vec3i::y), "b");
	chai.add(chaiscript::fun(&vec3i::z), "c");
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
		{
			try {
				script->update(e, dt);
			}catch(std::exception& e) {
				std::cout << e.what() << '\n';
				script->update = nullptr;
			}
		}
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
			
			auto creator = chai.eval<std::function<std::function<void (size_t, double)> (World::Entity)>>(source);
			
			creators.emplace(name, creator);
		}
		
		auto& script = w.getAll<Script>()[e];
		script->update = creators[name](e);
	});
}
