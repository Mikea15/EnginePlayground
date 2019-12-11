
#include "Game.h"

#include <algorithm>
#include <cmath>

#include "State.h"
#include "Resources/Resources.h"
#include "Scene/Scene.h"

Game::Game(State* state)
	: m_isRunning(true)
	, m_deltaTime(1.0f / 60.0f)
	, m_gameState(nullptr)
{
	LoadConfig();

	SetState(state);
	InitSystems();
}

Game::~Game()
{
	SaveConfig();
}

void Game::InitSystems()
{
	srand(static_cast<unsigned int>(time(NULL)));

	m_sdlHandler.Init();

	Resources::Init();
	m_renderer = new SimpleRenderer();
	m_renderer->Init();
	m_renderer->SetRenderSize(m_sdlHandler.GetWindowParams().Width, m_sdlHandler.GetWindowParams().Height);

	m_gameState->Init(this);
}

void Game::CleanupSystems()
{
	delete m_renderer;

	Scene::Clear();
	Resources::Clean();

	// state cleanup
	m_gameState->Cleanup();
	m_gameState = nullptr;
}


void Game::SetState(State* state)
{
	if (m_gameState != nullptr)
	{
		m_gameState->Cleanup();
		m_gameState = nullptr;
	}

	m_gameState = state;
}

int Game::Execute()
{
	m_currentTime = std::chrono::high_resolution_clock::now();
	float accumulator = 0.0f;

	while (m_isRunning)
	{
		auto now = std::chrono::high_resolution_clock::now();
		auto timeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(now - m_currentTime);
		float frameTime = static_cast<float>(timeSpan.count());

		m_time += frameTime;

		// cap on how many updates we can do.
		// eg. dt: 1/60 ( 60 ups ).
		// frametime: 0.25
		// max ups = frametime / m_deltaTime
		// max ups = 0.25 / 0.016 = 15.6ups
		// for 30 ups -> max frametime 0.25 ( 7.5 )
		// for 60 ups -> max frametime 0.083 ( 5 updates )
		if (frameTime > 0.25)
			frameTime = 0.25;

		m_currentTime = now;

		accumulator += frameTime;

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			// Handle Input
			m_sdlHandler.HandleEvents(&event);

			if (event.type == SDL_QUIT)
			{
				m_isRunning = false;
			}
			if (event.type == SDL_WINDOWEVENT
				&& event.window.event == SDL_WINDOWEVENT_CLOSE
				&& event.window.windowID == SDL_GetWindowID(m_sdlHandler.GetSDLWindow())
				)
			{
				m_isRunning = false;
			}
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
			{
				m_isRunning = false;
			}

			m_gameState->HandleInput(&event);
		}

		while (accumulator >= m_deltaTime)
		{
			// window Update
			m_sdlHandler.Update(m_deltaTime);

			// Update
			m_gameState->Update(m_deltaTime);

			accumulator -= m_deltaTime;
		}

		const float alpha = accumulator / m_deltaTime;

		// Render
		m_sdlHandler.BeginRender();

		m_gameState->Render(alpha);

		// ui
		// m_sdlHandler.BeginUIRender();
		// m_gameState->RenderUI();
		// m_sdlHandler.EndUIRender();

		m_sdlHandler.EndRender();
	}

	CleanupSystems();

	return 0;
}


void Game::LoadConfig()
{

}

void Game::SaveConfig()
{

}
