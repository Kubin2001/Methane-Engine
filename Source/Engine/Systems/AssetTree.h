#pragma once

#include <vector>
#include <unordered_map>
#include <string>

enum class AssetType {
	Texture,
	Sound,
	Other
};


class Asset {
private:
	AssetType type = AssetType::Texture;
};


class AssetFolder {
private:
	std::string name{};
	std::vector<Asset> assets{};
	std::vector<std::string> subFolders{};
public:
	AssetFolder() = default;

	AssetFolder(const std::string& name, const std::vector<Asset>& assets, const std::vector<std::string>& subFolders)
		:name(name), assets(assets), subFolders(subFolders){}
};

class AssetTree {
private:
	std::unordered_map<std::string, AssetFolder> folders; // Key folder path not name since name can be duplicated

public:
	void Reload(const std::string &path);
};