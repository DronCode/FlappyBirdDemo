#include "GameResultDialog.h"
#include "res.h"

#include <cassert>

spGameResultDialog GameResultDialog::instance { nullptr };

GameResultDialog::GameResultDialog() : MyScene()
{
    setName("GameResultDialog");
    _dialog = true;

    const auto stageSize = oxygine::getStage()->getSize();

    // Background
    {
        _background = new oxygine::Sprite();
        _background->setResAnim(res::ui.getResAnim("sky"));
        _background->setPosition(0.f, 0.f);
        _background->attachTo(_view);
    }

    // Result text
    {
        _resultTextField = new oxygine::TextField();
        _resultTextField->setFont(res::ui.getResFont("main"));
        _resultTextField->setFontSize(24);
        _resultTextField->setSize(300.f, 300.f);
        _resultTextField->setPosition((stageSize.x * .5f) - (_resultTextField->getSize().x * .5f), 150);
        _resultTextField->setColor(oxygine::Color::BlueViolet);
        _resultTextField->setHAlign(TextStyle::HorizontalAlign::HALIGN_MIDDLE);
        _resultTextField->attachTo(_view);
    }

    // Buttons (restart)
    {
        _restartButton = createButton("Restart", "generic_button");
        _restartButton->setPosition((stageSize.x * .5f) - (_restartButton->getSize().x * .5f), 350.f);
        _restartButton->attachTo(_view);
        _restartButton->setName(GameResultDialog::kTryAgain);
        _restartButton->addEventListener(oxygine::TouchEvent::TOUCH_UP, getFinish());
    }

    // Buttons (quit)
    {
        _quitButton = createButton("Quit", "generic_button");
        _quitButton->setPosition((stageSize.x * .5f) - (_quitButton->getSize().x * .5f), 450.f);
        _quitButton->attachTo(_view);
        _quitButton->setName(GameResultDialog::kExitSession);
        _quitButton->addEventListener(oxygine::TouchEvent::TOUCH_UP, getFinish());
    }

    addEventListener(EVENT_PRESHOWING, CLOSURE(this, &GameResultDialog::onBeforeShowed));
    addEventListener(EVENT_POSTHIDING, CLOSURE(this, &GameResultDialog::onAfterHidden));
}

void GameResultDialog::setSessionData(int score, int topScore) {
    _score = score;
    _topScore = topScore;
}

void GameResultDialog::onBeforeShowed(Event*) {
    assert(_score != -1);
    assert(_topScore != -1);

    if (_resultTextField) {
        char buffer[256];
        std::snprintf(buffer, 256, "Game finished\n\nYou score is %d\nTop score is %d", _score, _topScore);
        _resultTextField->setText(buffer);
    }

    _resultTextField->setAlpha(0);
    _quitButton->setAlpha(0);
    _restartButton->setAlpha(0);

    _resultTextField->addTween(Actor::TweenAlpha(255), kFadeInDuration);
    _quitButton->addTween(Actor::TweenAlpha(255), kFadeInDuration);
    _restartButton->addTween(Actor::TweenAlpha(255), kFadeInDuration);
}

void GameResultDialog::onAfterHidden(Event*) {
    _score = -1;
    _topScore = -1;
}

spButton GameResultDialog::createButton(const std::string& text, const std::string& background) {
    spButton button = new oxygine::Button();
    button->setPriority(10);
    button->setResAnim(res::ui.getResAnim(background));

    spTextField label = new oxygine::TextField();

    TextStyle style;
    style.font = res::ui.getResFont("main");
    style.color = oxygine::Color::White;
    style.vAlign = TextStyle::VALIGN_MIDDLE;
    style.hAlign = TextStyle::HALIGN_MIDDLE;
    style.multiline = true;
    style.fontSize = 24;

    label->setSize(button->getSize());
    label->setPosition(label->getPosition().x, label->getPosition().y - (.4f * static_cast<float>(style.fontSize)));
    label->setStyle(style);
    label->setText(text);
    label->attachTo(button);

    return button;
}