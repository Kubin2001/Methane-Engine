#pragma once

#include <vector>

#include <UI.h>

#include "Object.h"

class ObjectViev {
	std::string name{};
	ClickBox* mainButton = nullptr;
	Object* associatedObject = nullptr;

	public:
	ObjectViev(const std::string& name, UI *ui, Point pos) {
		this->name = name;
		mainButton = ui->LCreateClickBox(EditorGlobals::UILayerLow, "", pos.x, pos.y, 20, 20, TexMan::GetTex("RefreshIcon"), ui->GetFont("arial12px"), name);;
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
		Point pos{ bounds.x + 10, static_cast<int>((bounds.y + 10) + (definedObjects.size() * 25)) };
		definedObjects.emplace_back(ObjectViev(name, ui,pos));
		std::println("Called!!!");
	}

};