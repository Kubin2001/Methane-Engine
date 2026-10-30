#pragma once

#include <memory>

#include "SceneManager.h"

#include "Logger.h"

#include "MainInterface.h"
#include "AssetTree.h"

class EngineScene : public Scene {
	private:
	std::unique_ptr<MainInterface> mainInterface{};
	AssetTree tree;

	public:
	void Init() override{
		mainInterface = std::make_unique<MainInterface>(ui);
		std::filesystem::path assetsPath("Assets");
		if(!std::filesystem::exists(assetsPath)){
			std::filesystem::create_directory(assetsPath);
		}
		tree.Init(ui, assetsPath.string(), mainInterface->GetDownRect());


		Logger::Log("Engine Scene On");

	}

	void LogicUpdate() override{}

	void FrameUpdate() override{
		mainInterface->FrameUpdate();
		tree.FrameUpdate(mainInterface->GetDownRect());
	}

	void Input(SDL_Event& event) {}

	void Render() override{}

	void Clear() override{
		ui->ClearAll();
	}
};