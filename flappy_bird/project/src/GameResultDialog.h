#pragma once

#include "oxygine-flow.h"
#include "oxygine-framework.h"
#include "MyScene.h"

DECLARE_SMART(GameResultDialog, spGameResultDialog)

class GameResultDialog : public MyScene
{
private:
    static constexpr int kFadeInDuration = 400;
public:
    static spGameResultDialog instance;
    static constexpr const char* kExitSession = "EXIT_SESSION";
    static constexpr const char* kTryAgain = "TRY_AGAIN";

    GameResultDialog();

    void setSessionData(int score, int topScore);

private:
    void onBeforeShowed(Event* ev);
    void onAfterHidden(Event* ev);

    static spButton createButton(const std::string& text, const std::string& background);

private:
    oxygine::spSprite       _background { nullptr };
    oxygine::spTextField    _resultTextField { nullptr };
    oxygine::spButton       _restartButton { nullptr };
    oxygine::spButton       _quitButton { nullptr };

    int _score { -1 };
    int _topScore { -1 };
};