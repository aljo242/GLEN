#pragma once

namespace GLEN{

class Timer
{
public:
	Timer() = default;
	inline double getDelta() const noexcept {return deltaTime;} 
	inline double getFPS() const noexcept {return fpsCounter;}
	void update(const bool trackFPS) noexcept;

private:
	unsigned long frameCount {0ul};
	double deltaTime {0.0};
	double lastTime  {0.0};
	double lastWindow{0.0};
	double fpsCounter{0.0};
	void logFPS() const noexcept;	
};

} // namespace GLEN