#include "Game.h"

#include "res.h"

#include "Player.h"
#include "PhyData.h"
#include "DeadZone.h"
#include "GlobalSettings.h"
#include "ObstacleController.h"
#include "GamePhysicsWorldContactProcessor.h"
#include "GameplaySession.h"

#include "SafeAreaTrigger.h" //TODO: Remove after debug
#include "Obstacle.h" //TODO: Remove after debug

constexpr int kVelocityIterations = 6;
constexpr int kPositionIterations = 2;

Game::Game() {
}

Game::~Game() {
    delete _phyWorldContactProcessor;
    _phyWorldContactProcessor = nullptr;

    if (_phyDebugDraw) {
        _phyDebugDraw->detach();
        _phyDebugDraw = nullptr;
    }

    delete _phyWorld;
    _phyWorld = nullptr;
}

void Game::init()
{
    _phyWorld = new b2World { _phyGravity };

    _phyWorldContactProcessor = new GamePhysicsWorldContactProcessor();
    _phyWorld->SetContactListener(_phyWorldContactProcessor);

    //TODO: Show only by request?
    _phyDebugDraw = new Box2DDraw();
    _phyDebugDraw->SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit | b2Draw::e_pairBit | b2Draw::e_centerOfMassBit);
    _phyDebugDraw->attachTo(this);
    _phyDebugDraw->setWorld(Settings::kScale, _phyWorld);
    _phyDebugDraw->setPriority(1);

    //scene layer would have size of display
    const auto stageSize = getStage()->getSize();
    setSize(stageSize);

    //create background
    {
        spSprite sky = new Sprite;
        sky->setResAnim(res::ui.getResAnim("sky"));
        sky->attachTo(this);
        sky->setSize(getStage()->getSize());
    }

    //create separate layer for elements virtual joystick and other UI in future
    {
        _ui = new Actor;
        _ui->attachTo(this);
        //it would be higher than other actors with default priority = 0
        _ui->setPriority(1);
    }

    //create score label
    {
        _scoreLabel = new TextField();
        _scoreLabel->setFont(res::ui.getResFont("main"));
        _scoreLabel->setPriority(4);
        _scoreLabel->setFontSize(32);
        _scoreLabel->setColor(Color::DarkBlue);
        _scoreLabel->setText("Your score\n0");
        _scoreLabel->setAlign(TextStyle::VALIGN_DEFAULT, TextStyle::HALIGN_MIDDLE);
        _scoreLabel->setSize(150.f, static_cast<float>(_scoreLabel->getFontSize()) + 10.f);
        _scoreLabel->setPosition((.5f * stageSize.x) - (.5f * _scoreLabel->getSize().x), 24.f);
    }

    //create welcome label
    {
        _welcomeLabel = new TextField();
        _welcomeLabel->attachTo(this);
        _welcomeLabel->setPriority(4);
        _welcomeLabel->setFont(res::ui.getResFont("main"));
        _welcomeLabel->setFontSize(32);
        _welcomeLabel->setColor(Color::Green);
        _welcomeLabel->setText("Press (space) to start your challenge");
        _welcomeLabel->setAlign(TextStyle::VALIGN_DEFAULT, TextStyle::HALIGN_MIDDLE);
        _welcomeLabel->setSize(150.f, static_cast<float>(_scoreLabel->getFontSize()) + 10.f);
        _welcomeLabel->setPosition((.5f * stageSize.x) - (.5f * _scoreLabel->getSize().x), (stageSize.y * .70f) - (static_cast<float>(_scoreLabel->getFontSize()) * .5f));
    }

    const auto displaySize = core::getDisplaySize();

    // PLAYER
    {
        _player = new Player(_phyWorld);
        _player->attachTo(this);
        _player->setPriority(2);
        const float x = (core::getDisplaySize().x / 2.f) * 0.25f;
        const float y = (core::getDisplaySize().y / 2.f);
        _player->setPosition(x, y);
        _player->init();
    }

    // DEAD ZONE
    {
        _deadZone = new DeadZone(_phyWorld);
        _deadZone->attachTo(this);
        _deadZone->setPriority(1);
        _deadZone->setPosition(displaySize.x / 2.f, static_cast<float>(displaySize.y - DeadZone::kHeight));
        _deadZone->setSize(static_cast<float>(displaySize.x), static_cast<float>(DeadZone::kHeight));
        _deadZone->init();
    }

    // LOCK AREA (Prohibit to fly to our of area)
    {
        _topLockArea = new PhysicsBasedActor(_phyWorld, { 0.f, 0.f }, 1.f, b2_staticBody);
        _topLockArea->attachTo(this);
        _topLockArea->setPriority(1);
        _topLockArea->setPosition(displaySize.x / 2.f, -(Settings::kLockAreaHeight / 2.f));
        _topLockArea->setSize(static_cast<float>(displaySize.x), Settings::kLockAreaHeight);
        _topLockArea->updatePhysicsSize();
        _topLockArea->applyTransform(_topLockArea->getPosition(), _topLockArea->getRotation());
    }

    // OBSTACLE CONTROLLER
    {
        _obstacleController = new ObstacleController(_phyWorld);
        _obstacleController->attachTo(this);
        _obstacleController->init();
    }
}

