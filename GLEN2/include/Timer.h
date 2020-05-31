#pragma once

namespace GLEN
{
	class Timer
	{
	public:
		Timer() = default;
		inline float getDelta() const noexcept {return deltaTime;} 
		inline float getFPS() const noexcept {return fpsCounter;}
		void update(const bool trackFPS) noexcept;

	private:
		float deltaTime {0.0f};
		float lastTime  {0.0f};
		float lastWindow{0.0f};
		float fpsCounter{0.0f};
		unsigned int frameCount {0};
		
	};


}