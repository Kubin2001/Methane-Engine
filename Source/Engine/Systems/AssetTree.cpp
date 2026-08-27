#include "AssetTree.h"

#include <filesystem>

void AssetTree::Reload(const std::string& strPath) {
	namespace fs = std::filesystem;

	if (!fs::exists(strPath)) {
		return;
	}
	std::string folderName = std::filesystem::path(strPath).stem().string();
	std::vector<std::string> subFolders;
	std::vector<Asset> assets;
	for (auto& entry : fs::directory_iterator(strPath)) {
		std::filesystem::path path = entry.path();
		if (entry.is_directory()) {
			subFolders.emplace_back(path.string());
		}
		else {
			assets.emplace_back();
		}
	}
	auto foldersIter = folders.find(strPath);
	if (foldersIter == folders.end()) {
		folders[strPath] = AssetFolder(folderName, assets, subFolders);
	}

	for (auto& folder : subFolders) {
		Reload(folder);
	}
}