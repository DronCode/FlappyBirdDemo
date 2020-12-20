#pragma once

#include "PhysicsBasedActor.h"

class SafeAreaTrigger : public PhysicsBasedActor
{
public:
    static constexpr const char* kBodyName = "#SAFEAREA";

    explicit SafeAreaTrigger(b2World* world);

    void init();
};