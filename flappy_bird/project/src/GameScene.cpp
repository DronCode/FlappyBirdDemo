#include "GameScene.h"
#include "MainMenuScene.h"
#include "MyButton.h"
#include "res.h"
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
    GameResultDialog::instance->setSessionData(event->score, event->topScore);

    _game->fadeOut([this] { onGameFadeOutFinished(); });
}

void GameScene::onGameFadeOutFinished() {
    _game->getClock()->pause();

    flow::show(GameResultDialog::instance, [this](Event* ev) { onGameResultDialogFinished(ev); });
}

void GameScene::onGameResultDialogFinished(Event* ev) {
    const auto targetName = ev->target->getName();

    assert(targetName == GameResultDialog::kExitSession || targetName == GameResultDialog::kTryAgain);

    if (targetName == GameResultDialog::kExitSession || targetName == GameResultDialog::kTryAgain) {
        if (targetName == GameResultDialog::kExitSession) {
            MainMenuScene::instance->setAutoJoinToGameSession(false);
        } else if (targetName == GameResultDialog::kTryAgain) {
            MainMenuScene::instance->setAutoJoinToGameSession(true);
        }

        GameScene::instance->finish();
    }
}