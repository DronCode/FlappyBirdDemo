#include "oxygine-framework.h"
#include "oxygine-sound.h"
#include "res.h"
#include "Game.h"
#include "MainMenuScene.h"
#include "GameScene.h"
#include "GameMenu.h"
#include "ResFontFT.h"
#include "FlappyBirdBootstrap.h"
#include "GameResultDialog.h"

using namespace oxygine;

SoundPlayer player;

void FlappyBirdBootstrap::preInit() {
}

void FlappyBirdBootstrap::init()
{
	ResFontFT::initLibrary();
    ResFontFT::setSnapSize(10);

    SoundSystem::create()->init(4);
    
    //load resources
    res::load();

	flow::init();
    //create all scenes
    GameMenu::instance = new GameMenu;
    GameScene::instance = new GameScene;
    MainMenuScene::instance = new MainMenuScene;
    GameResultDialog::instance = new GameResultDialog;

    //show main menu
    flow::show(MainMenuScene::instance);
}

void FlappyBirdBootstrap::update()
{
    SoundSystem::get()->update();
    player.update();
	flow::update();
}

void FlappyBirdBootstrap::destroy()
{
    MainMenuScene::instance = nullptr;
    GameScene::instance = nullptr;
    GameMenu::instance = nullptr;
    GameResultDialog::instance = nullptr;

    res::free();
	ResFontFT::freeLibrary();

    SoundSystem::free();
	flow::free();
}