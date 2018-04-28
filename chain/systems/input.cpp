//
//  input.cpp
//  chain
//
//  Created by Danny van Swieten on 04/04/2018.
//  Copyright © 2018 Danny. All rights reserved.
//

#include "input.hpp"

void keyCallback(GLFWwindow* window, int x, int y, int z, int w)
{
    auto self = static_cast<InputUpdater*>(glfwGetWindowUserPointer(window));
    self->registerKeyPress(x, y, z, w);
}

InputUpdater::InputUpdater(GLFWwindow *window): window(window)
{
    assert(window);
	
    glfwSetKeyCallback(window, keyCallback);
    glfwSetWindowUserPointer(window, this);
}

void InputUpdater::operator()(World &w, double dt)
{
	for(auto& b: pressedThisFrame)
		b = false;
	
    glfwPollEvents();
}

void InputUpdater::registerKeyPress(int key, int scanCode, int action, int mods)
{
	bool down = action == GLFW_PRESS;
	if(down)
		pressedThisFrame[key] = down;
	
	currentlyPressed[key] = down;
}

bool InputUpdater::isPressed(int c) const
{
	return currentlyPressed[c];
}

bool InputUpdater::wasPressedThisFrame(int c) const
{
	return pressedThisFrame[c];
}

void InputUpdater::reflect(chaiscript::ChaiScript &context)
{
    context.add_global(chaiscript::var(this), "inputController");
    context.add(chaiscript::fun(&InputUpdater::isPressed), "isPressed");
}


