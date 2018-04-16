//
//  camera_system.cpp
//  chain
//
//  Created by Danny van Swieten on 11/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#include "camera_system.hpp"

#include <moditone/bear/graphics/perspective.hpp>
#include <moditone/bear/graphics/look_at.hpp>

void CameraUpdater::operator()(World &w, double dt)
{
    auto& cams = w.getAll<Camera>();
    auto& transforms = w.getAll<Transform>();
    std::vector<World::Entity> entities;
    w.getAllEntitiesWithComponents<Transform, Camera>(entities);
    
    for(const auto e: entities)
    {
        auto& cam = cams[e];
        auto& transform = transforms[e];
        cam->position = transform->position;
        cam->viewMatrix[3][0] = cam->position[0];
        cam->viewMatrix[3][1] = cam->position[1];
        cam->viewMatrix[3][2] = cam->position[2];
    }
    
    for(auto& update: updates)
        update(w, dt);
    
    updates.clear();
}

void CameraUpdater::lookAt(World::Entity e, const vec3 &target)
{
    updates.emplace_back([e, target](World& w, double dt) {
        std::vector<World::Entity> entities;
        auto& camera = w.getAll<Camera>()[e];
        camera->viewMatrix = bear::graphics::lookAt(camera->position, target, camera->up);
    });
}

void CameraUpdater::setZFar(World::Entity e, float zFar)
{
    updates.emplace_back([e, zFar](World& w, double dt) {
        std::vector<World::Entity> entities;
        auto& camera = w.getAll<Camera>()[e];
        camera->zFar = zFar;
        
        camera->perspectiveMatrix = bear::graphics::perspective(unit::degree<float>{camera->fieldOfView},
                                                                4.0f / 3.0f,
                                                                camera->zNear, camera->zFar);
    });
}

void CameraUpdater::setZNear(World::Entity e, float zNear)
{
    updates.emplace_back([e, zNear](World& w, double dt) {
        std::vector<World::Entity> entities;
        auto& camera = w.getAll<Camera>()[e];
        camera->zNear = zNear;
        
        camera->perspectiveMatrix = bear::graphics::perspective(unit::degree<float>{camera->fieldOfView},
                                                                4.0f / 3.0f,
                                                                camera->zNear, camera->zFar);
        
    });
}


void CameraUpdater::setFieldOfView(World::Entity e, float fov)
{
    updates.emplace_back([e, fov](World& w, double dt) {
        std::vector<World::Entity> entities;
        
        auto& camera = w.getAll<Camera>()[e];
        camera->fieldOfView = fov;
        
        camera->perspectiveMatrix = bear::graphics::perspective(unit::degree<float>{camera->fieldOfView},
                                                                4.0f / 3.0f,
                                                                camera->zNear, camera->zFar);
    });
}

void CameraUpdater::setMainCamera(World::Entity e)
{
    updates.emplace_back([e](World& w, double) {
        auto& camera = w.getAll<Camera>()[e];
        camera->perspectiveMatrix = bear::graphics::perspective(unit::degree<float>{camera->fieldOfView},
                                                                4.0f / 3.0f,
                                                                camera->zNear, camera->zFar);
        
        w.mainCamera = &(*w.getAll<Camera>()[e]);
    });
}
