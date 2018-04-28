//
//  main.cpp
//  chain
//
//  Created by Danny on 27/03/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#include "game.hpp"

#include <moditone/bear/graphics/mesh_builder.hpp>

class MyGame: public Game
{
public:
    MyGame(): Game("My Game")
    {
        gridEntity = engine.getWorld().createEntity();
        engine.getScriptSystem().load("/Users/dannyvanswieten/Desktop/UniformGrid.chai", gridEntity);
        
        cameraEntity = engine.getWorld().createEntity();
        engine.getScriptSystem().load("/Users/dannyvanswieten/Desktop/Camera.chai", cameraEntity);
    }
    
    void onStart() override
    {
        
    }
    
    void onPause() override
    {
        
    }
    
    void onStop() override
    {
        
    }
    
private:
    
    World::Entity gridEntity;
    World::Entity cameraEntity;
};

int main(int argc, const char * argv[])
{
    MyGame myGame;
    myGame.start();
	
	return 0;
}
