#pragma once

#include "Timer.h"
#include "Log.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>


using namespace GLEN;

constexpr unsigned long  frameWindow {60};
constexpr double   frameWindowDouble {60.0};

void Timer::update(const bool trackFPS) noexcept
{
	const double currentTime {glfwGetTime()};
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;
	
	if (trackFPS && (( frameCount % frameWindow) == 0ul)) // check every 60 frames
	{
		fpsCounter = (currentTime - lastWindow) / frameWindowDouble;
		lastWindow = currentTime;
		logFPS();
	} 

	++frameCount;
}

void Timer::logFPS() const noexcept
{
	GLEN_CRITICAL("FPS: {}", 1.0/(fpsCounter));
}