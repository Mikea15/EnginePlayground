#include "CameraFrustum.h"

#include "Camera.h"



void CameraFrustum::Update(Camera* camera)
{
	float tan = 2.0 * std::tan(camera->GetFov() * 0.5);
	float nearHeight = tan * camera->GetNearPlane();
	float nearWidth = nearHeight * camera->GetAspectRatio();
	float farHeight = tan * camera->GetFarPlane();
	float farWidth = farHeight * camera->GetAspectRatio();

	glm::vec3 nearCenter = camera->GetPosition() + camera->GetForward() * camera->GetNearPlane();
	glm::vec3 farCenter = camera->GetPosition() + camera->GetForward() * camera->GetFarPlane();

	glm::vec3 v;
	// left plane
	v = (nearCenter - camera->GetRight() * nearWidth * 0.5f) - camera->GetPosition();
	Left.SetNormalD(glm::cross(glm::normalize(v), camera->GetUp()), nearCenter - camera->GetRight() * nearWidth * 0.5f);
	// right plane
	v = (nearCenter + camera->GetRight() * nearWidth * 0.5f) - camera->GetPosition();
	m_right.SetNormalD(glm::cross(camera->GetUp(), glm::normalize(v)), nearCenter + camera->GetRight() * nearWidth * 0.5f);
	// top plane
	v = (nearCenter + camera->GetUp() * nearHeight * 0.5f) - camera->GetPosition();
	Top.SetNormalD(glm::cross(glm::normalize(v), camera->GetRight()), nearCenter + camera->GetUp() * nearHeight * 0.5f);
	// bottom plane
	v = (nearCenter - camera->GetUp() * nearHeight * 0.5f) - camera->GetPosition();
	Bottom.SetNormalD(glm::cross(camera->GetRight(), glm::normalize(v)), nearCenter - camera->GetUp() * nearHeight * 0.5f);
	// near plane
	Near.SetNormalD(camera->GetForward(), nearCenter);
	// far plane
	Far.SetNormalD(-camera->GetForward(), farCenter);
}

bool CameraFrustum::Intersect(glm::vec3 point)
{
	for (int i = 0; i < 6; ++i)
	{
		if (Planes[i].Distance(point) < 0)
		{
			return false;
		}
	}
	return true;
}

bool CameraFrustum::Intersect(glm::vec3 point, float radius)
{
	for (int i = 0; i < 6; ++i)
	{
		if (Planes[i].Distance(point) < -radius)
		{
			return false;
		}
	}
	return true;
}

bool CameraFrustum::Intersect(glm::vec3 boxMin, glm::vec3 boxMax)
{
	for (int i = 0; i < 6; ++i)
	{
		glm::vec3 positive = boxMin;
		if (Planes[i].Normal.x >= 0)
		{
			positive.x = boxMax.x;
		}
		if (Planes[i].Normal.y >= 0)
		{
			positive.y = boxMax.y;
		}
		if (Planes[i].Normal.z >= 0)
		{
			positive.z = boxMax.z;
		}
		if (Planes[i].Distance(positive) < 0)
		{
			return false;
		}
	}
	return true;
}

