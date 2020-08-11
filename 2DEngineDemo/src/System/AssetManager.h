#pragma once

#include <unordered_map>
#include <string>

// Load and store assets data from file to use in game (IMAGES, FONTS, SOUNDS, ..)
class AssetManager
{
private:
	std::unordered_map<std::string, int> textures_;
public:
	AssetManager() = default;
	~AssetManager();

	void ClearData();

	void AddTexture(std::string textureID, const wchar_t* filePath);
	int GetTexture(std::string textureID) const;
};

