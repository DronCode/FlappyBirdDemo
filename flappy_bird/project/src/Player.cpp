#include "Player.h"
#include "GlobalSettings.h"
#include "box2d/box2d.h"
#include "res.h"

Player::Player(b2World* world)
    : PhysicsBasedActor(world, oxygine::Vector2(0.f, 0.f), 1.f, b2_dynamicBody)
{}

void Player::init()
{
    // Visual
    _view = new oxygine::Sprite;
    _view->setResAnim(res::ui.getResAnim("player"));
    _view->attachTo(this);
    _view->setPosition(0.f, 0.f);
    _view->setAnchorInPixels(0.f, 0.f);

    setSize(_view->getSize());

    setupPhysicsBodyProperties();
}

void Player::update(const oxygine::UpdateState& us)
{
    // Update actor
    Actor::update(us);

    // Update state
    updateInternalState(us);
}

void Player::die() {
    if constexpr (!Settings::kGodMode) {
        if (isDead()) { return; }
        switchToState(PlayerState::DEAD);
    }
}

void Player::sendImpulse() {
    if (_state == PlayerState::IDLE) {
        switchToState(PlayerState::FALLING);
        getBody()->SetAwake(true);
    }
    else if (_state == PlayerState::FALLING) {
        switchToState(PlayerState::JUMPING);

        // Send physics impulse to the body
        const float force = -getJumpForce();
        const b2Vec2 kJumpImpulseVec = b2Vec2 { 0.f, force };

        getBody()->ApplyLinearImpulseToCenter(kJumpImpulseVec, true);

        // Close flag
        _shouldUpdateImpulseTimeOnNextUpdate = true;
    }
}

float Player::getJumpForce() const
{
    return ((_state == PlayerState::FALLING ? Player::kImpulseFromFallingFactor : Player::kImpulseFromJumpingFactor) * Player::kJumpImpulseFactor * Player::kMass) + Settings::kGravity;
}

void Player::addCoin() {
    ++_coins;
}

void Player::joinToObstacleSafeZone() {
    assert(_inObstacleSafeZone == false);
    if (!_inObstacleSafeZone) {
        _inObstacleSafeZone = true;
    }
}

void Player::leaveFromObstacleSafeZone() {
    assert(_inObstacleSafeZone == true);
    if (_inObstacleSafeZone) {
        _inObstacleSafeZone = false;
        ++_score;
    }
}

void Player::setupPhysicsBodyProperties() {
    setMarkerId(Player::kBodyName);

    getBody()->SetAwake(false);
    applyTransform(getPosition(), getRotation());

    updatePhysicsSize();

    b2MassData massData;
    getBody()->GetMassData(&massData);
    massData.mass = Player::kMass;
    getBody()->SetMassData(&massData);
}

void Player::updateInternalState(const oxygine::UpdateState& us) {
    if (isDead()) { return; }

    if (_shouldUpdateImpulseTimeOnNextUpdate && _state == PlayerState::JUMPING) {
        _lastImpulseTime = us.time;
        _shouldUpdateImpulseTimeOnNextUpdate = false;
        return; //Do not update anything else here
    }

    if (us.time - _lastImpulseTime >= kImpulseCooldownMs) {
        if (_state == PlayerState::JUMPING) {
            switchToState(PlayerState::FALLING);
        }
    }
}

void Player::switchToState(PlayerState state) {
    switch (state)
    {
        case PlayerState::IDLE: return; // No way to join into idle from any other state
        case PlayerState::JUMPING:
            if (_state == PlayerState::IDLE || _state == PlayerState::FALLING) { // We can jump only if we in IDLE or FALLING
                _state = state;
            }
            break;
        case PlayerState::FALLING:
            if (_state == PlayerState::IDLE || _state == PlayerState::JUMPING) { //We can start falling only if we jumping
                _state = state;
            }
            break;
        case PlayerState::DEAD:
            if (_state != PlayerState::DEAD && _state != PlayerState::IDLE) { // We can die only if we are not already dead and not in safe IDLE state
                _state = state;
            }
            break;
    }
}