#include "GameplaySession.h"

void GameplaySession::incScore() {
    ++_score;
    if (_score > _topScore) {
        _topScore = _score;
    }
}

void GameplaySession::resetScore() {
    _score = 0;
}

GameplaySession& GameplaySession::getSession() {
    static GameplaySession session;
    return session;
}