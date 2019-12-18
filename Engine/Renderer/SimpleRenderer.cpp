#include "SimpleRenderer.h"

#include <GL/glew.h>

#include "Camera/Camera.h"

#include "Mesh/Mesh.h"
#include "Scene/SceneNode.h"
#include "Shading/Shader.h"
#include "Shading/Material.h"
#include "MaterialLibrary.h"

#include "Utils/Logger.h"

#include <stack>
#include <algorithm>

#define ENABLE_GLSTATE_CACHE 1

SimpleRenderer::~SimpleRenderer()
{
	delete m_materialLibrary;
}

void SimpleRenderer::Init()
{
	m_materialLibrary = new MaterialLibrary();
}

void SimpleRenderer::SetCamera(Camera* camera)
{
	m_camera = camera;
}

void SimpleRenderer::SetRenderSize(int width, int height)
{
	m_renderTargetWidth = width;
	m_renderTargetHeight = height;
}

Material* SimpleRenderer::CreateMaterial(std::string base)
{
	return m_materialLibrary->CreateMaterial(base);
}

Material* SimpleRenderer::CreateCustomMaterial(Shader* shader)
{
	return m_materialLibrary->CreateCustomMaterial(shader);
}

Material* SimpleRenderer::CreatePostProcessingMaterial(Shader* shader)
{
	return m_materialLibrary->CreatePostProcessingMaterial(shader);
}

void SimpleRenderer::PushRender(Mesh* mesh, Material* material, glm::mat4 transform, glm::mat4 prevTransform)
{
	RenderCommand command;
	command.Mesh = mesh;
	command.Material = material;
	command.Transform = transform;
	command.PrevTransform = prevTransform;
	command.BoxMin = glm::vec3(-10000.0f);
	command.BoxMax = glm::vec3(10000.0f);

	m_renderCommands.push_back(command);
}

void SimpleRenderer::PushRender(SceneNode* node)
{
	node->UpdateTransform(true);

	std::stack<SceneNode*> nodeStack;
	nodeStack.push(node);
	while (!nodeStack.empty())
	{
		SceneNode* currentNode = nodeStack.top();
		nodeStack.pop();

		if (currentNode->Mesh != nullptr) 
		{
			const glm::vec3 boxMinWorld = node->GetWorldPosition() + (node->GetWorldScale() * node->BoxMin);
			const glm::vec3 boxMaxWorld = node->GetWorldPosition() + (node->GetWorldScale() * node->BoxMax);

			RenderCommand command;
			command.Mesh = currentNode->Mesh;
			command.Material = currentNode->Material;
			command.Transform = currentNode->GetTransform();
			command.PrevTransform = currentNode->GetPrevTransform();
			command.BoxMin = boxMinWorld;
			command.BoxMax = boxMaxWorld;
			m_renderCommands.push_back(command);
		}

		for (SceneNode* childNode : currentNode->GetChildren())
		{
			nodeStack.push(childNode);
		}
	}
}

