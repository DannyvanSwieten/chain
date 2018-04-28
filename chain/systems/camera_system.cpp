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
    
}

void CameraUpdater::lookAt(World::Entity e, const vec3 &target)
{
    scheduleStateUpdate([e, target](World& w, double dt) {
        auto& camera = w.getAll<Camera>()[e];
        camera->position = w.getAll<Transform>()[e]->position;
        camera->viewMatrix = bear::graphics::lookAt(camera->position, target, camera->up);
        camera->perspectiveMatrix = bear::graphics::perspective(unit::degree<float>{camera->fieldOfView},
                                                                4.0f / 3.0f,
                                                                camera->zNear, camera->zFar);
    });
}

void CameraUpdater::setZFar(World::Entity e, float zFar)
{
    scheduleStateUpdate([e, zFar](World& w, double dt) {
        auto& camera = w.getAll<Camera>()[e];
        camera->position = w.getAll<Transform>()[e]->position;
        camera->zFar = zFar;
        camera->perspectiveMatrix = bear::graphics::perspective(unit::degree<float>{camera->fieldOfView},
                                                                4.0f / 3.0f,
                                                                camera->zNear, camera->zFar);
    });
}

void CameraUpdater::setZNear(World::Entity e, float zNear)
{
    scheduleStateUpdate([e, zNear](World& w, double dt) {
        auto& camera = w.getAll<Camera>()[e];
        camera->position = w.getAll<Transform>()[e]->position;
        camera->zNear = zNear;
        
        camera->perspectiveMatrix = bear::graphics::perspective(unit::degree<float>{camera->fieldOfView},
                                                                4.0f / 3.0f,
                                                                camera->zNear, camera->zFar);
        
    });
}


void CameraUpdater::setFieldOfView(World::Entity e, float fov)
{
    scheduleStateUpdate([e, fov](World& w, double dt) {
        auto& camera = w.getAll<Camera>()[e];
        camera->position = w.getAll<Transform>()[e]->position;
        camera->fieldOfView = fov;
        camera->perspectiveMatrix = bear::graphics::perspective(unit::degree<float>{camera->fieldOfView},
                                                                4.0f / 3.0f,
                                                                camera->zNear, camera->zFar);
    });
}

void CameraUpdater::setMainCamera(World::Entity e)
{
    scheduleStateUpdate([e](World& w, double) {
        
        if(!w.has<Camera>(e))
            w.attach<Camera>(e);
        
        auto& camera = w.getAll<Camera>()[e];
        camera->position = w.getAll<Transform>()[e]->position;
        camera->perspectiveMatrix = bear::graphics::perspective(unit::degree<float>{camera->fieldOfView},
                                                                4.0f / 3.0f,
                                                                camera->zNear, camera->zFar);
        
        w.mainCamera = &(*w.getAll<Camera>()[e]);
    });
}

void CameraUpdater::reflect(chaiscript::ChaiScript &context)
{
    context.add_global(chaiscript::var(this), "cameraSystem");
    context.add(chaiscript::fun(&CameraUpdater::setMainCamera), "setMainCamera");
    context.add(chaiscript::fun(&CameraUpdater::setFieldOfView), "setFieldOfView");
    context.add(chaiscript::fun(&CameraUpdater::setFieldOfView), "setZNear");
    context.add(chaiscript::fun(&CameraUpdater::setFieldOfView), "setZFar");
    context.add(chaiscript::fun(&CameraUpdater::lookAt), "lookAt");
}
