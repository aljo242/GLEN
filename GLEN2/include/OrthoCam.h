#pragma once

#include <glm/glm.hpp>

namespace GLEN {

class OrthoCam
{
public:
	OrthoCam(const float left, const float right, const float bottom, const float top);
	OrthoCam(const glm::vec4 orientation);

	void SetProj(const float left, const float right, const float bottom, const float top);
	void SetProj(const glm::vec4 orientation);

	glm::vec3 GetPosition()			const { return m_Position; }
	float GetRotation()				const { return m_Rotation; }
	glm::mat4 GetProjection()		const { return m_Proj; }
	glm::mat4 GetView()				const { return m_View; }
	glm::mat4 GetViewProj()			const { return m_ViewProj; }

private:
	void RecalculateView();

	glm::mat4 m_Proj;
	glm::mat4 m_View;
	glm::mat4 m_ViewProj;
 
	glm::vec3 m_Position;
	float m_Rotation;

	const glm::vec3 cameraUp {glm::vec3(0.0f, 1.0f, 0.0f) };
	const glm::vec3 cameraRight{glm::vec3(1.0f, 0.0f, 0.0f)};
	const glm::vec3 cameraFront {glm::vec3(0.0f, 0.0f, -1.0f)};
};

} // namespace GLEN