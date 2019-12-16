#pragma once

#include <glm/glm.hpp>

class RenderTarget;

/*
  Light container object for any 3D directional light source. Directional light types support
  shadow casting, holding a reference to the RenderTarget (and the relevant light space view
  projection matrix) used for its shadow map generation.
*/
class DirectionalLight
{
public:
	glm::vec3 Direction = glm::vec3(0.0f);
	glm::vec3 Color = glm::vec3(1.0f);
	float Intensity = 1.0f;

	bool CastShadows = true;
	RenderTarget* ShadowMapRT;
	glm::mat4 LightSpaceViewProjection;
};


