#pragma once

#include <vector>
#include <string>

#include "RenderCommand.h"

class SceneNode;
class Camera;
class MaterialLibrary;
class Shader;

class SimpleRenderer
{
public:
	SimpleRenderer() = default;
	~SimpleRenderer();

	void Init();
	void SetCamera(Camera* camera);
	void SetRenderSize(int width, int height);

	// create either a deferred default material (based on default set of materials available (like glass)), or a custom material (with custom you have to supply your own shader)
	Material* CreateMaterial(std::string base = "default"); // these don't have the custom flag set (default material has default state and uses checkerboard texture as albedo (and black metallic, half roughness, purple normal, white ao)
	Material* CreateCustomMaterial(Shader* shader);         // these have the custom flag set (will be rendered in forward pass)
	Material* CreatePostProcessingMaterial(Shader* shader); // these have the post-processing flag set (will be rendered after deferred/forward pass)

	void PushRender(Mesh* mesh, Material* material, glm::mat4 transform = glm::mat4(1.0f), glm::mat4 prevTransform = glm::mat4(1.0f));
	void PushRender(SceneNode* node);

	void RenderPushedCommands();

	int GetRenderWidth() const { return m_renderTargetWidth; }
	int GetRenderHeight() const { return m_renderTargetHeight; }

private:
	std::vector<RenderCommand> m_renderCommands;

	int m_renderTargetHeight;
	int m_renderTargetWidth;

	Camera* m_camera;

	MaterialLibrary* m_materialLibrary;
};

