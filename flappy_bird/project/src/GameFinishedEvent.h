#pragma once

#include "oxygine-framework.h"

class GameFinishedEvent : public oxygine::Event
{
public:
    enum {
        GAME_FINISHED_EVENT = eventID('G', 'E', 'G', 'F')
    };

    int score;
    int topScore;

    GameFinishedEvent(int _score, int _topScore);
};