#include "GameScene.h"
#include "MainMenuScene.h"
#include "MyButton.h"
#include "res.h"
#include "GameMenu.h"
#include "GameResultDialog.h"
#include "GameFinishedEvent.h"

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

    _game->addEventListener(GameFinishedEvent::GAME_FINISHED_EVENT, std::bind(&GameScene::onGameFinished, this, std::placeholders::_1));
    _leaveFromSceneEventId = getStage()->addEventListener(KeyEvent::KEY_UP, std::bind(&GameScene::onKeyEvent, this, std::placeholders::_1));
}

void GameScene::onKeyEvent(Event* ev)
{
    auto keyEvent = reinterpret_cast<KeyEvent*>(ev);
    switch (keyEvent->data->keysym.scancode) {
        case SDL_SCANCODE_SPACE:
            _game->sendPlayerImpulse();
            break;
        default:
            break;
    }
}

void GameScene::onGameFinished(Event* ev) {
    auto event = reinterpret_cast<GameFinishedEvent*>(ev);

    _game->getClock()->pause();

    GameResultDialog::instance->setSessionData(event->score, event->topScore);

    flow::show(GameResultDialog::instance, [=](Event* ev) {
        _game->getClock()->resume();

        if (ev->target->getName() == GameResultDialog::kExitSession) {
            finish();
        }
        else if (ev->target->getName() == GameResultDialog::kTryAgain) {
            releaseGameInstance();
            init();
        }
    });
}

void GameScene::releaseGameInstance() {
    _game->removeEventListener(GameFinishedEvent::GAME_FINISHED_EVENT);
    _game->detach();
    _game = nullptr;
}