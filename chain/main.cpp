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
        engine.getLuaContext().loadScript(gridEntity, "/Users/dannyvanswieten/Desktop/UniformGrid.lua");
        
        sphereEntity = engine.getWorld().createEntity();
        engine.getLuaContext().loadScript(sphereEntity, "/Users/dannyvanswieten/Desktop/Sphere.lua");
        
        cameraEntity = engine.getWorld().createEntity();
        engine.getLuaContext().loadScript(cameraEntity, "/Users/dannyvanswieten/Desktop/Camera.lua");
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
    World::Entity sphereEntity;
    
    World::Entity luaEntity;
};

int main(int argc, const char * argv[])
{
    MyGame myGame;
    myGame.start();
	
	return 0;
}
