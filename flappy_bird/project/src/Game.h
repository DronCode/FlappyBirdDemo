#pragma once

#include "oxygine-framework.h"
#include "box2d/box2d.h"
#include "GlobalSettings.h"

#ifdef OX_DEBUG
#include "Box2DDebugDraw.h"
#endif

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
    void fadeOut(const std::function<void()>& onFadeOutCompleted);
    void sendPlayerImpulse();

private:
    void doUpdate(const UpdateState& us);
    void doPhysicsUpdate();
    void updateGameState();
    void switchToState(GameState state);
    void updateScoreLabel();

#ifdef OX_DEBUG
    void updateDebugGameState();
#endif

protected:
    friend class Player;

protected:
    // Entities
    spSprite                            _sky;
    spActor                             _ui;
    spTextField                         _scoreLabel;
    spTextField                         _welcomeLabel;
    spPlayer                            _player;
    spDeadZone                          _deadZone;
    spObstacleController                _obstacleController;
    spPhysicsBasedActor                 _topLockArea;
    spTween                             _tweens;

    // Physics
#ifdef OX_DEBUG
    spBox2DDraw                         _phyDebugDraw;
#endif
    GamePhysicsWorldContactProcessor*   _phyWorldContactProcessor { nullptr };
    b2Vec2                              _phyGravity { 0.f,  Settings::kGravity };
    b2World*                            _phyWorld { nullptr };

    // Logic
    GameState                           _state { GameState::IDLE };
    int                                 _playerLastScore { 0 };
};