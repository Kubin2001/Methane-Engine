#pragma once

#include <memory>

#include "SceneManager.h"

#include "Logger.h"

#include "MainInterface.h"
#include "AssetTree.h"
#include "MainConfig.h"
#include "SceneLayout.h"

class EngineScene : public Scene {
	private:
	std::unique_ptr<MainInterface> mainInterface{};
	AssetTree tree;
	std::unique_ptr<SceneLayout> sceneLayout = nullptr;

	public:
	void Init() override{
		mainInterface = std::make_unique<MainInterface>(ui);
		sceneLayout = std::make_unique<SceneLayout>(ui, mainInterface->GetSceneLayoutRect());
		MainConfig::Load("Common");
		std::filesystem::path assetsPath("Assets");
		if(!std::filesystem::exists(assetsPath)){
			std::filesystem::create_directory(assetsPath);
		}
		tree.Init(ui, assetsPath.string(), mainInterface->GetAssetsRect());
		ui->SetLayerClipTest(true, mainInterface->GetSceneLayoutRect(), EditorGlobals::UILayerSceneLayout);

		Logger::Log("Engine Scene On");

	}

	void LogicUpdate() override{}

	void FrameUpdate() override{
		mainInterface->FrameUpdate();
		tree.FrameUpdate(mainInterface->GetAssetsRect());

		if (mainInterface->EntityAddSignal()) {
			sceneLayout->Add("New Object");
		}
	}

	void Input(SDL_Event& event)override {}

	void Render() override{}

	void Clear() override{
		ui->ClearAll();
	}
};