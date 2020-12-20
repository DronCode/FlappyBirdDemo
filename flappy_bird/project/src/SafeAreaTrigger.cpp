#include "SafeAreaTrigger.h"
#include <cassert>

SafeAreaTrigger::SafeAreaTrigger(b2World* world) : PhysicsBasedActor(world, oxygine::Vector2(0.f, 0.f), 1.f, b2_staticBody)
{
}

void SafeAreaTrigger::init()
{
    auto fixture = getBody()->GetFixtureList();
    assert(fixture);
    if (fixture) {
        fixture->SetSensor(true);
    }

    applyTransform(getPosition(), getRotation());
    updatePhysicsSize();

    setMarkerId(SafeAreaTrigger::kBodyName);
}