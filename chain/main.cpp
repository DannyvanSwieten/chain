//
//  main.cpp
//  chain
//
//  Created by Danny on 27/03/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#include <iostream>
#include <queue>

#include "world.hpp"
#include "systems/camera_system.hpp"
#include "systems/physics.hpp"
#include "systems/collision.hpp"
#include "systems/input.hpp"
#include "systems/renderer.hpp"
#include "systems/mesh.hpp"
#include "systems/script.hpp"

#include <GLFW/glfw3.h>

int main(int argc, const char * argv[]) {
    
    const auto result = glfwInit();
    if(result != GLFW_TRUE)
        exit(1);
    
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    
    GLFWwindow* window = nullptr;
    window = glfwCreateWindow(1080, 720, "Ja toch", nullptr, nullptr);
    assert(window);
    
    glfwShowWindow(window);
    
    World w(10);
    Renderer renderer(window);
    CameraUpdater cameraUpdater;
    InputUpdater input(window);
	PhysicsUpdater physics;
	CollisionUpdater collision;
	StaticMeshUpdater meshUpdater;
	ScriptUpdater scriptUpdater;
    
    scriptUpdater.getContext().add_global(chaiscript::var(w), "world");
    scriptUpdater.getContext().add(chaiscript::fun(&World::getEntityByName), "getEntityByName");
    scriptUpdater.getContext().add(chaiscript::fun(&World::setPosition), "setPosition");
    scriptUpdater.getContext().add((chaiscript::fun(&World::getPosition)), "getPosition");
	
	scriptUpdater.getContext().add_global(chaiscript::var(std::ref(meshUpdater)), "meshSystem");
	scriptUpdater.getContext().add(chaiscript::fun(&StaticMeshUpdater::updateMesh), "updateMesh");
	scriptUpdater.getContext().add(chaiscript::fun(&StaticMeshUpdater::setMaterialProperty<float>), "setMaterialProperty");
	scriptUpdater.getContext().add(chaiscript::fun(&StaticMeshUpdater::setMaterialProperty<vec2>), "setMaterialProperty");
	scriptUpdater.getContext().add(chaiscript::fun(&StaticMeshUpdater::setMaterialProperty<vec3>), "setMaterialProperty");
	scriptUpdater.getContext().add(chaiscript::fun(&StaticMeshUpdater::setMaterialProperty<vec4>), "setMaterialProperty");
    
    scriptUpdater.getContext().add_global(chaiscript::var(std::ref(cameraUpdater)), "cameraSystem");
    scriptUpdater.getContext().add(chaiscript::fun(&CameraUpdater::setMainCamera), "setMainCamera");
    scriptUpdater.getContext().add(chaiscript::fun(&CameraUpdater::setFieldOfView), "setFieldOfView");
    scriptUpdater.getContext().add(chaiscript::fun(&CameraUpdater::setFieldOfView), "setZNear");
    scriptUpdater.getContext().add(chaiscript::fun(&CameraUpdater::setFieldOfView), "setZFar");
    scriptUpdater.getContext().add(chaiscript::fun(&CameraUpdater::lookAt), "lookAt");
	
	scriptUpdater.getContext().add_global(chaiscript::var(std::ref(input)), "inputController");
	scriptUpdater.getContext().add(chaiscript::fun(&InputUpdater::isPressed), "isPressed");
	
	scriptUpdater.getContext().add_global(chaiscript::var(std::ref(physics)), "physicsSystem");
	scriptUpdater.getContext().add(chaiscript::fun(&PhysicsUpdater::applyForce), "applyForce");
	
	scriptUpdater.getContext().add_global(chaiscript::var(std::ref(collision)), "collisionSystem");
	scriptUpdater.getContext().add(chaiscript::fun(&CollisionUpdater::registerCollisionProbe), "registerCollisionProbe");
	
	w.addUpdater(std::ref(renderer));
	w.addUpdater(std::ref(scriptUpdater));
    w.addUpdater(std::ref(cameraUpdater));
	w.addUpdater(std::ref(meshUpdater));
    w.addUpdater(std::ref(collision));
    w.addUpdater(std::ref(physics));
	w.addUpdater(std::ref(input));
    
    const auto e = w.createEntity();
	w.attach<Script>(e);
	w.attach<RigidBody>(e);
//    w.attach<SpringJoint>(e);
	w.attach<BoundingBox>(e);
    w.attach<Camera>(e);
	
	w.setPosition(e, {0.0, 0, -5});
	w.setScale(e, {0.5, 0.5, 0.5});
	
    scriptUpdater.load("/Users/dannyvanswieten/Desktop/RedSquare.chai", e);
	
	const auto e2 = w.createEntity();
	w.attach<Script>(e2);
	w.attach<RigidBody>(e2);
//    w.attach<SpringJoint>(e2);
	w.attach<BoundingBox>(e2);
	
	w.setPosition(e2, {0, 0, 5});
	w.setScale(e2, {0.5, 0.5, 0.5});
	
    scriptUpdater.load("/Users/dannyvanswieten/Desktop/GreenSquare.chai", e2);
	
	size_t i = 0;
	while(true)
	{
		w.update(1.0 / 60);
		i++;
	}
	
	return 0;
}
