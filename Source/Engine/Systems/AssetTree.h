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
		if (extension == "wav.") {
			type = AssetType::Sound;
		}
		else if (extension == "png.") {
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
	std::filesystem::path path;
	std::vector<Asset> assets{};
	std::vector<unsigned int> subFolders{}; // folder Keys
	unsigned int  root = 0; // if root is 0 then there is not root (only in main folder)

	AssetFolder() = default;

	AssetFolder(const std::string& name, const std::vector<Asset>& assets, const std::filesystem::path& path,
		unsigned int root) : name(name), path(path), assets(assets), root(root) {}
};

class FolderViev {
	std::vector<ClickBox*> folders{};
	std::vector<unsigned int> foldersId{};
	std::vector<ClickBox*> files{};
	ClickBox* rootCb = nullptr;
	unsigned int rootId{};

public:
	FolderViev() = default;

	void Create(UI* ui, const MT::Rect& bounds, const std::unordered_map<unsigned int, AssetFolder>& foldersMap,
		unsigned int id);

	void Clear(UI *ui);

	unsigned int SubfolderUpdate();

	unsigned int RootUpdate();

};

class AssetTree {
private:
	std::unordered_map<unsigned int, AssetFolder> folders; // Key is unique folder id (current id +1)
	FolderViev currentViev;
	UI* ui = nullptr;

public:
	void Init(UI* ui) {
		this->ui = ui;
	}
	void ReBuild(const std::string& path, unsigned int rootId = 0);

	void CreateViev(const MT::Rect& bounds, unsigned int id) {
		currentViev.Clear(ui);
		auto fIter = folders.find(id);
		if (fIter != folders.end()) {
			currentViev.Create(ui, bounds, folders, fIter->first);
		}
	}

	void FrameUpdate(const MT::Rect& bounds) {
		unsigned int folderOut = currentViev.SubfolderUpdate();
		if (folderOut != 0) {
			CreateViev(bounds, folderOut);
			return;
		}
		unsigned int rootId = currentViev.RootUpdate();
		if (rootId != 0) {
			CreateViev(bounds, rootId);
			return;
		}
	}
};