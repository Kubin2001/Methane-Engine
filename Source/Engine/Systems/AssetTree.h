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
	std::string root{};

	AssetFolder() = default;

	AssetFolder(const std::string& name, const std::vector<Asset>& assets, 
		const std::vector<std::string>& subFolders, const std::string & root)
		:name(name), assets(assets), subFolders(subFolders), root(root){}
};

class FolderViev {
	std::vector<ClickBox*> folders{};
	std::vector<std::string> fullFoldersNames{};
	std::vector<ClickBox*> files{};
	ClickBox* root = nullptr;
	std::string rootName{};

public:
	FolderViev() = default;

	void Create(UI *ui, const MT::Rect& bounds, const AssetFolder& folder);

	void Clear(UI *ui);

	std::string SubfolderUpdate();

	std::string RootUpdate();

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
	void ReBuild(const std::string &path, const std::string& root = "");

	void CreateViev(const MT::Rect& bounds, const std::string& folderName) {
		currentViev.Clear(ui);
		auto fIter = folders.find(folderName);
		if (fIter != folders.end()) {
			currentViev.Create(ui, bounds, fIter->second);
		}
	}

	void FrameUpdate(const MT::Rect& bounds) {
		std::string folderOut = currentViev.SubfolderUpdate();
		if (!folderOut.empty()) {
			CreateViev(bounds, folderOut);
			return;
		}
		std::string rootName = currentViev.RootUpdate();
		if (!rootName.empty()) {
			CreateViev(bounds, rootName);
			return;
		}
	}
};