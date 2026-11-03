#pragma once

#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <print>

#include "Renderer.h"
#include "json.hpp"
#include "TextureManager.h"

inline static bool LoadingPossible(std::ifstream& file, const std::string& path, nlohmann::json& jsonData) {
	if (!file.is_open()) {
		std::println("Error loading json: {}", path);
		return false;
	}

	try {

		file >> jsonData;
		std::println("JSON Loaded: {}", path);
	}
	catch (const std::exception& e) {
		std::println("Json Error at:{}  [ERROR]: {}", path, e.what());
		return false;
	}
	return true;
}

class MainConfig {
	inline static std::unordered_map<std::string, MT::Texture*> fileExtTextures{};
	inline static MT::Texture* baseExtText = TexMan::GetDefaultTex();

	static void LoadFileExtTex(const std::string &path) {
		baseExtText = TexMan::GetTex("FeFileIcon");
		std::ifstream file(path);
		nlohmann::json jsonData;

		if (!LoadingPossible(file, path, jsonData)) { return; }

		for (auto& [key, value] : jsonData.items()) {
			if (key.empty()) {
				std::println("Empty or missing json key in: {}", path);
				continue;
			}
			if (!value.is_string()) {
				std::println("Invalid JSON value for key: {}", key);
				continue;
			}

			fileExtTextures["." + key] = TexMan::GetTex(value.get<std::string>());
		}
	}

	public:
	static void Load(const std::string& folder) {
		if (!std::filesystem::exists(folder)) {
			std::filesystem::create_directory(folder);
		}
		// Loading extension textures
		LoadFileExtTex(folder + "\\assetsExtTextures.json");
	}

	static MT::Texture* GetExtTexture(const std::string &ext) {
		auto iter = fileExtTextures.find(ext);
		if (iter == fileExtTextures.end()) {
			return baseExtText;
		}
		return iter->second;

	}
};
