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
	const auto object = w.createObject();
	w.attach<float>(object);
	w.attach<int>(object);
	
	w.addUpdater([](World& w, double dt){
		
		auto& floatData = w.getAll<float>();
		auto& intData = w.getAll<int>();
		
		for(auto i = 0; i < w.size(); i++)
		{
			floatData[i];
			intData[i];
			std::cout << "Process data." << std::endl;
		}
	});
	
	w.update(1.0 / 60);
	
	return 0;
}
