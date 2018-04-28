//
//  camera_system.hpp
//  chain
//
//  Created by Danny van Swieten on 11/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#pragma once

#include "../world/world.hpp"
#include "system.hpp"

class CameraUpdater: public System
{
public:
    
    void operator()(World& w, double dt) final;
    
    void lookAt(World::Entity, const vec3& target);
    void setFieldOfView(World::Entity e, float fov);
    void setZNear(World::Entity e, float zNear);
    void setZFar(World::Entity e, float zFar);
    
    void setMainCamera(World::Entity e);
    
    void reflect(chaiscript::ChaiScript&) override;
};
