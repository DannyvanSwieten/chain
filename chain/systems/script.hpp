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
#include "system.hpp"

class ScriptUpdater: public System
{
public:
	
	ScriptUpdater();
	
	void operator()( World&, double ) final;
	void load(const std::string& source, World::Entity e);
	
	chaiscript::ChaiScript& getContext()
	{
		return chai;
	}
	
private:
	
	chaiscript::ChaiScript chai;
	
	std::vector<World::Entity> entitiesToUpdate;
	
	std::map<std::string, std::function<std::function<void (size_t, double)> (World::Entity)>> creators;
};
