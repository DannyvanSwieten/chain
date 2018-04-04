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
    
    w.addUpdater(std::ref(renderer));
    w.addUpdater(std::ref(input));
    w.addUpdater(std::ref(collision));
    w.addUpdater(std::ref(physics));
    w.addUpdater(std::ref(collision));
	
    const auto object = w.createEntity();
    w.attach<RigidBody>(object);
    w.attach<BoundingBox>(object);
    
    physics.setMass(100.0, object);
    collision.setBoundingVolume({50, 50, 50}, object);
    
    const auto object2 = w.createEntity();
    w.attach<RigidBody>(object2);
    w.attach<BoundingBox>(object2);
    w.attach<SpringJoint>(object2);
    w.setPosition({0, 100, 0}, object2);
    
    physics.setMass(100.0, object2);
    collision.setBoundingVolume({50, 50, 50}, object2);
	
	size_t i = 0;
	while(true)
	{
		w.update(1.0 / 60);
		i++;
	}
	
	return 0;
}
