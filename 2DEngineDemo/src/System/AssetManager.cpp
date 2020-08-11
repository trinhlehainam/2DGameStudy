#include "AssetManager.h"

#include "TextureManager.h"

AssetManager::~AssetManager()
{
}

void AssetManager::ClearData()
{
	for (auto texture : textures_)
	{
		TextureManager::DeleteGraph(texture.second);
	}
	textures_.clear();
}

void AssetManager::AddTexture(std::string textureID, const wchar_t* filePath)
{
	textures_.emplace(textureID, TextureManager::LoadTexture(filePath));
}

int AssetManager::GetTexture(std::string textureID) const
{
	return textures_.at(textureID);
}
