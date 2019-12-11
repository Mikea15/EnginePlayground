#pragma once

union SDL_Event;

#include "Game.h"
#include "Shading/Material.h"
#include "Resources/Resources.h"
#include "Scene/Scene.h"
#include "Scene/SceneNode.h"
#include "Scene/Skybox.h"
#include "Mesh/Plane.h"
#include "Mesh/Sphere.h"
#include "Mesh/Torus.h"
#include "Mesh/Cube.h"

class State
{
public:
	virtual ~State() = default;

	virtual void Init(Game* game) = 0;
	virtual void HandleInput(SDL_Event* event) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(float alpha = 1.0f) = 0;

	virtual void RenderUI() {};
	virtual void Cleanup() = 0;
};

class StubState
	: public State
{
public:
	StubState()
		: State()
	{}

	~StubState() override {};

	void Init(Game* game) override 
	{
		renderer = game->GetRenderer();
		renderer->SetCamera(&m_camera);

		// basic shapes
		Plane plane(16, 16);
		Sphere sphere(64, 64);
		Sphere tSphere(256, 256);
		Torus torus(2.0f, 0.4f, 32, 32);
		Cube cube;

		// material setup
		Material* matPbr = renderer->CreateMaterial();
		Shader* plasmaOrbShader = Resources::LoadShader("plasma orb", "shaders/custom/plasma_orb.vs", "shaders/custom/plasma_orb.fs");
		Material* matPlasmaOrb = renderer->CreateCustomMaterial(plasmaOrbShader);
		matPlasmaOrb->Cull = false;
		matPlasmaOrb->Blend = true;
		matPlasmaOrb->BlendSrc = GL_ONE;
		matPlasmaOrb->BlendDst = GL_ONE;
		matPlasmaOrb->SetTexture("TexPerllin", Resources::LoadTexture("perlin noise", "textures/perlin.png"), 0);
		matPlasmaOrb->SetFloat("Strength", 1.5f);
		matPlasmaOrb->SetFloat("Speed", 0.083f);

		// configure camera
		m_camera.SetPerspective(glm::radians(60.0f),
			renderer->GetRenderSize().x / renderer->GetRenderSize().y,
			0.1f, 100.0f);

		// scene setup
		mainTorus = Scene::MakeSceneNode(&torus, matPbr);
		secondTorus = Scene::MakeSceneNode(&torus, matPbr);
		thirdTorus = Scene::MakeSceneNode(&torus, matPbr);
		plasmaOrb = Scene::MakeSceneNode(&tSphere, matPlasmaOrb);

		mainTorus->AddChild(secondTorus);
		secondTorus->AddChild(thirdTorus);

		mainTorus->SetScale(1.0f);
		mainTorus->SetPosition(glm::vec3(-4.4f, 3.46f, -0.3));
		secondTorus->SetScale(0.65f);
		secondTorus->SetRotation(glm::vec4(0.0, 1.0, 0.0, glm::radians(90.0)));
		thirdTorus->SetScale(0.65f);

		plasmaOrb->SetPosition(glm::vec3(-4.0f, 4.0f, 0.25f));
		plasmaOrb->SetScale(0.6f);

		//// - background
		//Skybox* background = new Skybox;
		//PBRCapture* pbrEnv = rendererPtr->GetSkypCature();
		//background->SetCubemap(pbrEnv->Prefiltered);
		//float lodLevel = 1.5f;
		//background->Material->SetFloat("lodLevel", lodLevel);

		// post processing
		Shader* postProcessing1 = Resources::LoadShader("postprocessing1", "shaders/screen_quad.vs", "shaders/custom_post_1.fs");
		Shader* postProcessing2 = Resources::LoadShader("postprocessing2", "shaders/screen_quad.vs", "shaders/custom_post_2.fs");
		Material* customPostProcessing1 = renderer->CreatePostProcessingMaterial(postProcessing1);
		Material* customPostProcessing2 = renderer->CreatePostProcessingMaterial(postProcessing2);

		// mesh 
		// sponza = Resources::LoadMesh(renderer, "sponza", "meshes/sponza/sponza.obj");
		// sponza->SetPosition(glm::vec3(0.0, -1.0, 0.0));
		// sponza->SetScale(0.01f);

		// lighting
		DirectionalLight dirLight;
		dirLight.Direction = glm::vec3(0.2f, -1.0f, 0.25f);
		dirLight.Color = glm::vec3(1.0f, 0.89f, 0.7f);
		dirLight.Intensity = 50.0f;
		renderer->AddLight(&dirLight);

		std::vector<PointLight> torchLights;
		{
			PointLight torch;
			torch.Radius = 2.5;
			torch.Color = glm::vec3(1.0f, 0.3f, 0.05f);
			torch.Intensity = 50.0f;
			torch.RenderMesh = true;

			torch.Position = glm::vec3(4.85f, 0.7f, 1.43f);
			torchLights.push_back(torch);
			torch.Position = glm::vec3(4.85f, 0.7f, -2.2f);
			torchLights.push_back(torch);
			torch.Position = glm::vec3(-6.19f, 0.7f, 1.43f);
			torchLights.push_back(torch);
			torch.Position = glm::vec3(-6.19f, 0.7f, -2.2f);
			torchLights.push_back(torch);
			renderer->AddLight(&torchLights[0]);
			renderer->AddLight(&torchLights[1]);
			renderer->AddLight(&torchLights[2]);
			renderer->AddLight(&torchLights[3]);
		}

		std::vector<PointLight> randomLights;
		std::vector<glm::vec3> randomLightStartPositions;
		PointLight light;
		light.Radius = 1.0 + 3.0;
		light.Intensity = 10.0 + 1000.0;
		light.Color = glm::vec3(1, 1, 1);
		light.RenderMesh = true;
		randomLights.push_back(light);
		randomLightStartPositions.push_back(glm::vec3(12.0f, 5.0f, 6.0f));
	};

	void HandleInput(SDL_Event* event) override {};
	void Update(float deltaTime) override 
	{
		m_camera.Update(deltaTime);
	};

	void Render(float alpha = 1.0f) override 
	{
		glClearColor(0.5, 0.2, 0.5, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		{
			//CLOCK(PUSH);
			renderer->PushRender(mainTorus);
			// renderer->PushRender(sponza);
			// renderer->PushRender(plasmaOrb);
			// renderer->PushRender(background);
		}
		{
			//CLOCK(RENDER);
			// request Cell to render all currently pushed commands
			renderer->RenderPushedCommands();
		}
	};

	void RenderUI() {};
	void Cleanup() override {};

private:
	Renderer* renderer;
	FlyCamera m_camera = FlyCamera(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	
	SceneNode* sponza;
	SceneNode* mainTorus;
	SceneNode* secondTorus;
	SceneNode* thirdTorus;
	SceneNode* plasmaOrb;
};
