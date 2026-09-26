#include "AssetTree.h"



#include "Globals.h"

unsigned int folderID = 1;


void FolderViev::Create(UI *ui, const MT::Rect& bounds, const std::unordered_map<unsigned int, AssetFolder> &foldersMap,
	unsigned int id) {
	int xStart = bounds.x + 10;
	int yStart = bounds.y + 10;

	int x = xStart;
	int y = yStart;
	int size = 40;
	int maxX = bounds.x + bounds.w - size;
	int step = size + 50;
	Font* font = ui->GetFont("arial12");

	const AssetFolder* folder = nullptr;
	auto folderIter = foldersMap.find(id);
	if (folderIter == foldersMap.end()) {
		throw std::runtime_error("Unexisting folder this should not exist");
	}
	folder = &folderIter->second;

	const AssetFolder* root = nullptr;
	auto rootIter = foldersMap.find(folder->root);
	if (rootIter != foldersMap.end()) {
		root = &rootIter->second;
	}



	auto FillElem = [](UIElemBase* elem, const std::string &text, int size) {
		elem->text = text;
		elem->SetRenderTextType(TextRenderType::CenteredX);
		elem->SetHoverFilter(true, 255, 255, 255, 120);
		elem->textStartY = size + 5;
	};

	// Root
	if (root) {
		ClickBox* cb = ui->LCreateClickBox(EditorGlobals::UILayerLow, AnonUIName(), x, y, size, size,
			TexMan::GetTex("RootFolderIcon"), font);
		FillElem(cb, std::filesystem::path(root->path).stem().string().substr(0, 12), size);
		rootId = folder->root;
		rootCb = cb;
		x += step;
	}

	// SubFolders
	for (auto &subFolder : folder->subFolders) {
		const AssetFolder* sFolder = nullptr;
		auto sFolderIter = foldersMap.find(subFolder);
		if (sFolderIter != foldersMap.end()) {
			sFolder = &sFolderIter->second;
		}
		if (!sFolder) {
			continue;
		}
		ClickBox* cb = ui->LCreateClickBox(EditorGlobals::UILayerLow, AnonUIName(), x, y, size, size, 
			TexMan::GetTex("FeFolderIcon"),font);
		FillElem(cb, std::filesystem::path(sFolder->path).stem().string().substr(0, 12), size);
		folders.emplace_back(cb);
		foldersId.emplace_back(subFolder);
		x += step;
		if (x > maxX) {
			x = xStart;
			y += step;
		}
	}

	// Assets
	for (auto& asset : folder->assets) {
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
	foldersId.clear();
	rootId = 0;
	files.clear();
	if (rootCb) {
		ui->DeleteElement(rootCb->GetName());
		rootCb = nullptr;
	}
}

unsigned int FolderViev::SubfolderUpdate() {
	for (size_t i = 0; i < folders.size(); i++) {
		auto& folder = folders[i];
		if (folder->ConsumeStatus()) {
			return foldersId[i];
		}
	}
	return 0u;
}

unsigned int FolderViev::RootUpdate() {
	if (rootCb && rootCb->ConsumeStatus()) {
		return rootId;
	}
	return 0;
}

void AssetTree::ReBuild(const std::string& path, unsigned int rootId) {
	namespace fs = std::filesystem;

	if (!fs::exists(path)) {
		return;
	}
	AssetFolder* root = nullptr;
	unsigned int id = folderID++;
	auto folderIter = folders.find(rootId);
	if (folderIter != folders.end()) {
		root = &folderIter->second;
		root->subFolders.emplace_back(id);
	}

	std::string folderName = std::filesystem::path(path).stem().string();
	std::vector<Asset> assets;
	for (auto& entry : fs::directory_iterator(path)) {
		std::filesystem::path subPath = entry.path();
		if (!entry.is_directory()) {
			assets.emplace_back(subPath);
		}
	}
	auto foldersIter = folders.find(id);
	if (foldersIter == folders.end()) {
		folders[id] = AssetFolder(folderName, assets, path, rootId);
	}

	for (auto& entry : fs::directory_iterator(path)) {
		std::filesystem::path subPath = entry.path();
		if (entry.is_directory()) {
			ReBuild(subPath.string(), id);
		}
	}
}
