#pragma once

#include <memory>

#include "SceneManager.h"

#include "Logger.h"

#include "MainInterface.h"
#include "AssetTree.h"

class EngineScene : public Scene {
	private:
	std::unique_ptr<MainInterface> mainInterface{};

	public:
	void Init() override{
		mainInterface = std::make_unique<MainInterface>(ui);
		AssetTree tree;
		tree.Reload("Textures");

		Logger::Log("Engine Scene On");

	}

	void LogicUpdate() override{}

	void FrameUpdate() override{
		mainInterface->FrameUpdate();
	}

	void Input(SDL_Event& event) {}

	void Render() override{}

	void Clear() override{
		ui->ClearAll();
	}
};