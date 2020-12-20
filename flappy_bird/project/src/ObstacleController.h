#pragma once

#include "oxygine-framework.h"
#include "box2d/box2d.h"

#include "GlobalSettings.h"

#include <array>

DECLARE_SMART(Obstacle, spObstacle);
DECLARE_SMART(SafeAreaTrigger, spSafeAreaTrigger);

class ObstacleController : public oxygine::Actor
{
    struct ObstacleGroupDefinition {
        spObstacle Top             { nullptr };
        spObstacle Bottom          { nullptr };
        spSafeAreaTrigger SafeArea { nullptr };
        bool Active                { false };
        float Width                { 0.f }; // Do not change manually, it's calculated at generateSizeConstraints, R/O
        float LastMoveTime         { 0.f };

        void reset(b2World* phyWorld, oxygine::Actor* parent);
        void moveTo(float x, float y);
        void resetPosition();
        void generateSizeConstraints();
        oxygine::Vector2 getPosition() const;
    };
public:
    explicit ObstacleController(b2World* phyWorld);

    void init();
    void update(const oxygine::UpdateState& us) override;
    void start();
    void stop();

    void moveObstaclesStage(float currentTime, int& activeObstacles, int& inactiveObstacles);
    void spawnObstaclesStage(int activeObstacles, int inactiveObstacles);

    void incrementPassedObstaclesCount();
    void calculateRequiredDistanceToSpawn();

private:
    static int generateCooldownTimeForNextObstacle();

private:
    // Physics world
    b2World* _phyWorld { nullptr };

    // Spawn control
    bool _isActive { false };
    float _timescale { 1.f };
    int _totalPassedObstaclesCount { 0 };
    int _maxObstacleGroupsOnScreen { Settings::kMaxObstacleGroupsOnScreen };
    float _obstacleMovementSpeed { Settings::kDefaultObstacleMovementSpeed };
    float _requiredDistanceToNextSpawn { Settings::kMinDistanceToNextSpawn };

    std::array<ObstacleGroupDefinition, Settings::kMaxObstacleGroupsInPool> _obstacleGroups;
};