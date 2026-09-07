#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <filesystem>

#include "UI.h"

enum class AssetType {
	Texture,
	Sound,
	Other
};


class Asset {
public:
	AssetType type = AssetType::Texture;
	std::string name{};

	Asset(const std::filesystem::path& path) {
		std::string extension = path.extension().string();
		if (extension == "wav") {
			type = AssetType::Sound;
		}
		else if (extension == "png") {
			type = AssetType::Texture;
		}
		else {
			type = AssetType::Other;
		}
		name = path.filename().string();
	}
};


class AssetFolder {
public:
	std::string name{};
	std::vector<Asset> assets{};
	std::vector<std::string> subFolders{};

	AssetFolder() = default;

	AssetFolder(const std::string& name, const std::vector<Asset>& assets, const std::vector<std::string>& subFolders)
		:name(name), assets(assets), subFolders(subFolders){}
};

class FolderViev {
	std::vector<Label*> folders{};
	std::vector<Label*> files{};

public:
	FolderViev() = default;

	void Create(UI *ui, const MT::Rect& bounds, const AssetFolder& folder);

};

class AssetTree {
private:
	std::unordered_map<std::string, AssetFolder> folders; // Key folder path not name since name can be duplicated
	FolderViev currentViev;
	UI* ui = nullptr;

public:
	void Init(UI* ui) {
		this->ui = ui;
	}
	void ReBuild(const std::string &path);

	void CreateViev(const MT::Rect& bounds, const std::string& folderName) {
		auto fIter = folders.find(folderName);
		if (fIter != folders.end()) {
			currentViev.Create(ui, bounds, fIter->second);
		}
	}
};