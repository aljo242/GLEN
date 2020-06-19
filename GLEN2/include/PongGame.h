#pragma once

#include <vector>
#include <array>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "KeyState.h"

namespace GLEN
{

class PongGame
{
public:
	explicit PongGame(std::array<float, 2> leftInit, std::array<float, 2> rightInit, const char level);

	void UpdateState(KeyState ks);

	std::array<float, 2> ballPosition {0.0f, 0.0f};
	std::array<float, 2> leftPaddlePosition;
	std::array<float, 2> rightPaddlePosition;
	std::array<float, 2> boundingPoints {0.0f, 0.0f}; // using x and y offsets from the center points of the paddles

private:
	std::array<float, 2> ballVelocity;
	float leftPaddleVelocity{ 0.0f };
	float rightPaddleVelocity{ 0.0f };
	char trackingLevel;
	uint64_t collisionsCount{ 0 };

	void ResetBall();
};
} // namespace GLEN