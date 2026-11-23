#pragma once

#include <vector>

#include <UI.h>

#include "Object.h"

class ObjectViev {
	std::string name{};
	ClickBox* mainButton = nullptr;
	Object* associatedObject = nullptr;

	public:
	ObjectViev(const std::string& name, UI *ui, const MT::Rect &bounds, int y) {
		this->name = name;
		mainButton = ui->LCreateClickBox(EditorGlobals::UILayerSceneLayout, "", bounds.x + 10, y, bounds.w - 20, 20, nullptr, ui->GetFont("arial12"), name,1.0f,5);
		mainButton->SetBorder(1, 255, 255, 255, 120);
		mainButton->SetColor(30, 30, 30,0);
		mainButton->SetTextAligment(TextAligment::CenteredY);
	}

};

class SceneLayout {
	std::vector<ObjectViev> definedObjects{};
	UI* ui = nullptr;
	MT::Rect bounds{};
	public:

	SceneLayout(UI *ui, MT::Rect bounds): ui(ui), bounds(bounds){
	}

	void Add(const std::string& name) {
		int y = static_cast<int>((bounds.y + 10) + (definedObjects.size() * 25));
		definedObjects.emplace_back(ObjectViev(name, ui, bounds, y));
		std::println("Called!!!");
	}

};