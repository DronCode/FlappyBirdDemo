#pragma once
#include "MyScene.h"

DECLARE_SMART(MainMenuScene, spMainMenuScene);
class MainMenuScene: public MyScene
{
public:
    static spMainMenuScene instance;

    MainMenuScene();

    void init();
    void setAutoJoinToGameSession(bool flag);
    bool isAutoJoinToGameSessionEnabled() const;

private:
    void onEvent(Event* ev);
    void onGameSessionFinishedEvent(Event* ev);

private:
    bool _autoJoinToGameSession { false };
};