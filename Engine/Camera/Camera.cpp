#include "Camera.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/compatibility.hpp>

//must be less than 90 to avoid gimbal lock
const float Camera::s_maxPitchAngle = 85.0f;
const float Camera::s_minFov = 0.1f;
const float Camera::s_maxFov = 179.0f;

Camera::Camera()
	: m_view(1.0f)
	, m_projection(1.0f)
	, m_position(0.0f, 0.0f, 0.0f)
	, m_forward(0.0f, 0.0f, -1.0f)
	, m_up(0.0f, 1.0f, 0.0f)
	, m_right(1.0f, 0.0f, 0.0f)
{
	UpdateView();
}

Camera::Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 up) 
	: m_view(1.0f)
	, m_projection(1.0f)
	, m_position(position)
	, m_forward(forward)
	, m_up(up)
	, m_right(1.0f, 0.0f, 0.0f)
{
	UpdateView();
}

void Camera::Update(float deltaTime)
{
	m_frustum.Update(this);
}

void Camera::SetPerspective(float fov, float aspect, float near, float far)
{
	m_properties.m_isPerspective = true;
	m_properties.m_fov = fov;
	m_properties.m_aspectRatio = aspect;
	m_properties.m_nearPlane = near;
	m_properties.m_farPlane = far;

	m_projection = glm::perspective(fov, aspect, near, far);
}

void Camera::SetOrthographic(float left, float right, float top, float bottom, float near, float far)
{
	m_properties.m_isPerspective = false;
	m_properties.m_nearPlane = near;
	m_properties.m_farPlane = far;
	m_projection = glm::ortho(left, right, bottom, top, near, far);
}

void Camera::UpdateView()
{
	m_view = glm::lookAt(m_position, m_position + m_forward, m_up);
}

float Camera::FrustumHeightAtDistance(float distance)
{
	if (m_properties.m_isPerspective)
	{
		return 2.0f * distance * tanf(glm::radians(m_properties.m_fov * 0.5));
	}
	return m_frustum.Top.D;
}

float Camera::DistanceAtFrustumHeight(float frustumHeight)
{
	if (m_properties.m_isPerspective)
	{
		return frustumHeight * 0.5f / tanf(glm::radians(m_properties.m_fov * 0.5f));
	}
	return m_frustum.Near.D;
}

void Camera::SetProperties(const Properties& properties)
{
	if (m_properties != properties)
	{
		m_properties = properties;
	}
}

void Camera::SetFov(float fov)
{
	if (fov < s_minFov)
	{
		fov = s_minFov;
	}
	else if (fov > s_maxFov)
	{
		fov = s_maxFov;
	}
	m_properties.m_fov = fov;
}

void Camera::SetNearFarPlane(float nearPlane, float farPlane)
{
	m_properties.m_nearPlane = nearPlane;
	m_properties.m_farPlane = farPlane;
}

void Camera::SetAspectRatio(float ratio)
{
	m_properties.m_aspectRatio = ratio;
}

