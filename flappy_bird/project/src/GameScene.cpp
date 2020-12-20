#include "GameScene.h"
#include "MainMenuScene.h"
#include "MyButton.h"
#include "res.h"
#include "GameMenu.h"

spGameScene GameScene::instance;

GameScene::GameScene()
{
    setName("GameScene");
}

void GameScene::init()
{
    _game = new Game;
    _game->init();
    _game->attachTo(_view);
    _game->setClock(new Clock);

    _leaveFromSceneEventId = getStage()->addEventListener(KeyEvent::KEY_UP, std::bind(&GameScene::onKeyEvent, this, std::placeholders::_1));
}

void GameScene::onKeyEvent(Event* ev)
{
    auto keyEvent = reinterpret_cast<KeyEvent*>(ev);
    switch (keyEvent->data->keysym.scancode) {
        case SDL_SCANCODE_ESCAPE:
            _game->detach();
            _game = nullptr;
            getStage()->removeEventListener(_leaveFromSceneEventId);
            Scene::finish();
            break;
        case SDL_SCANCODE_SPACE:
            _game->sendPlayerImpulse();
            break;
        default:
            break;
    }
}