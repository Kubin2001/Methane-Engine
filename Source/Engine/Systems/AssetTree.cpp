#include "AssetTree.h"



#include "Globals.h"


void FolderViev::Create(UI *ui, const MT::Rect& bounds, const AssetFolder& folder) {
	int xStart = bounds.x + 10;
	int yStart = bounds.y + 10;

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
	if (folder.root != "") {
		ClickBox* cb = ui->LCreateClickBox(EditorGlobals::UILayerLow, AnonUIName(), x, y, size, size,
			TexMan::GetTex("RootFolderIcon"), font);
		FillElem(cb, std::filesystem::path(folder.root).stem().string().substr(0, 12), size);
		rootName = std::filesystem::path(folder.root).stem().string();
		root = cb;
		x += step;
	}

	// SubFolders
	for (auto &subFolder : folder.subFolders) {
		ClickBox* cb = ui->LCreateClickBox(EditorGlobals::UILayerLow, AnonUIName(), x, y, size, size, 
			TexMan::GetTex("FeFolderIcon"),font);
		FillElem(cb, std::filesystem::path(subFolder).stem().string().substr(0, 12), size);
		folders.emplace_back(cb);
		fullFoldersNames.emplace_back(subFolder);
		x += step;
		if (x > maxX) {
			x = xStart;
			y += step;
		}
	}

	// Assets
	for (auto& asset : folder.assets) {
		ClickBox* cb = ui->LCreateClickBox(EditorGlobals::UILayerLow, AnonUIName(), x, y, size, size,
			TexMan::GetTex("pngTex"), font);
		FillElem(cb, std::filesystem::path(asset.name).stem().string().substr(0, 12), size);
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
	fullFoldersNames.clear();
	rootName.clear();
	files.clear();
	if (root) {
		ui->DeleteElement(root->GetName());
		root = nullptr;
	}
}

std::string FolderViev::SubfolderUpdate() {
	for (size_t i = 0; i < folders.size(); i++) {
		auto& folder = folders[i];
		if (folder->ConsumeStatus()) {
			return fullFoldersNames[i];
		}
	}
	return "";
}

std::string FolderViev::RootUpdate() {
	if (root && root->ConsumeStatus()) {
		return rootName;
	}
	return "";
}

void AssetTree::ReBuild(const std::string& strPath, const std::string &root) {
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
			subFolders.emplace_back(path.stem().string());
		}
		else {
			assets.emplace_back(path);
		}
	}
	auto foldersIter = folders.find(strPath);
	if (foldersIter == folders.end()) {
		folders[folderName] = AssetFolder(strPath, assets, subFolders, root);
	}

	for (auto& folder : subFolders) {
		ReBuild(strPath + "/" + folder, strPath);
	}
}
