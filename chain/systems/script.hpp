//
//  script.hpp
//  chain
//
//  Created by Danny on 05/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#pragma once

#include <map>
#include <chaiscript/chaiscript.hpp>

#include "../world/world.hpp"

class ScriptUpdater
{
public:
	
	ScriptUpdater();
	
	void operator()( World&, double );
	void load(const std::string& source, World::Entity e);
	
	chaiscript::ChaiScript& getContext()
	{
		return chai;
	}
	
private:
	
	chaiscript::ChaiScript chai;
	
	std::vector<World::Entity> entitiesToUpdate;
	
	std::vector<std::function<void(World&, double)>> stateUpdates;
	std::map<std::string, std::function<std::function<void (size_t, double)> (World::Entity)>> creators;
};
