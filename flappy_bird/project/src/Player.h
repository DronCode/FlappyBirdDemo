#pragma once

#include "oxygine-framework.h"
#include "box2d/box2d.h"
#include "PhysicsBasedActor.h"

#include "PhyData.h"

class Player : public PhysicsBasedActor
{
    enum class PlayerState;
public:
    static constexpr const char* kBodyName = "#PLAYER";
    constexpr static float kImpulseCooldownMs        = 1.5f;
    static constexpr float kJumpImpulseFactor        = 1.978f;
    static constexpr float kImpulseFromFallingFactor = 1.76f;
    static constexpr float kImpulseFromJumpingFactor = 0.988f;
    static constexpr float kMass                     = 17.2f;

    explicit Player(b2World* world);

    void init();
    void update(const oxygine::UpdateState& us) override;

    void die();
    bool isDead() const { return _state == PlayerState::DEAD; }
    bool isInIdle() const { return _state == PlayerState::IDLE; }
    void sendImpulse();

    void addCoin();
    void joinToObstacleSafeZone();
    void leaveFromObstacleSafeZone();

    int getScore() const { return _score; }

private:
    void setupPhysicsBodyProperties();
    void updateInternalState(const oxygine::UpdateState& us);
    void switchToState(PlayerState state);
    float getJumpForce() const;

private:
    oxygine::timeMS _lastImpulseTime;
    oxygine::spSprite _view;
    bool _shouldUpdateImpulseTimeOnNextUpdate { false };
    bool _inObstacleSafeZone { false };

    enum class PlayerState {
        IDLE,           // Waiting for first jump
        JUMPING,        // Jumped, waiting for cooldown
        FALLING,        // Falling, waiting for jump or collision
        DEAD            // Dead, collided with something
    };

    PlayerState _state { PlayerState::IDLE };
    int _score { 0 };
    int _coins { 0 };
};