#if 0
#pragma once

#include "Renderer/Renderer.h"
#include "Renderer/RenderTarget.h"
#include "Renderer/PBRCapture.h"
#include "Camera/FlyCamera.h"
#include "Resources/Resources.h"
#include "Mesh/Mesh.h"
#include "Mesh/Quad.h"
#include "Mesh/Circle.h"
#include "Mesh/LineStrip.h"
#include "Mesh/Cube.h"
#include "Mesh/Plane.h"
#include "Mesh/Sphere.h"
#include "Mesh/Torus.h"
#include "Shading/Shader.h"
#include "Shading/Material.h"
#include "Shading/Texture.h"
#include "Shading/TextureCube.h"
#include "Scene/Scene.h"
#include "Scene/SceneNode.h"
#include "Scene/Skybox.h"

#include <GL/glew.h>

class GLFWwindow;

/* 

    Global entry-point for Cell's initialization. Starts all Cell's relevant initialization 
    calls, and makes sure Cell is ready to operate after calling its initialization function. 
    Note that all GL function pointers are initialized here as well (including any extensions) 
    w/ GLAD.

*/
Renderer* Init(GLFWwindow* window, GLADloadproc loadProcFunc);


// cleans up Cell of all resources that are no longer relevant.
void Clean();

/* 

    Initializes all render data required for processing a new frame. This is mostly relevant
    for rendering GUI items.

*/
void NewFrame();

/* 

    Renders (debugging) GUI (IMGUI) on top of Cell's main renderer, by default the renderer's 
    configuration UI is rendered as well, which can be enabled/disabled on demand.

*/
void RenderGUI();

// pass mouse input to Cell
void InputMouse(int button, int action);

// pass keyboard input to Cell
void InputKey(int key, int action);

// pass scroll input to Cell
void InputScroll(float scrollOffset);

#endif