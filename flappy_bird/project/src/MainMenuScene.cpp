#include "MainMenuScene.h"
#include "res.h"
#include "MyButton.h"
#include "GameScene.h"

spMainMenuScene MainMenuScene::instance;

MainMenuScene::MainMenuScene()
{
    setName("MainMenuScene");
    //create background
    spSprite sky = new Sprite;
    sky->setResAnim(res::ui.getResAnim("bg"));
    sky->attachTo(_view);
    sky->setSize(getStage()->getSize());

    //create play button at center
    spSprite btn = new MyButton;
    btn->setName("play");
    btn->setResAnim(res::ui.getResAnim("play"));
    btn->setAnchor(0.5f, 0.5f);
    btn->setPosition(_view->getSize() / 2);
    btn->attachTo(_view);

    //handle click to button
    btn->addEventListener(TouchEvent::CLICK, CLOSURE(this, &MainMenuScene::onEvent));


    //create menu button
    btn = new MyButton;
    btn->setName("exit");
    btn->setResAnim(res::ui.getResAnim("menu"));
    btn->setAnchor(0.5f, 0.5f);
    btn->attachTo(_view);

    //align it to top right
    btn->setX(_view->getWidth() - btn->getWidth() / 2);
    btn->setY(btn->getHeight() / 2);

    btn->addEventListener(TouchEvent::CLICK, CLOSURE(this, &MainMenuScene::onEvent));

	addBackHandler(getFinish());
}

void MainMenuScene::onEvent(Event* ev)
{
    string id = ev->currentTarget->getName();
    if (id == "exit")
    {
        //request quit
        core::requestQuit();
    }

    if (id == "play")
    {
        //clicked to play button
        //change scene
        GameScene::instance->init();
        flow::show(GameScene::instance);
    }
}