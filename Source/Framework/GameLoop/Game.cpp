#include "Game.h"

#include <SDL.h>
#include <print>

#include "GlobalVariables.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "SceneManager.h"
#include "Addons.h"
#include "Files.h"
#include "Logger.h"
#include "EngineScene.h"

void Game::Start() {
	MethaneVersion();
	MT::SetSeed(static_cast<unsigned int>(time(0)));

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	Global::windowWidth = 1280;
	Global::windowHeight = 720;

	window.Init("Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Global::windowWidth, Global::windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	renderer = new MT::Renderer();
	renderer->Start(window);

	window.SetFullScreen();
	renderer->Resize(window.GetSize().x, window.GetSize().y);
	Global::windowWidth = window.GetSize().x;
	Global::windowHeight = window.GetSize().y;

	TexMan::Start(renderer);
	TexMan::DeepLoad("Textures");
	SoundMan::Init();
	SoundMan::DeepLoad("Sounds");

	ui = std::make_unique<UI>(renderer);
	ui->UseLayerInRendering(true);
	ui->settings.stopCheckAtFirst = true;
	ui->settings.stopHoverAtFirst = true;

	ui->CrateTempFontFromTTF("Fonts/arial.ttf", 12, "arial12");
	ui->CrateTempFontFromTTF("Fonts/arial.ttf", 14, "arial14");
	ui->CrateTempFontFromTTF("Fonts/arial.ttf", 18, "arial18");
	ui->CrateTempFontFromTTF("Fonts/arial.ttf", 20, "arial20");
	ui->CrateTempFontFromTTF("Fonts/arial.ttf", 40, "arial40");

	renderer->FLatRenderCopySetUp();

	Logger::SetUp("", LogOutput::Console, LogOutput::Console);

	SceneMan::AddScene<EngineScene>("EngineScene");
	SceneMan::SwitchScene<EngineScene>("EngineScene", renderer, ui.get());
}

void Game::LogicUpdate() {
	Global::frameCounter++;
	SceneMan::GetCurrentScene()->LogicUpdate();
}

void Game::FrameUpdate() {
	Input();
	SceneMan::GetCurrentScene()->FrameUpdate();
	ui->FrameUpdate();
	Render();
}

void Game::Input() {
	while (SDL_PollEvent(&event)) {
		SceneMan::GetCurrentScene()->Input(event);
		ui->ManageInput(event);
		Exit();
	}
	Global::inputDelay++;
}

void Game::Render() {
	renderer->ClearFrame(51, 51, 51);
	SceneMan::GetCurrentScene()->Render();
	ui->Render();
	renderer->Present();
}


void Game::Exit() {
	if (event.type == SDL_QUIT) {
		Global::status = false; 
	}
	else if (event.type == SDL_KEYUP && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
		Global::status = false;
	}
}

Game::~Game() {
	SceneMan::GetCurrentScene()->Clear();
	Logger::Close();
	TexMan::Clear();
	SoundMan::Clear();
	SceneMan::Clear();
	renderer->Clear();
	SDL_Quit();
}
