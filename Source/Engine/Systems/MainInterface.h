#pragma once

#include "UI.h"

#include "Globals.h"

class MainInterface {
	UI* ui = nullptr;

	UIList<ClickBox> fileList{};
	UIList<ClickBox> addList{};

	// Panels
	Label* upPanel = nullptr; // Add / File
	Label* leftPanel = nullptr; // Object List
	Label* rightPanel = nullptr; // Object Properities
	Label* downPanel = nullptr; // Resources
	MT::Rect gamePanel{};

	void SetUpPanels() {
		auto SetUpElem = [](UIElemBase* elem) {
			elem->SetColor(30, 30, 30, 255);
			elem->SetRenderTextType(TextRenderType::CenteredX);
			elem->textStartY = 5;
			elem->SetBorder(1, 255, 255, 255, 120);
		};
		MT::Rect upRect{ 0, 0, Global::windowWidth, 50 };
		MT::Rect downRect{ 0, Global::windowHeight - 300, Global::windowWidth ,300};
		MT::Rect leftRect = RectFillBetweenY(0, Global::windowWidth * 0.20, upRect, downRect);
		MT::Rect rightRect = RectFillBetweenY(Global::windowWidth - Global::windowWidth * 0.20, Global::windowWidth * 0.20, upRect, downRect);
		gamePanel = MT::Rect{ leftRect.x + leftRect.w, upRect.y + upRect.h,
			Global::windowWidth - (leftRect.w + rightRect.w), Global::windowWidth - (upRect.h + downRect.h) };
		upPanel = ui->LCreateLabel(EditorGlobals::UILayerLowest, AnonUIName(), upRect.x, upRect.y, upRect.w, upRect.h);
		downPanel = ui->LCreateLabel(EditorGlobals::UILayerLowest, AnonUIName(), downRect.x, downRect.y, downRect.w, downRect.h);
		leftPanel = ui->LCreateLabel(EditorGlobals::UILayerLowest, AnonUIName(), leftRect.x, leftRect.y, leftRect.w, leftRect.h);
		rightPanel = ui->LCreateLabel(EditorGlobals::UILayerLowest, AnonUIName(), rightRect.x, rightRect.y, rightRect.w, rightRect.h);
		SetUpElem(upPanel);
		SetUpElem(downPanel);
		SetUpElem(leftPanel);
		SetUpElem(rightPanel);
	}


	void SetUpList() {
		int x = 5;
		int y = 2;
		int w = 70;
		int h = 20;
		Font* font = ui->GetFont("arial12");

		auto SetUpElem = [](ClickBox* cb) {
			cb->SetColor(40, 40, 40, 255);
			cb->SetRenderTextType(TextRenderType::CenteredY);
			cb->textStartX = 5;
			cb->SetHoverFilter(true, 255, 255, 255, 80);
		};

		ClickBox* cbHandler = ui->LCreateClickBox(EditorGlobals::UILayerMenuBar, "fileListMain", x, y, w, h, nullptr, font, "File");
		SetUpElem(cbHandler);
		fileList.Init(ui, cbHandler, w, h, { 40,40,40, 255 }, { "Open", "Save", "Save As" , "Close"}, 0);
		for (auto& listElem : fileList.GetAll()) {
			SetUpElem(listElem);
		}

		x += w + 5;
		cbHandler = ui->LCreateClickBox(EditorGlobals::UILayerMenuBar, "fileListAdd", x, y, w, h, nullptr, font, "Add");
		addList.Init(ui, cbHandler, w, h, { 40,40,40, 255 }, { "Scene", "Map", "Entity", "Rect"}, 0);
		SetUpElem(cbHandler);
		for (auto& listElem : addList.GetAll()) {
			SetUpElem(listElem);
		}
	}



	public:

	MainInterface(UI* ui) : ui(ui) {
		SetUpPanels();
		SetUpList();
	}

	void FrameUpdate() {
		if (fileList.Main()->ConsumeStatus()) {
			fileList.Toogle();
		}
		if (addList.Main()->ConsumeStatus()) {
			addList.Toogle();
		}

		if (addList.IsExpanded()) {
			if (addList[3]->ConsumeStatus()) {
				addList[3]->SetColor(RandInt(0, 255), RandInt(0, 255), RandInt(0, 255));
			}
		}
	}

	~MainInterface() {
		
	}

	
};