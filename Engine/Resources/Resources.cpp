#include "Resources.h"

#include "ShaderLoader.h"
#include "TextureLoader.h"
#include "MeshLoader.h"

#include "Scene/Scene.h"
#include "Scene/SceneNode.h"

#include "Utils/Utils.h"

#include <stack>
#include <vector>

#include "Utils/Logger.h"

std::map<unsigned int, Shader>      Resources::m_Shaders = std::map<unsigned int, Shader>();
std::map<unsigned int, Texture>     Resources::m_Textures = std::map<unsigned int, Texture>();
std::map<unsigned int, TextureCube> Resources::m_TexturesCube = std::map<unsigned int, TextureCube>();
std::map<unsigned int, SceneNode*>  Resources::m_Meshes = std::map<unsigned int, SceneNode*>();

void Resources::Init()
{
	// initialize default assets/resources that should  always be available, regardless of 
	// configuration.        
	Texture placeholderTexture;
}
void Resources::Clean()
{
	// traverse all stored mesh scene nodes and delete accordingly.
	// Note that this time we don't care about deleting dangling pointers as each scene node is
	// unique and shouldn't reference other scene nodes than their children.
	for (auto it = m_Meshes.begin(); it != m_Meshes.end(); it++)
	{
		delete it->second;
	}
}

Shader* Resources::LoadShader(std::string name, std::string vsPath, std::string fsPath, std::vector<std::string> defines)
{
	unsigned int id = Utils::Hash(name);

	// if shader already exists, return that handle
	if (Resources::m_Shaders.find(id) != Resources::m_Shaders.end())
		return &Resources::m_Shaders[id];

	Shader shader = ShaderLoader::Load(name, "../../../../data/" + vsPath, "../../../../data/" + fsPath, defines);
	Resources::m_Shaders[id] = shader;
	return &Resources::m_Shaders[id];
}

Shader* Resources::GetShader(std::string name)
{
	unsigned int id = Utils::Hash(name);

	// if shader exists, return that handle
	if (Resources::m_Shaders.find(id) != Resources::m_Shaders.end())
	{
		return &Resources::m_Shaders[id];
	}
	else
	{
		LOG_ERROR("Requested shader: %s not found!", name.c_str());
		return nullptr;
	}
}

Texture* Resources::LoadTexture(std::string name, std::string path, GLenum target, GLenum format, bool srgb)
{
	unsigned int id = Utils::Hash(name);

	// if texture already exists, return that handle
	if (Resources::m_Textures.find(id) != Resources::m_Textures.end())
		return &Resources::m_Textures[id];

	LOG("Loading texture file at: %s", path.c_str());

	Texture texture = TextureLoader::LoadTexture("../../../../data/" + path, target, format, srgb);

	LOG("Succesfully loaded: %s", path.c_str());

	// make sure texture got properly loaded
	if (texture.Width > 0)
	{
		Resources::m_Textures[id] = texture;
		return &Resources::m_Textures[id];
	}
	else
	{
		return nullptr;
	}
}

Texture* Resources::LoadHDR(std::string name, std::string path)
{
	unsigned int id = Utils::Hash(name);

	// if texture already exists, return that handle
	if (Resources::m_Textures.find(id) != Resources::m_Textures.end())
		return &Resources::m_Textures[id];

	Texture texture = TextureLoader::LoadHDRTexture("../../../../data/" + path);
	// make sure texture got properly loaded
	if (texture.Width > 0)
	{
		Resources::m_Textures[id] = texture;
		return &Resources::m_Textures[id];
	}
	else
	{
		return nullptr;
	}
}

Texture* Resources::GetTexture(std::string name)
{
	unsigned int id = Utils::Hash(name);

	// if shader exists, return that handle
	if (Resources::m_Textures.find(id) != Resources::m_Textures.end())
	{
		return &Resources::m_Textures[id];
	}
	else
	{
		LOG("Requested texture: %s not found", name.c_str());
		return nullptr;
	}
}

TextureCube* Resources::LoadTextureCube(std::string name, std::string folder)
{
	unsigned int id = Utils::Hash(name);

	// if texture already exists, return that handle
	if (Resources::m_TexturesCube.find(id) != Resources::m_TexturesCube.end())
		return &Resources::m_TexturesCube[id];

	TextureCube texture = TextureLoader::LoadTextureCube("../../../../data/" + folder);
	Resources::m_TexturesCube[id] = texture;
	return &Resources::m_TexturesCube[id];
}

TextureCube* Resources::GetTextureCube(std::string name)
{
	unsigned int id = Utils::Hash(name);

	// if shader exists, return that handle
	if (Resources::m_TexturesCube.find(id) != Resources::m_TexturesCube.end())
	{
		return &Resources::m_TexturesCube[id];
	}
	else
	{
		LOG("Requested texture cube: %s not found!", name.c_str());
		return nullptr;
	}
}

SceneNode* Resources::LoadMesh(Renderer* renderer, std::string name, std::string path)
{
	unsigned int id = Utils::Hash(name);

	// if mesh's scene node was already loaded before, copy the scene node's memory and return 
	// the copied reference. We return a copy as the moment the global scene deletes the 
	// returned node, all other and next requested scene nodes of this model will end up as
	// dangling pointers.
	if (Resources::m_Meshes.find(id) != Resources::m_Meshes.end())
	{
		return Scene::MakeSceneNode(Resources::m_Meshes[id]);
	}

	// MeshLoader::LoadMesh initializes a scene node hierarchy on the heap. We are responsible 
	// for managing the memory; keep a reference to the root node of the model scene. 
	SceneNode* node = MeshLoader::LoadMesh(renderer, "../../../../data/" + path);
	Resources::m_Meshes[id] = node;

	// return a copied reference through the scene to prevent dangling pointers. 
	// See description above.
	return Scene::MakeSceneNode(node);
}

SceneNode* Resources::GetMesh(std::string name)
{
	unsigned int id = Utils::Hash(name);

	// if mesh's scene node was already loaded before, copy the scene node's memory and return 
	// the copied reference. We return a copy as the moment the global scene deletes the 
	// returned node, all other and next requested scene nodes of this model will end up as
	// dangling pointers.
	if (Resources::m_Meshes.find(id) != Resources::m_Meshes.end())
	{
		return Scene::MakeSceneNode(Resources::m_Meshes[id]);
	}
	else
	{
		LOG("Requested mesh: %s not found!", name.c_str());
		return nullptr;
	}
}
