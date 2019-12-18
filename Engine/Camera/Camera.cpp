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
	, m_isDirty(true)
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
	, m_isDirty(true)
{
	UpdateView();
}

void Camera::Update(float deltaTime)
{
	m_frustum.Update(this);
	UpdateView();
}

void Camera::SetPerspective(float fov, float aspect, float near, float far)
{
	m_properties.m_isPerspective = true;
	m_properties.m_fov = fov;
	m_properties.m_aspectRatio = aspect;
	m_properties.m_nearPlane = near;
	m_properties.m_farPlane = far;

	UpdateProjection();
}

void Camera::SetOrthographic(float left, float right, float top, float bottom, float near, float far)
{
	m_properties.m_isPerspective = false;

	m_properties.m_orthoLeft = left;
	m_properties.m_orthoRight = right;
	m_properties.m_orthoBottom = bottom;
	m_properties.m_orthoTop = top;
	m_properties.m_nearPlane = near;
	m_properties.m_farPlane = far;

	UpdateProjection();
}

void Camera::UpdateView()
{
	if (m_isDirty)
	{
		m_view = glm::lookAt(m_position, m_position + m_forward, m_up);
		m_isDirty = false;
	}
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

void Camera::SetPosition(const glm::vec3& position)
{
	m_position = position;
	m_isDirty = true;
}

void Camera::SetProperties(const Properties& properties)
{
	if (m_properties != properties)
	{
		m_properties = properties;
		UpdateProjection();
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
	UpdateProjection();
}

void Camera::SetNearFarPlane(float nearPlane, float farPlane)
{
	m_properties.m_nearPlane = nearPlane;
	m_properties.m_farPlane = farPlane;
	UpdateProjection();
}

void Camera::SetAspectRatio(float ratio)
{
	m_properties.m_aspectRatio = ratio;
	UpdateProjection();
}

void Camera::UpdateProjection()
{
	if (m_properties.m_isPerspective)
	{
		m_projection = glm::perspective(
			m_properties.m_fov,
			m_properties.m_aspectRatio,
			m_properties.m_nearPlane,
			m_properties.m_farPlane);
	}
	else
	{
		m_projection = glm::ortho(
			m_properties.m_orthoLeft,
			m_properties.m_orthoRight,
			m_properties.m_orthoBottom,
			m_properties.m_orthoTop,
			m_properties.m_nearPlane,
			m_properties.m_farPlane);
	}
}

