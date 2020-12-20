#pragma once

#include "oxygine-framework.h"
#include "box2d/b2_world.h"

DECLARE_SMART(Obstacle, spObstacle);
DECLARE_SMART(SafeAreaTrigger, spSafeAreaTrigger);

class ObstacleGroup : public oxygine::Actor
{
public:
    ObstacleGroup();

    void init(b2World* world);

    void reset();
    void generateRandomArea();

private:
    spObstacle              _topObstacle        { nullptr };
    spObstacle              _bottomObstacle     { nullptr };
    spSafeAreaTrigger       _safeAreaTrigger    { nullptr };

    oxygine::spBox9Sprite   _topView            { nullptr };
    oxygine::spBox9Sprite   _bottomView         { nullptr };
};