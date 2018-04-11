//
//  camera_system.hpp
//  chain
//
//  Created by Danny van Swieten on 11/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#pragma once

#include "../world/world.hpp"

class CameraUpdater
{
public:
    
    void operator()(World& w, double dt);
    
    void setFieldOfView(World::Entity e, float fov);
    void setZNear(World::Entity e, float zNear);
    void setZFar(World::Entity e, float zFar);
    
private:
    
    std::vector<std::function<void(World&, double)>> updates;
};
