#pragma once
#include "OrthoCam.h"

#include "glm/gtc/matrix_transform.hpp"

using namespace GLEN;

OrthoCam::OrthoCam(const float left, const float right, const float bottom, const float top)
	:
	m_Position(0.0f, 0.0f, 0.0f),
	m_Rotation(0.0f),
	m_Proj(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
	m_View(1.0f)
{
	m_ViewProj = m_View * m_Proj;
}

OrthoCam::OrthoCam(const glm::vec4 orientation)
	:
	m_Position(0.0f, 0.0f, 0.0f),
	m_Rotation(0.0f),
	m_Proj(glm::ortho(orientation.x, orientation.y, orientation.z, orientation.w, -1.0f, 1.0f)),
	m_View(1.0f)
{
	m_ViewProj = m_Proj * m_View;
}

void GLEN::OrthoCam::SetProj(const float left, const float right, const float bottom, const float top)
{
	m_Proj = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
	m_ViewProj = m_Proj * m_View;
}

void GLEN::OrthoCam::SetProj(const glm::vec4 orientation)
{
	m_Proj = glm::ortho(orientation.x, orientation.y, orientation.z, orientation.w, -1.0f, 1.0f);
	m_ViewProj = m_View * m_Proj;
}

void GLEN::OrthoCam::RecalculateView()
{
	glm::mat4 transform{
		glm::translate(glm::mat4(1.0f), m_Position) *
		glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1))
	};

	m_View = glm::inverse(transform);
	m_ViewProj = m_Proj * m_View;
}