#include "AssetTree.h"





unsigned int folderID = 1;


void FolderViev::Create(UI *ui, const MT::Rect& bounds, const AssetFolder &currentFolder) {
	int xStart = bounds.x + 20;
	int yStart = bounds.y + 30;

	int x = xStart;
	int y = yStart;
	int size = 40;
	int maxX = bounds.x + bounds.w - size;
	int step = size + 50;
	Font* font = ui->GetFont("arial12");


	auto FillElem = [](UIElemBase* elem, const std::string &text, int size) {
		elem->text = text;
		elem->SetRenderTextType(TextRenderType::CenteredX);
		elem->SetHoverFilter(true, 255, 255, 255, 120);
		elem->textStartY = size + 5;
	};

	// Root
	if (currentFolder.parent) {
		ClickBox* cb = ui->LCreateClickBox(EditorGlobals::UILayerLow, AnonUIName(), x, y, size, size,
			TexMan::GetTex("RootFolderIcon"), font);
		FillElem(cb, currentFolder.parent->substr(0, 12), size);
		rootCb = cb;
		x += step;
	}

	// SubFolders
	for (auto &subFolder : currentFolder.folders) {
		ClickBox* cb = ui->LCreateClickBox(EditorGlobals::UILayerLow, AnonUIName(), x, y, size, size, 
			TexMan::GetTex("FeFolderIcon"),font);
		FillElem(cb, subFolder.substr(0, 12), size);
		folders.emplace_back(cb);
		x += step;
		if (x > maxX) {
			x = xStart;
			y += step;
		}
	}

	// Assets
	for (auto& asset : currentFolder.assets) {
		ClickBox* cb = ui->LCreateClickBox(EditorGlobals::UILayerLow, AnonUIName(), x, y, size, size,
			TexMan::GetTex("pngTex"), font);
		FillElem(cb, std::filesystem::path(asset.name).filename().string().substr(0, 12), size);
		files.emplace_back(cb);
		x += step;
		if (x > maxX) {
			x = xStart;
			y += step;
		}
	}
}

void FolderViev::Clear(UI* ui) {
	for (auto& lb : folders) {
		ui->DeleteElement(lb->GetName());
	}
	for (auto& lb : files) {
		ui->DeleteElement(lb->GetName());
	}
	folders.clear();
	files.clear();
	if (rootCb) {
		ui->DeleteElement(rootCb->GetName());
		rootCb = nullptr;
	}
}

int FolderViev::SubfolderUpdate() {
	for (size_t i = 0; i < folders.size(); i++) {
		auto& folder = folders[i];
		if (folder->ConsumeStatus()) {
			return i;
		}
	}
	return -1;
}

bool FolderViev::RootUpdate() {
	return (rootCb && rootCb->ConsumeStatus());
}

void AssetTree::Build() {
	namespace fs = std::filesystem;

	if (!fs::exists(currentPath)) {
		return;
	}


	currentFolder.assets.clear();
	currentFolder.folders.clear();

	for (auto& entry : fs::directory_iterator(currentPath)) {
		std::filesystem::path subPath = entry.path();
		if (entry.is_directory()) {
			currentFolder.folders.emplace_back(subPath.filename().string());
		}
		else {
			currentFolder.assets.emplace_back(subPath);
		}
	}

	if (currentPath.has_parent_path()) {
		currentFolder.parent = currentPath.parent_path().filename().string();
	}
	else {
		currentFolder.parent = std::nullopt;
	}

	std::println("Current Path: {}", currentPath.string());
}

