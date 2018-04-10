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
    window = glfwCreateWindow(800, 600, "Ja toch", nullptr, nullptr);
    assert(window);
    
    glfwShowWindow(window);
    
    World w(10);
    Renderer renderer(window);
    InputUpdater input(window);
	PhysicsUpdater physics;
	CollisionUpdater collision;
	StaticMeshUpdater meshUpdater;
	ScriptUpdater scriptUpdater;
	
	scriptUpdater.getContext().add_global(chaiscript::var(std::ref(meshUpdater)), "meshSystem");
	scriptUpdater.getContext().add(chaiscript::fun(&StaticMeshUpdater::updateMesh), "updateMesh");
	
	scriptUpdater.getContext().add(chaiscript::var(std::ref(input)), "inputController");
	scriptUpdater.getContext().add(chaiscript::fun(&InputUpdater::isPressed), "isPressed");
	
	scriptUpdater.getContext().add(chaiscript::var(std::ref(physics)), "physicsSystem");
	scriptUpdater.getContext().add(chaiscript::fun(&PhysicsUpdater::applyForce), "applyForce");
	
	w.addUpdater(std::ref(renderer));
	w.addUpdater(std::ref(scriptUpdater));
	w.addUpdater(std::ref(meshUpdater));
    w.addUpdater(std::ref(collision));
    w.addUpdater(std::ref(physics));
	w.addUpdater(std::ref(input));
    
    const auto e = w.createEntity();
	w.attach<Script>(e);
	w.attach<RigidBody>(e);
	
	const auto e2 = w.createEntity();
	w.attach<Script>(e2);
	w.attach<RigidBody>(e2);
	
	scriptUpdater.load("/Users/danny/Desktop/Behaviour.chai", e);
	scriptUpdater.load("/Users/danny/Desktop/Behaviour.chai", e2);
	
	size_t i = 0;
	while(true)
	{
		w.update(1.0 / 60);
		i++;
	}
	
	return 0;
}
