#include "PongGame.h"
#include <random>
#include "Log.h"

GLEN::PongGame::PongGame(std::array<float, 2> leftInit, std::array<float, 2> rightInit, const char level)
	:
	leftPaddlePosition(leftInit),
	rightPaddlePosition(rightInit),
	trackingLevel(level)
{
	ResetBall();
}

void GLEN::PongGame::ResetBall()
{
	ballPosition = {0.0f, 0.0f};
	// randomly initialize
	constexpr auto upperIntBound{ 100 };
	constexpr auto lowerIntBound{ -100 };
	constexpr auto boundsMedian{ (upperIntBound - lowerIntBound) / 2 };
	constexpr auto boundsRange{ upperIntBound - lowerIntBound };

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<> dist(lowerIntBound, upperIntBound);
	auto yInit{ (static_cast<float>(dist(rng) - boundsMedian)) / (boundsRange) };

	ballVelocity = std::array<float, 2>{.0075f, yInit / 60.0f};
}


void GLEN::PongGame::UpdateState(KeyState ks)
{
	// 1. Update Player Paddle
	// 2. Clamp within game bounds
	// 3. Update AI Paddle
	// 4. Clamp within game bounds
	// 5. Check for collisions with paddles or game bounds
	// 6. Update ball position



	// POS 0 = X POS
	// POS 1 = Y POS

	// update Paddle Position
	constexpr auto MAX_PADDLE_VELOCITY_PLAYER	{ 0.075f };
	constexpr auto PADDLE_VELOCITY_MOD_PLAYER	{ 0.0075f };

	constexpr auto MAX_PADDLE_VELOCITY_AI		{ 0.050f };
	constexpr auto PADDLE_VELOCITY_MOD_AI		{ 0.001f };

	constexpr auto PG_CLAMPBOUNDS = 3.0f;

	constexpr auto xOffsetBox{ 0.2f };
	constexpr auto yOffsetBox{ 1.0f };

	switch (ks)
	{
	case KeyState::W:
		leftPaddleVelocity += PADDLE_VELOCITY_MOD_PLAYER;
		if (leftPaddleVelocity >= MAX_PADDLE_VELOCITY_PLAYER)
		{
			leftPaddleVelocity = MAX_PADDLE_VELOCITY_PLAYER;
		}
		break;
	case KeyState::S:
		leftPaddleVelocity -= PADDLE_VELOCITY_MOD_PLAYER;
		if (leftPaddleVelocity <= MAX_PADDLE_VELOCITY_PLAYER)
		{
			leftPaddleVelocity = -MAX_PADDLE_VELOCITY_PLAYER;
		}
		break;
	default:
		leftPaddleVelocity = 0.0f;
	}
	leftPaddlePosition[1] += leftPaddleVelocity;


	// clamp paddle positions
	// TODO remove magic numbers and find true boundary conditions
	if ((leftPaddlePosition[1] > PG_CLAMPBOUNDS - xOffsetBox))
	{
		leftPaddlePosition[1] = PG_CLAMPBOUNDS - xOffsetBox;
	}
	else if (leftPaddlePosition[1] < -PG_CLAMPBOUNDS + xOffsetBox)
	{
		leftPaddlePosition[1] = -PG_CLAMPBOUNDS + xOffsetBox;
	}


	// TODO finish AI
	// calculate intercept of ball position with 
	// the axis that the paddle lies on
	// 
	// for now, only calculate the intercept without predicting bounces
	//
	// at higher AI levels, perhaps predict ahead

	if (ballVelocity[0] > 0) // if the ball is heading towards the AI paddle by checking its x axis
	{
		auto ballVx = ballVelocity[0];
		auto ballVy = ballVelocity[1];

		auto predictedXpos	{rightPaddlePosition[0]};
		auto xDiff			{predictedXpos - ballPosition[0]};
		
		auto predictedYpos	{ballPosition[1] +  ballVy * (xDiff / ballVx)};

		if ((predictedYpos > rightPaddlePosition[1]  - (xOffsetBox / 2.0f)) && (predictedYpos < rightPaddlePosition[1]  + (xOffsetBox / 2.0f)))
		{
			rightPaddleVelocity = 0.0f;
		}
		else if (predictedYpos > rightPaddlePosition[1])
		{
			rightPaddleVelocity += PADDLE_VELOCITY_MOD_AI;
			if (rightPaddleVelocity >= MAX_PADDLE_VELOCITY_AI)
			{
				rightPaddleVelocity = MAX_PADDLE_VELOCITY_AI;
			}
		}
		else if (predictedYpos < rightPaddlePosition[1])
		{
			rightPaddleVelocity -= PADDLE_VELOCITY_MOD_AI;
			if (rightPaddleVelocity <= MAX_PADDLE_VELOCITY_AI)
			{
				rightPaddleVelocity = -MAX_PADDLE_VELOCITY_AI;
			}
		}

		GLEN_CRITICAL("Velocity {}, Position {}, Target {}", rightPaddleVelocity, rightPaddlePosition[0], predictedYpos);
		rightPaddlePosition[1] += rightPaddleVelocity;


		// perform CLAMPING
		if ((rightPaddlePosition[1] > PG_CLAMPBOUNDS - xOffsetBox))
		{
			rightPaddlePosition[1] =  PG_CLAMPBOUNDS - xOffsetBox;
		}
		else if (rightPaddlePosition[1] < - PG_CLAMPBOUNDS + xOffsetBox)
		{
			rightPaddlePosition[1] = - PG_CLAMPBOUNDS + xOffsetBox;
		}
	}

	



	// detect ball-paddle collision
	if (ballPosition[0] < leftPaddlePosition[0] + xOffsetBox)
	{
		if ((ballPosition[1] < (leftPaddlePosition[1] + yOffsetBox)) && (ballPosition[1] > (leftPaddlePosition[1] - yOffsetBox)))
		{
			collisionsCount++;
			ballVelocity[0] *= -1.0f;
			if (std::abs(leftPaddleVelocity) > MAX_PADDLE_VELOCITY_PLAYER - .025f)
			{
				ballVelocity[1] += leftPaddleVelocity *.1f;
			}
		}	
		else
		{
			ResetBall();
		}
	}
	else if (ballPosition[0] > rightPaddlePosition[0] - xOffsetBox)
	{
		if ((ballPosition[1] < (rightPaddlePosition[1] + yOffsetBox)) && (ballPosition[1] > (rightPaddlePosition[1] - yOffsetBox)))
		{
			collisionsCount++;
			ballVelocity[0] *= -1.0f;
			if (std::abs(rightPaddleVelocity) > MAX_PADDLE_VELOCITY_PLAYER - .025f)
			{
				ballVelocity[1] += rightPaddleVelocity * .1f;
			}
		}
		else
		{
			ResetBall();
		}
	}
	else if ((ballPosition[1] > PG_CLAMPBOUNDS) || (ballPosition[1] < -PG_CLAMPBOUNDS))
	{
		ballVelocity[1] *= -1.0f;
	}

	constexpr uint64_t	collisionsToBoost {5};
	constexpr auto		velocityBoost {1.001f};

	// every 10 collisions BOOST the velocity
	if ((collisionsCount  % collisionsToBoost)  == 0 )
	{
		ballVelocity[0] *= velocityBoost;
		ballVelocity[1] *= velocityBoost;
	}

	// update ball position
	ballPosition[0] += ballVelocity[0];
	ballPosition[1] += ballVelocity[1];
}
