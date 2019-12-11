#pragma once

#include "IMGUIHandler.h"
#include "WindowParams.h"

#include <iostream>

#include <vector>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>

class SDLHandler
{
public:
	SDLHandler();
	~SDLHandler();

	bool Init();

	void HandleEvents(SDL_Event* event);
	void Update(float deltaTime);

	void BeginRender();
	void EndRender();
	void BeginUIRender();
	void EndUIRender();

	void SetWindowParameters(const WindowParams& params, bool initialSetup = false);

	SDL_Window* GetSDLWindow() const { return m_window; }
	const SDL_GLContext& GetGLContext() const { return m_mainGLContext; }

	const char* GetGLSLVersion() const { return m_glslVersion; }

	const std::vector<SDL_DisplayMode>& GetDisplayModes() const { return m_displayModes; }

	std::string GetDisplayModeName(int index) const 
	{
		auto it = m_displayModeIndexToName.find(index);
		if (it != m_displayModeIndexToName.end()) 
		{ 
			return it->second; 
		}
		return std::string();
	}

	const SDL_DisplayMode* GetCurrentDisplayMode();

	const WindowParams& GetWindowParams() const { return m_windowParams; }

	void RenderUI();

	static void DisplayGLErrors() {
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			std::cerr << "OpenGL error: " << err << std::endl;
		}
	}

private:
	void LoadWindowParams();
	void SaveWindowParams();
	void FindDisplayModes();

private:
	WindowParams m_windowParams;
	WindowParams m_tempWindowParams;

	SDL_Window* m_window;
	SDL_GLContext m_mainGLContext;

	IMGUIHandler m_uiHandler;

	const char* m_glslVersion = "#version 130";

	std::vector<SDL_DisplayMode> m_displayModes;
	std::unordered_map<int, std::string> m_displayModeIndexToName;

	bool m_lockMouseCursor = false;

	// statics
	static std::string s_configFileName;
};
