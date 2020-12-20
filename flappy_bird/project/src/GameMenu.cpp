#include "GameMenu.h"
#include "MainMenuScene.h"
#include "MyButton.h"
#include "res.h"

spGameMenu GameMenu::instance;

GameMenu::GameMenu()
{
    setName("GameMenu");
	_dialog = true;

    //create holder for button and text
    _buttons = new Actor;
    _buttons->attachTo(_view);

    float y = _view->getHeight() / 2 - 80;

    //initialize TextStyle for TextField
    //TextStyle is plain struct with "setting" for Text
    TextStyle style;
	style.font = res::ui.getResFont("main");
    //vertical align
    style.vAlign = TextStyle::VALIGN_BOTTOM;
    //horizontal align
    style.hAlign = TextStyle::HALIGN_MIDDLE;
	style.fontSize = 71;

    spTextField paused = new TextField;
    paused->setStyle(style);
    //colored text by "html" tags
    paused->setHtmlText("Paused<div c='00FF00'>!</div>");
    paused->setX(_view->getWidth() / 2);
    paused->setY(y);
    paused->attachTo(_buttons);
    paused->setAlpha(128);

    //animate with infinity loops "Paused!" text
    paused->addTween(Actor::TweenAlpha(255), 600, -1, true);
    paused->addTween(Actor::TweenScale(1.05f), 600, -1, true);

    y += 70;

	addEventListener(EVENT_PRE_SHOWING, CLOSURE(this, &GameMenu::preShowing));
}

void GameMenu::preShowing(Event*)
{
    //before showing dialog hide buttons
    _buttons->setAlpha(0);

    //animate background
    //set default small size
    _bg->setSize(_bg->getAnimFrame().getSize());

    //and modify height and then width by tween
    spTweenQueue tq = new TweenQueue;
    tq->add(Actor::TweenHeight(300), 500);
    tq->add(Actor::TweenWidth(300), 500);
    //and show buttons when done
    tq->addDoneCallback(CLOSURE(this, &GameMenu::showButtons));
    _bg->addTween(tq);
}

void GameMenu::showButtons(Event* ev)
{
    //tween activated from GameMenu::_show is done
    //fade in buttons and text
    _buttons->addTween(Actor::TweenAlpha(255), 300);
}