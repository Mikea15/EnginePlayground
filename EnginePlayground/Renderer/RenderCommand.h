#pragma once

#include <glm/glm.hpp>


class Mesh;
class Material;

/*

  All render state required for submitting a render command.

*/
struct RenderCommand
{
	glm::mat4 Transform;
	glm::mat4 PrevTransform;
	Mesh* Mesh;
	Material* Material;
	glm::vec3 BoxMin;
	glm::vec3 BoxMax;
};


