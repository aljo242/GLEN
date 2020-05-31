#pragma once

#include "Timer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace GLEN;

void Timer::update(const bool trackFPS) noexcept
{
	const float currentTime {static_cast<float>(glfwGetTime())};
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;
	
	if (trackFPS && (( frameCount % 60) == 0)) // check every 60 frames
	{
		fpsCounter = (currentTime - lastWindow) / static_cast<float>(frameCount);
		lastWindow = currentTime;
	}

	++frameCount;
}