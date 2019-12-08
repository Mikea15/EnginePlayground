# CMakeList.txt : CMake project for EnginePlayground, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(MODULE_NAME EnginePlayground)

# Add source to this project's executable.
add_library(
	${MODULE_NAME} SHARED

	Camera/Camera.cpp
	Camera/Camera.h
	Camera/CameraFrustum.cpp
	Camera/CameraFrustum.h
	Camera/FlyCamera.cpp
	Camera/FlyCamera.h

	# External/Imgui/imgui_demo.cpp
	External/Imgui/imgui_impl_opengl3.cpp
	External/Imgui/imgui_impl_sdl.cpp
	External/Imgui/imgui_stdlib.cpp
	External/Imgui/imgui_stdlib.h

	Lighting/DirectionalLight.h
	Lighting/PointLight.h

	Mesh/Circle.cpp 
	Mesh/Circle.h
	Mesh/Cube.cpp 
	Mesh/Cube.h
	Mesh/LineStrip.cpp 
	Mesh/LineStrip.h
	Mesh/Mesh.cpp 
	Mesh/Mesh.h
	Mesh/Plane.cpp 
	Mesh/Plane.h
	Mesh/Quad.cpp 
	Mesh/Quad.h
	Mesh/Sphere.cpp 
	Mesh/Sphere.h
	Mesh/Torus.cpp 
	Mesh/Torus.h

	Renderer/CommandBuffer.cpp
	Renderer/CommandBuffer.h
	Renderer/GLCache.cpp
	Renderer/GLCache.h
	Renderer/MaterialLibrary.cpp
	Renderer/MaterialLibrary.h
	Renderer/PBR.cpp
	Renderer/PBR.h
	Renderer/PBRCapture.cpp
	Renderer/PBRCapture.h
	Renderer/PostProcessor.cpp
	Renderer/PostProcessor.h
	Renderer/RenderCommand.h
	Renderer/Renderer.cpp
	Renderer/Renderer.h
	Renderer/RenderTarget.cpp
	Renderer/RenderTarget.h

	Resources/MeshLoader.cpp
	Resources/MeshLoader.h
	Resources/Resources.cpp
	Resources/Resources.h
	Resources/ShaderLoader.cpp
	Resources/ShaderLoader.h
	Resources/TextureLoader.cpp
	Resources/TextureLoader.h

	Scene/Scene.cpp
	Scene/Scene.h
	Scene/SceneNode.cpp
	Scene/SceneNode.h
	#Scene/Skybox.cpp
	#Scene/Skybox.h

	Shading/Material.cpp
	Shading/Material.h
	Shading/Shader.cpp
	Shading/Shader.h
	Shading/ShadingTypes.h
	Shading/Texture.cpp
	Shading/Texture.h
	Shading/TextureCube.cpp
	Shading/TextureCube.h

	Utils/Logger.h
	Utils/Utils.h

	EnginePlayground.cpp
	EnginePlayground.h
)

# TODO: Add tests and install targets if needed.
find_package(glm CONFIG REQUIRED)
target_link_libraries(${MODULE_NAME} glm)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(${MODULE_NAME} imgui::imgui)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(${MODULE_NAME} nlohmann_json nlohmann_json::nlohmann_json)

find_package(GLEW REQUIRED)
target_link_libraries(${MODULE_NAME} GLEW::GLEW)

find_package(assimp CONFIG REQUIRED)
target_link_libraries(${MODULE_NAME} assimp::assimp)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(${MODULE_NAME} SDL2::SDL2 SDL2::SDL2main)