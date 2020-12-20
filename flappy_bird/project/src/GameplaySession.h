#pragma once

class GameplaySession
{
private:
    int _score { 0 };
    int _topScore { 0 };

    GameplaySession() = default;
public:
    GameplaySession(const GameplaySession&)             = delete;
    GameplaySession(GameplaySession&&)                  = delete;
    GameplaySession& operator=(const GameplaySession&)  = delete;
    GameplaySession& operator=(GameplaySession&&)       = delete;

    void incScore();
    int getScore() const { return _score; }
    void resetScore();
    int getTopScore() const { return _topScore; }

    static GameplaySession& getSession();
};