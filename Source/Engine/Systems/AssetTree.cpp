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

	for (auto &subFolder : folder.subFolders) {
		Label* lb = ui->LCreateLabel(EditorGlobals::UILayerLow, AnonUIName(), x, y, size, size, 
			TexMan::GetTex("FeFolderIcon"),font);
		
		lb->text = std::filesystem::path(subFolder).stem().string().substr(0,12);
		lb->SetRenderTextType(TextRenderType::CenteredX);
		lb->SetHoverFilter(true, 255, 255, 255, 120);
		lb->textStartY = size + 5;
		x += step;
		if (x > maxX) {
			x = xStart;
			y += step;
		}
	}

	for (auto& asset : folder.assets) {
		Label* lb = ui->LCreateLabel(EditorGlobals::UILayerLow, AnonUIName(), x, y, size, size,
			TexMan::GetTex("pngTex"), font);

		lb->text = asset.name.substr(0, 12);
		lb->SetRenderTextType(TextRenderType::CenteredX);
		lb->SetHoverFilter(true, 255, 255, 255, 120);
		lb->textStartY = size + 5;
		x += step;
		if (x > maxX) {
			x = xStart;
			y += step;
		}
	}
}

void AssetTree::ReBuild(const std::string& strPath) {
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
			assets.emplace_back(path);
		}
	}
	auto foldersIter = folders.find(strPath);
	if (foldersIter == folders.end()) {
		folders[strPath] = AssetFolder(folderName, assets, subFolders);
	}

	for (auto& folder : subFolders) {
		ReBuild(folder);
	}
}
