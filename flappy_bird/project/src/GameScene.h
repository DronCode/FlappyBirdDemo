#pragma once

#include "MyScene.h"
#include "Game.h"

DECLARE_SMART(GameScene, spGameScene);

class GameScene: public MyScene
{
public:
    static spGameScene instance;
    GameScene();

    void init();

private:
    void onKeyEvent(Event* ev);
    int _leaveFromSceneEventId;

    spGame _game;
};
