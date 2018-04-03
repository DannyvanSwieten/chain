//
//  main.cpp
//  chain
//
//  Created by Danny on 27/03/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#include <iostream>
#include <queue>

//#include <chaiscript/chaiscript.hpp>

#include "world.hpp"

int main(int argc, const char * argv[]) {

	World w(10);
	const auto object = w.createEntity();
	w.attach<Transform>(object);
    
    const auto object2 = w.createEntity();
    
    const auto object3 = w.createEntity();
    
    const auto object4 = w.createEntity();
    w.attach<Transform>(object4);
	
	w.addUpdater([](World& w, double dt){
		
        std::vector<World::Entity> entitiesToUpdate;
        w.getAllEntitiesWithComponents<Transform>(entitiesToUpdate);
        
        auto& transforms = w.getAll<Transform>();
		
        for(const auto e: entitiesToUpdate)
			std::cout << transforms[e]->position << std::endl;
	});
	
	w.update(1.0 / 60);
	
	return 0;
}