void SimpleRenderer::RenderPushedCommands()
{
	glClearColor(0.7f, 0.3f, 1.0f, 1.0f);
	glViewport(0, 0, m_renderTargetWidth, m_renderTargetHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const glm::mat4 view = m_camera->GetView();
	const glm::mat4 projection = m_camera->GetProjection();
	const glm::vec3 cameraPosition = m_camera->GetPosition();

	std::vector<RenderCommand> transparents;
	std::vector<RenderCommand> solids;
	for (auto it = m_renderCommands.rbegin(), end = m_renderCommands.rend(); it != end; ++it)
	{
		if (it->Material->Blend)
		{
			transparents.push_back(*it);
		}
		else
		{
			solids.push_back(*it);
		}
	}

	m_renderCommands.clear();

	for (RenderCommand rc : solids)
	{
		// Frustum Culling.
		if (m_enableFrustumCulling && !m_camera->GetFrustum().Intersect(rc.BoxMin, rc.BoxMax)) {
			continue;
		}

		Shader* currentShader = rc.Material->GetShader();

		if (m_enableGLCache) 
		{
			m_glState.SetBlend(rc.Material->Blend);
			if (rc.Material->Blend)
			{
				m_glState.SetBlendFunc(rc.Material->BlendSrc, rc.Material->BlendDst);
			}
			m_glState.SetDepthFunc(rc.Material->DepthCompare);
			m_glState.SetDepthTest(rc.Material->DepthTest);
			m_glState.SetCull(rc.Material->Cull);
			m_glState.SetCullFace(rc.Material->CullFace);

			m_glState.SwitchShader(currentShader->ID);
		}
		else 
		{
			// Blend
			if (rc.Material->Blend) 
			{ 
				glEnable(GL_BLEND);
				glBlendFunc(rc.Material->BlendSrc, rc.Material->BlendDst);
			}
			else 
			{ 
				glDisable(GL_BLEND); 
			}

			glEnable(rc.Material->Cull);
			glDepthFunc(rc.Material->DepthCompare);
			if (rc.Material->DepthTest)
			{
				glEnable(GL_DEPTH_TEST);
			}
			else
			{
				glDisable(GL_DEPTH_TEST);
			}

			if (rc.Material->Cull)
			{
				glEnable(GL_CULL_FACE);
			}
			else
			{
				glDisable(GL_CULL_FACE);
			}
			glCullFace(rc.Material->CullFace);
			currentShader->Use();
		}

		currentShader->SetMatrix("view", view);
		currentShader->SetMatrix("projection", projection);
		currentShader->SetVector("CamPos", cameraPosition);

		currentShader->SetMatrix("model", rc.Transform);
		currentShader->SetMatrix("prevModel", rc.PrevTransform);

		std::map<std::string, UniformValueSampler>* samplers = rc.Material->GetSamplerUniforms();
		for (auto it = samplers->begin(), end = samplers->end(); it != end; ++it)
		{
			if (it->second.Type == SHADER_TYPE_SAMPLERCUBE) 
			{
				it->second.TextureCube->Bind(it->second.Unit);
			}
			else
			{
				it->second.Texture->Bind(it->second.Unit);
			}
		}

		std::map<std::string, UniformValue>* uniforms = rc.Material->GetUniforms();
		for (auto it = uniforms->begin(), end = uniforms->end(); it != end; ++it)
		{
			switch (it->second.Type)
			{
			case SHADER_TYPE_BOOL:
				currentShader->SetBool(it->first, it->second.Bool);
				break;
			case SHADER_TYPE_INT:
				currentShader->SetInt(it->first, it->second.Int);
				break;
			case SHADER_TYPE_FLOAT:
				currentShader->SetFloat(it->first, it->second.Float);
				break;
			case SHADER_TYPE_VEC2:
				currentShader->SetVector(it->first, it->second.Vec2);
				break;
			case SHADER_TYPE_VEC3:
				currentShader->SetVector(it->first, it->second.Vec3);
				break;
			case SHADER_TYPE_VEC4:
				currentShader->SetVector(it->first, it->second.Vec4);
				break;
			case SHADER_TYPE_MAT2:
				currentShader->SetMatrix(it->first, it->second.Mat2);
				break;
			case SHADER_TYPE_MAT3:
				currentShader->SetMatrix(it->first, it->second.Mat3);
				break;
			case SHADER_TYPE_MAT4:
				currentShader->SetMatrix(it->first, it->second.Mat4);
				break;
			default:
				LOG_ERROR("Unrecognized Uniform type set.");
				break;
			}
		}

		// Render Mesh
		glBindVertexArray(rc.Mesh->m_VAO);

		const GLenum mode = rc.Mesh->Topology == TRIANGLE_STRIP ? GL_TRIANGLE_STRIP : GL_TRIANGLES;
		if (!rc.Mesh->Indices.empty()) 
		{
			glDrawElements(mode, rc.Mesh->Indices.size(), GL_UNSIGNED_INT, 0);
		}
		else
		{
			glDrawArrays(mode, 0, rc.Mesh->Positions.size());
		}
	}

	solids.clear();


	// Try front to back render for transparents.
	std::sort(transparents.begin(), transparents.end(), [](RenderCommand lhs, RenderCommand rhs) {
		return lhs.Transform[3].z > rhs.Transform[3].z;
		});

	for (RenderCommand rc : transparents)
	{
		// Frustum Culling.
		if (m_enableFrustumCulling && !m_camera->GetFrustum().Intersect(rc.BoxMin, rc.BoxMax)) {
			continue;
		}

		Shader* currentShader = rc.Material->GetShader();

		if (m_enableGLCache)
		{
			m_glState.SetBlend(rc.Material->Blend);
			if (rc.Material->Blend)
			{
				m_glState.SetBlendFunc(rc.Material->BlendSrc, rc.Material->BlendDst);
			}
			m_glState.SetDepthFunc(rc.Material->DepthCompare);
			m_glState.SetDepthTest(rc.Material->DepthTest);
			m_glState.SetCull(rc.Material->Cull);
			m_glState.SetCullFace(rc.Material->CullFace);

			m_glState.SwitchShader(currentShader->ID);
		}
		else
		{
			// Blend
			if (rc.Material->Blend)
			{
				glEnable(GL_BLEND);
				glBlendFunc(rc.Material->BlendSrc, rc.Material->BlendDst);
			}
			else
			{
				glDisable(GL_BLEND);
			}

			glEnable(rc.Material->Cull);
			glDepthFunc(rc.Material->DepthCompare);
			if (rc.Material->DepthTest)
			{
				glEnable(GL_DEPTH_TEST);
			}
			else
			{
				glDisable(GL_DEPTH_TEST);
			}

			if (rc.Material->Cull)
			{
				glEnable(GL_CULL_FACE);
			}
			else
			{
				glDisable(GL_CULL_FACE);
			}
			glCullFace(rc.Material->CullFace);
			currentShader->Use();
		}

		currentShader->SetMatrix("view", view);
		currentShader->SetMatrix("projection", projection);
		currentShader->SetVector("CamPos", cameraPosition);

		currentShader->SetMatrix("model", rc.Transform);
		currentShader->SetMatrix("prevModel", rc.PrevTransform);

		std::map<std::string, UniformValueSampler>* samplers = rc.Material->GetSamplerUniforms();
		for (auto it = samplers->begin(), end = samplers->end(); it != end; ++it)
		{
			if (it->second.Type == SHADER_TYPE_SAMPLERCUBE)
			{
				it->second.TextureCube->Bind(it->second.Unit);
			}
			else
			{
				it->second.Texture->Bind(it->second.Unit);
			}
		}

		std::map<std::string, UniformValue>* uniforms = rc.Material->GetUniforms();
		for (auto it = uniforms->begin(), end = uniforms->end(); it != end; ++it)
		{
			switch (it->second.Type)
			{
			case SHADER_TYPE_BOOL:
				currentShader->SetBool(it->first, it->second.Bool);
				break;
			case SHADER_TYPE_INT:
				currentShader->SetInt(it->first, it->second.Int);
				break;
			case SHADER_TYPE_FLOAT:
				currentShader->SetFloat(it->first, it->second.Float);
				break;
			case SHADER_TYPE_VEC2:
				currentShader->SetVector(it->first, it->second.Vec2);
				break;
			case SHADER_TYPE_VEC3:
				currentShader->SetVector(it->first, it->second.Vec3);
				break;
			case SHADER_TYPE_VEC4:
				currentShader->SetVector(it->first, it->second.Vec4);
				break;
			case SHADER_TYPE_MAT2:
				currentShader->SetMatrix(it->first, it->second.Mat2);
				break;
			case SHADER_TYPE_MAT3:
				currentShader->SetMatrix(it->first, it->second.Mat3);
				break;
			case SHADER_TYPE_MAT4:
				currentShader->SetMatrix(it->first, it->second.Mat4);
				break;
			default:
				LOG_ERROR("Unrecognized Uniform type set.");
				break;
			}
		}

		// Render Mesh
		glBindVertexArray(rc.Mesh->m_VAO);

		const GLenum mode = rc.Mesh->Topology == TRIANGLE_STRIP ? GL_TRIANGLE_STRIP : GL_TRIANGLES;
		if (!rc.Mesh->Indices.empty())
		{
			glDrawElements(mode, rc.Mesh->Indices.size(), GL_UNSIGNED_INT, 0);
		}
		else
		{
			glDrawArrays(mode, 0, rc.Mesh->Positions.size());
		}
	}

	transparents.clear();
}

void SimpleRenderer::RenderUIMenu()
{
	if (ImGui::BeginMenu("Simple Renderer"))
	{
		ImGui::Checkbox("Enable Frustum Culling", &m_enableFrustumCulling);
		ImGui::Checkbox("Enable GL Cache", &m_enableGLCache);
		ImGui::EndMenu();
	}
}
