# CMakeList.txt : CMake project for EnginePlayground, include source and define
# project specific logic here.
#
set(MODULE_NAME Engine)

# Add source to this project's executable.
add_library(
	${MODULE_NAME} STATIC

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
	Renderer/DebugDraw.cpp
	Renderer/DebugDraw.h
	Renderer/GLStateCache.cpp
	Renderer/GLStateCache.h
	Renderer/IRenderer.h
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
	Renderer/SimpleRenderer.cpp
	Renderer/SimpleRenderer.h
	Renderer/ViewportGrid.cpp
	Renderer/ViewportGrid.h

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
	Scene/Skybox.cpp
	Scene/Skybox.h

	Shading/Material.cpp
	Shading/Material.h
	Shading/Shader.cpp
	Shading/Shader.h
	Shading/ShadingTypes.h
	Shading/Texture.cpp
	Shading/Texture.h
	Shading/TextureCube.cpp
	Shading/TextureCube.h

	SystemComponents/StatSystemComponent.cpp
	SystemComponents/StatSystemComponent.h
	SystemComponents/SystemComponentManager.cpp
	SystemComponents/SystemComponentManager.h

	Systems/AABB.cpp
	Systems/AABB.h
	Systems/BoundingFrustum.cpp
	Systems/BoundingFrustum.h
	Systems/BoundingSphere.cpp
	Systems/BoundingSphere.h
	Systems/GeomDefines.h
	Systems/Plane.cpp
	Systems/Plane.h
	Systems/Octree.cpp
	Systems/Octree.h
	Systems/GameFrameTime.cpp
	Systems/GameFrameTime.h
	Systems/QuadTree.cpp
	Systems/QuadTree.h
	Systems/Rect.cpp
	Systems/Rect.h


	Utils/FileIO.h
	Utils/Logger.h
	Utils/MathUtils.h
	Utils/Utils.h

	Window/IMGUIHandler.cpp
	Window/IMGUIHandler.h
	Window/SDLHandler.cpp
	Window/SDLHandler.h
	Window/WindowParams.cpp
	Window/WindowParams.h
	
	Game.cpp
	Game.h
	State.h
 
 )

# TODO: Add tests and install targets if needed.
find_package(glm CONFIG REQUIRED)
target_link_libraries(${MODULE_NAME} PUBLIC glm)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(${MODULE_NAME} PUBLIC imgui::imgui)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(${MODULE_NAME} PUBLIC nlohmann_json nlohmann_json::nlohmann_json)

find_package(GLEW REQUIRED)
target_link_libraries(${MODULE_NAME} PUBLIC GLEW::GLEW)

find_package(assimp CONFIG REQUIRED)
target_link_libraries(${MODULE_NAME} PUBLIC assimp::assimp)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(${MODULE_NAME} PUBLIC SDL2::SDL2 SDL2::SDL2main)
