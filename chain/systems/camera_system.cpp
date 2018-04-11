//
//  camera_system.cpp
//  chain
//
//  Created by Danny van Swieten on 11/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#include "camera_system.hpp"

#include <moditone/bear/graphics/perspective.hpp>

void CameraUpdater::operator()(World &w, double dt)
{
    for(auto& update: updates)
        update(w, dt);
    
    updates.clear();
}

void CameraUpdater::setZFar(World::Entity e, float zFar)
{
    updates.emplace_back([e, zFar](World& w, double dt) {
        std::vector<World::Entity> entities;
        auto& camera = w.getAll<Camera>()[e];
        camera->zFar = zFar;
        
        camera->viewMatrix = bear::graphics::perspective(unit::degree<float>{camera->fieldOfView},
                                                         3.0f / 4.0f,
                                                         camera->zNear, camera->zFar);
    });
}


void CameraUpdater::setZNear(World::Entity e, float zNear)
{
    updates.emplace_back([e, zNear](World& w, double dt) {
        std::vector<World::Entity> entities;
        auto& camera = w.getAll<Camera>()[e];
        camera->zNear = zNear;
        
        camera->viewMatrix = bear::graphics::perspective(unit::degree<float>{camera->fieldOfView},
                                                         3.0f / 4.0f,
                                                         camera->zNear, camera->zFar);
    });
}


void CameraUpdater::setFieldOfView(World::Entity e, float fov)
{
    updates.emplace_back([e, fov](World& w, double dt) {
        std::vector<World::Entity> entities;
        
        auto& camera = w.getAll<Camera>()[e];
        camera->fieldOfView = fov;
        
        camera->viewMatrix = bear::graphics::perspective(unit::degree<float>{camera->fieldOfView},
                                                         3.0f / 4.0f,
                                                         camera->zNear, camera->zFar);
    });
}
