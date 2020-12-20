#include "DeadZone.h"

DeadZone::DeadZone(b2World* world)
    : PhysicsBasedActor(world, oxygine::Vector2(0.f, 0.f), 1.f, b2_staticBody)
{
}

void DeadZone::init()
{
    updatePhysicsSize();

    applyTransform(getPosition(), getRotation());

    setMarkerId(DeadZone::kDeadZoneName);
}

void DeadZone::update(const oxygine::UpdateState& us) {
    // Update actor
    Actor::update(us);
}