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
	// command.Material = m_materialLibrary->debugLightMaterial;
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
			RenderCommand command;
			command.Mesh = currentNode->Mesh;
			command.Material = currentNode->Material;
			command.Transform = currentNode->GetTransform();
			command.PrevTransform = currentNode->GetPrevTransform();
			command.BoxMin = currentNode->BoxMin;
			command.BoxMax = currentNode->BoxMax;
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

	const glm::mat4 view = m_camera->View;
	const glm::mat4 projection = m_camera->Projection;
	const glm::vec3 cameraPosition = m_camera->Position;

	for (RenderCommand rc : m_renderCommands)
	{
		Shader* currentShader = rc.Material->GetShader();
		currentShader->Use();
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

	m_renderCommands.clear();
}
