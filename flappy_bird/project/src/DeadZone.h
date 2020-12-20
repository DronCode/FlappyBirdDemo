#pragma once

#include "oxygine-framework.h"
#include "box2d/box2d.h"
#include "PhyData.h"

#include "PhysicsBasedActor.h"

class DeadZone : public PhysicsBasedActor
{
public:
    static constexpr int kHeight = 5;
    static constexpr const char* kDeadZoneName = "#DEADZONE";

    explicit DeadZone(b2World* world);

    void init();
    void update(const oxygine::UpdateState& us) override;
};