#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <print>
#include <filesystem>
#include <optional>

#include "UI.h"

#include "Globals.h"

enum class AssetType {
	Texture,
	Sound,
	Other
};


class Asset {
public:
	AssetType type = AssetType::Texture;
	std::string name{}; // name + extension 

	Asset(const std::filesystem::path& path) {
		std::string extension = path.extension().string();
		if (extension == ".wav") {
			type = AssetType::Sound;
		}
		else if (extension == ".png") {
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
	std::vector<std::string> folders;
	std::vector<Asset> assets{};
	std::optional<std::string> parent = std::nullopt;

	AssetFolder() = default;

};

class FolderViev {
	std::vector<ClickBox*> folders{};
	std::vector<ClickBox*> files{};
	ClickBox* rootCb = nullptr;

public:
	FolderViev() = default;

	void Create(UI* ui, const MT::Rect& bounds, const AssetFolder& currentFolder);

	void Clear(UI *ui);

	int SubfolderUpdate();

	bool RootUpdate();

};

class AssetTree {
private:
	std::unordered_map<unsigned int, AssetFolder> folders; // Key is unique folder id (current id +1)
	FolderViev currentViev;
	AssetFolder currentFolder{};
	UI* ui = nullptr;
	std::filesystem::path currentPath{}; // Relative to main root folder
	Label* pathLabel = nullptr;
	ClickBox* refreshCb = nullptr;

public:

	void Build();

	void Init(UI* ui, const std::string& path, const MT::Rect &rect) {
		this->ui = ui;
		currentPath = path;
		Build();
		int x = rect.x + 5;
		int y = rect.y + 5;
		refreshCb = ui->LCreateClickBox(EditorGlobals::UILayerLow,"", x, y, 20, 20, TexMan::GetTex("RefreshIcon"));
		refreshCb->SetHoverFilter(true, 255, 255, 255, 120);

		pathLabel = ui->LCreateLabel(EditorGlobals::UILayerLow, "", x + 25, y, 20, 20, nullptr, ui->GetFont("arial12"));
		pathLabel->SetColor(0, 0, 0, 0);
		pathLabel->SetFontColor(150, 150, 150);
		CreateViev(rect);
	}


	// Called when entering subfolder deletes unexisting folders and creates new that are nor included
	// It does not do deep reursion when folder is loaded and still exist

	void CreateViev(const MT::Rect& bounds) {
		currentViev.Clear(ui);
		currentViev.Create(ui, bounds, currentFolder);
		if (pathLabel) {
			pathLabel->text = currentPath.string();
		}

	}

	void FrameUpdate(const MT::Rect& bounds) {
		int folderOut = currentViev.SubfolderUpdate();
		if (folderOut != -1) {
			std::filesystem::path newPath = currentPath;
			newPath.append(currentFolder.folders[folderOut]);
			if (std::filesystem::exists(newPath)) { // Otherwise it tries to remove unexisting elements
				currentPath = currentPath.append(currentFolder.folders[folderOut]);
			}
			Build();
			CreateViev(bounds);
			return;
		}
		unsigned int rootId = currentViev.RootUpdate();
		if (rootId != 0) {
			std::filesystem::path newPath = currentPath.parent_path();
			if (std::filesystem::exists(newPath)) { // Otherwise it tries to remove unexisting elements
				currentPath = currentPath.parent_path();
			}
			Build();
			CreateViev(bounds);
			return;
		}
		if (refreshCb->ConsumeStatus()) {
			Build();
			CreateViev(bounds);
		}
	}
};