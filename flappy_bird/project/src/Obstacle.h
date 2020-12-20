#pragma once

#include "PhysicsBasedActor.h"
#include "oxygine-framework.h"

class Obstacle : public PhysicsBasedActor
{
public:
    enum class Location {
        Top,        //From top to bottom
        Bottom      //From bottom to top
    };

    static constexpr const char* kObstacleBodyName     = "#OBSTACLE";

    Obstacle(b2World* world, Location location);

    void init();
    void updatePhysicsSize() override;

private:
    oxygine::spBox9Sprite _view { nullptr };
    Obstacle::Location _location { Location::Top };
};