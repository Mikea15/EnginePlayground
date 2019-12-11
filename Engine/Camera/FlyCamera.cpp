#include "FlyCamera.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/compatibility.hpp>


FlyCamera::FlyCamera(glm::vec3 position, glm::vec3 forward, glm::vec3 up) : Camera(position, forward, up)
{
	Yaw = -90.0f;

	Forward = forward;
	m_WorldUp = Up;
	m_TargetPosition = position;
}

void FlyCamera::Update(float dt)
{
	Camera::Update(dt);
	// slowly interpolate to target position each frame given some damping factor.
	// this gives smooth camera movement that fades out the closer we are to our target.
	Position = glm::lerp(Position, m_TargetPosition, glm::clamp(dt * Damping, 0.0f, 1.0f));
	Yaw = glm::lerp(Yaw, m_TargetYaw, glm::clamp(dt * Damping * 2.0f, 0.0f, 1.0f));
	Pitch = glm::lerp(Pitch, m_TargetPitch, glm::clamp(dt * Damping * 2.0f, 0.0f, 1.0f));

	// calculate new Cartesian basis vectors from yaw/pitch pair:
	glm::vec3 newForward;
	newForward.x = cos(0.0174533 * Pitch) * cos(0.0174533 * Yaw);
	newForward.y = sin(0.0174533 * Pitch);
	newForward.z = cos(0.0174533 * Pitch) * sin(0.0174533 * Yaw);
	Forward = glm::normalize(newForward);
	Right = glm::normalize(glm::cross(Forward, m_WorldUp));
	Up = glm::cross(Right, Forward);

	// calculate the new view matrix
	UpdateView();
}

void FlyCamera::InputKey(float dt, CAMERA_MOVEMENT direction)
{
	float speed = MovementSpeed * dt;
	if (direction == CAMERA_FORWARD)
		m_TargetPosition = m_TargetPosition + Forward * speed;
	else if (direction == CAMERA_BACK)
		m_TargetPosition = m_TargetPosition - Forward * speed;
	else if (direction == CAMERA_LEFT)
		m_TargetPosition = m_TargetPosition - Right * speed;
	else if (direction == CAMERA_RIGHT)
		m_TargetPosition = m_TargetPosition + Right * speed;
	else if (direction == CAMERA_UP)
		m_TargetPosition = m_TargetPosition + m_WorldUp * speed;
	else if (direction == CAMERA_DOWN)
		m_TargetPosition = m_TargetPosition - m_WorldUp * speed;
}

void FlyCamera::InputMouse(float deltaX, float deltaY)
{
	float xmovement = deltaX * MouseSensitivty;
	float ymovement = deltaY * MouseSensitivty;

	m_TargetYaw += xmovement;
	m_TargetPitch += ymovement;

	// prevents calculating the length of the null vector
	if (m_TargetYaw == 0.0f) m_TargetYaw = 0.01f;
	if (m_TargetPitch == 0.0f) m_TargetPitch = 0.01f;

	// it's not allowed to move the pitch above or below 90 degrees asctime the current 
	// world-up vector would break our LookAt calculation.
	if (m_TargetPitch > 89.0f)  m_TargetPitch = 89.0f;
	if (m_TargetPitch < -89.0f) m_TargetPitch = -89.0f;
}

void FlyCamera::InputScroll(float deltaX, float deltaY)
{
	MovementSpeed = glm::clamp(MovementSpeed + deltaY * 1.0f, 1.0f, 25.0f);
	Damping = glm::clamp(Damping + deltaX * 0.5f, 1.0f, 25.0f);
}
