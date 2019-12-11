#pragma once

union SDL_Event;

#include "Game.h"
#include "Renderer/SimpleRenderer.h"
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
		plane = new Plane(16, 16);
		sphere = new Sphere(64, 64);
		tSphere = new Sphere(256, 256);
		torus = new Torus(2.0f, 0.4f, 32, 32);
		cube = new Cube();

		// material setup
		Material* matPbr = renderer->CreateMaterial("debug light");
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
		m_camera.SetPerspective(glm::radians(70.0f),
			static_cast<float>(renderer->GetRenderWidth() / renderer->GetRenderHeight()),
			0.1f, 100.0f);
		m_camera.Position = glm::vec3(0.0f, 0.0f, 5.0f);

		// scene setup
		mainTorus = Scene::MakeSceneNode(plane, matPbr);
		secondTorus = Scene::MakeSceneNode(torus, matPbr);
		thirdTorus = Scene::MakeSceneNode(torus, matPbr);
		plasmaOrb = Scene::MakeSceneNode(tSphere, matPlasmaOrb);

		// mainTorus->AddChild(secondTorus);
		// secondTorus->AddChild(thirdTorus);

		mainTorus->SetScale(1.0f);
		mainTorus->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
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
		// Shader* postProcessing1 = Resources::LoadShader("postprocessing1", "shaders/screen_quad.vs", "shaders/custom_post_1.fs");
		// Shader* postProcessing2 = Resources::LoadShader("postprocessing2", "shaders/screen_quad.vs", "shaders/custom_post_2.fs");
		// Material* customPostProcessing1 = renderer->CreatePostProcessingMaterial(postProcessing1);
		// Material* customPostProcessing2 = renderer->CreatePostProcessingMaterial(postProcessing2);

		// mesh 
		// sponza = Resources::LoadMesh(renderer, "sponza", "meshes/sponza/sponza.obj");
		// sponza->SetPosition(glm::vec3(0.0, -1.0, 0.0));
		// sponza->SetScale(0.01f);
	};

	void HandleInput(SDL_Event* event) override {};
	void Update(float deltaTime) override 
	{
		m_camera.Update(deltaTime);
	};

	void Render(float alpha = 1.0f) override 
	{
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
	SimpleRenderer* renderer;
	FlyCamera m_camera = FlyCamera(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	
	Plane* plane;
	Sphere*	sphere;
	Sphere*	tSphere;
	Torus* torus;
	Cube* cube;

	SceneNode* sponza;
	SceneNode* mainTorus;
	SceneNode* secondTorus;
	SceneNode* thirdTorus;
	SceneNode* plasmaOrb;
};
