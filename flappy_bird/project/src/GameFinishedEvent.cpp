#include "GameFinishedEvent.h"

GameFinishedEvent::GameFinishedEvent(int _score, int _topScore)
    : oxygine::Event(GAME_FINISHED_EVENT)
    , score(_score)
    , topScore(_topScore)
{
}