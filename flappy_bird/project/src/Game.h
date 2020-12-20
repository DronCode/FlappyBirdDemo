#pragma once

#include "oxygine-framework.h"
#include "box2d/box2d.h"
#include "Box2DDebugDraw.h"
#include "GlobalSettings.h"

#include <list>

using namespace oxygine;

class GamePhysicsWorldContactProcessor;

DECLARE_SMART(ObstacleController, spObstacleController);
DECLARE_SMART(DeadZone, spDeadZone);
DECLARE_SMART(Player, spPlayer);
DECLARE_SMART(Game, spGame);
DECLARE_SMART(PhysicsBasedActor, spPhysicsBasedActor);

class Game: public Actor
{
    enum class GameState {
        IDLE,       ///< Waiting for first jump
        PLAYING,    ///< Jumping, doing some other actions
        FINISHED    ///< Player is dead, show the score
    };

public:
    Game();
    ~Game();

    void init();
    void sendPlayerImpulse();

private:
    void doUpdate(const UpdateState& us);
    void doPhysicsUpdate();
    void updateGameState();
    void switchToState(GameState state);
    void updateScoreLabel();

protected:
    friend class Player;

protected:
    // Entities
    spActor                             _ui;
    spTextField                         _scoreLabel;
    spTextField                         _welcomeLabel;
    spPlayer                            _player;
    spDeadZone                          _deadZone;
    spBox2DDraw                         _phyDebugDraw;
    spObstacleController                _obstacleController;
    spPhysicsBasedActor                 _topLockArea;

    // Physics
    GamePhysicsWorldContactProcessor*   _phyWorldContactProcessor;
    b2Vec2                              _phyGravity { 0.f,  Settings::kGravity };
    b2World*                            _phyWorld { nullptr };

    // Logic
    GameState                           _state { GameState::IDLE };
    int                                 _playerLastScore { 0 };
};