#pragma once

#include "UI.h"

#include "Globals.h"

class MainInterface {
	UI* ui = nullptr;

	UIList<ClickBox> fileList{};
	UIList<ClickBox> addList{};

	public:

	MainInterface(UI* ui) : ui(ui) {
		int x = 5;
		int y = 2;
		int w = 70;
		int h = 20;
		Font* font = ui->GetFont("arial12");

		auto SetUpElem = [ui](ClickBox* cb) {
			cb->SetColor(40, 40, 40, 255);
			cb->SetRenderTextType(TextRenderType::CenteredY);
			cb->textStartX = 5;
			cb->SetHoverFilter(true, 255, 255, 255, 80);
		};

		ClickBox* cbHandler = ui->LCreateClickBox(EditorGlobals::UILayerMenuBar, "fileListMain", x, y, w, h, nullptr, font, "File");
		SetUpElem(cbHandler);
		fileList.Init(ui, cbHandler, w, h, { 40,40,40, 255 }, { "Temp1", "Temp2" }, 0);
		for (auto& listElem : fileList.GetAll()) {
			SetUpElem(listElem);
		}

		x += w + 5;
		cbHandler = ui->LCreateClickBox(EditorGlobals::UILayerMenuBar, "fileListAdd", x, y, w, h, nullptr, font, "Add");
		addList.Init(ui, cbHandler, w, h, { 40,40,40, 255 }, { "Temp1", "Temp2" }, 0);
		SetUpElem(cbHandler);
		for (auto& listElem : addList.GetAll()) {
			SetUpElem(listElem);
		}
	}

	void FrameUpdate() {
		if (fileList.Main()->ConsumeStatus()) {
			fileList.Toogle();
		}
		if (addList.Main()->ConsumeStatus()) {
			addList.Toogle();
		}
	}

	~MainInterface() {
		
	}

	
};