void Game::doUpdate(const UpdateState& us)
{
    doPhysicsUpdate();
    updateGameState();
}

void Game::doPhysicsUpdate()
{
    // Simulate physics worlds
    constexpr float fixedWorldIterationStep = 1.f / 60.f;
    _phyWorld->Step(fixedWorldIterationStep, kVelocityIterations, kPositionIterations);

    // Update each body
    {
        b2Body* body = _phyWorld->GetBodyList();
        while (body) {
            b2Body* next = body->GetNext();
            auto marker = reinterpret_cast<PhyData*>(body->GetUserData().pointer);
            if (PhyData::isPhyDataValid(marker)) //Check that pointer is valid by magic bytes
            {
                auto actor = marker->ActorInstance;
                if (actor) {
                    const b2Vec2& pos = body->GetPosition();
                    actor->setPosition(PhysicsBasedActor::b2v2tov2(pos));
                    actor->setRotation(body->GetAngle());
                }

                if (marker->ScheduledToBeDestroyed) //If our physics actor was marked to be destroyed or destroyed yet
                {
                    body->GetUserData().pointer = 0; // We should remove pointer to marker
                    _phyWorld->DestroyBody(body); // Destroy the body

                    if (actor) { //If actor exists
                        actor->detach(); //Detach it
                    }

                    delete marker; //Release the marker
                    marker = nullptr;
                }
            }
            body = next;
        }
    }
}

void Game::updateGameState()
{
    const bool isPlayerDead = _player->isDead();
    const bool isGameFinished = _state == GameState::FINISHED;

    if (isPlayerDead && !isGameFinished) {
        switchToState(GameState::FINISHED);
    }

    if (!isPlayerDead && !isGameFinished) {
        if (_player->getScore() != _playerLastScore) {
            const int dt = _player->getScore() - _playerLastScore;
            assert(dt > 0);
            if (dt > 0) {
                _playerLastScore = _player->getScore(); // Sync

                // If we missed more than 1 obstacle
                for (int i = 0; i < dt; i++) {
                    GameplaySession::getSession().incScore();
                    _obstacleController->incrementPassedObstaclesCount();
                }
            }
            updateScoreLabel();
        }
    }
}

void Game::updateScoreLabel() {
    const int score = GameplaySession::getSession().getScore();
    static constexpr size_t kBufferSize = 24;
    char buffer[kBufferSize];
    std::snprintf(buffer, kBufferSize, "Your score\n%d", score);
    _scoreLabel->setText(buffer);
}

void Game::switchToState(GameState state) {
    if (state != _state) {
        switch (state) {
            case GameState::IDLE:
                assert(false); //Unable to jump info IDLE state, game is already in IDLE state after start
                break;
            case GameState::PLAYING:
                //TODO: Show label with game score and hide label with text "Press (space) to start challenge"
                _welcomeLabel->detach();
                _scoreLabel->attachTo(this);
                GameplaySession::getSession().resetScore();
                _obstacleController->start();
                break;
            case GameState::FINISHED:
                _scoreLabel->detach();
                _obstacleController->stop();
                break;
        }

        _state = state;
    }
}

void Game::sendPlayerImpulse() {
    if (_state != GameState::FINISHED)
    {
        _player->sendImpulse();
        if (_state == GameState::IDLE) {
            switchToState(GameState::PLAYING);
        }
    }
}