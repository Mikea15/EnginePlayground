#pragma once

#include <glm/glm.hpp>

/*

  Light container object for any 3D point light source. Point lights range are solely
  determined by a radius value which is used for their frustum culling and attenuation
  properties. Attenuation is calculated based on a slightly tweaked point light attenuation
  equation derived by Epic Games (for use in UE4).

*/
class PointLight
{
public:
	glm::vec3 Position = glm::vec3(0.0f);
	glm::vec3 Color = glm::vec3(1.0f);
	float      Intensity = 1.0f;
	float      Radius = 1.0f;
	bool       Visible = true;
	bool       RenderMesh = false;
};

