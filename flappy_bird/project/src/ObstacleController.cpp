#include "ObstacleController.h"
#include "SafeAreaTrigger.h"
#include "Obstacle.h"
#include "Random.hpp"

#include "oxygine-framework.h"

ObstacleController::ObstacleController(b2World* phyWorld)
    : oxygine::Actor()
    , _phyWorld(phyWorld)
{}

void ObstacleController::init() {
    const auto stageSize = oxygine::getStage()->getSize();

    setPosition(0.f, 0.f);
    setSize(stageSize);
    setPriority(1);
    setAnchor(0.f, 0.f);

    size_t i = 0;
    for (; i < _obstacleGroups.size(); ++i) {
        auto& obstacleGroup = _obstacleGroups[i];

        obstacleGroup.reset(_phyWorld, this);
        obstacleGroup.resetPosition();
    }

    calculateRequiredDistanceToSpawn();
}

void ObstacleController::update(const oxygine::UpdateState& us) {
    oxygine::Actor::update(us);
    if (!_isActive) { return; }

    const auto stageSize = oxygine::getStage()->getSize();
    const float currentTime = static_cast<float>(us.time) * _timescale;

    int activeObstaclesCount = 0;
    int inactiveObstaclesCount = 0;

    assert(_maxObstacleGroupsOnScreen > 0 && _maxObstacleGroupsOnScreen <= Settings::kMaxObstacleGroupsOnScreen);

    moveObstaclesStage(currentTime, activeObstaclesCount, inactiveObstaclesCount);
    spawnObstaclesStage(activeObstaclesCount, inactiveObstaclesCount);
}

void ObstacleController::start() {
    assert(!_isActive);
    _isActive = true;
}

void ObstacleController::stop() {
    assert(_isActive);
    _isActive = false;
    _totalPassedObstaclesCount = 0;
}

void ObstacleController::moveObstaclesStage(float currentTime, int& activeObstacles, int& inactiveObstacles) {
    const auto stageSize = oxygine::getStage()->getSize();
    // Mover
    int moved = 0;
    for (auto& obstacle : _obstacleGroups) {
        if (!obstacle.Active) {
            ++inactiveObstacles;
            continue;
        }

        ++activeObstacles;

        if (obstacle.LastMoveTime == .0f) {
            obstacle.LastMoveTime = currentTime;
        }

        if (obstacle.getPosition().x + obstacle.Width < .0f) {
            // mark as inactive
            ++inactiveObstacles;
            obstacle.Active = false;
            obstacle.resetPosition();
        } else {
            const float distance = _obstacleMovementSpeed * (currentTime - obstacle.LastMoveTime);
            if (distance > .0f) {
                obstacle.moveTo(obstacle.getPosition().x - distance, 0.f);
                obstacle.LastMoveTime = currentTime;
                ++moved;
            }
        }
    }
    assert(moved <= _maxObstacleGroupsOnScreen);
}

void ObstacleController::spawnObstaclesStage(int activeObstacles, int inactiveObstacles) {
    // Spawner
    const auto stageSize = oxygine::getStage()->getSize();
    if ((inactiveObstacles > 0) && _obstacleGroups.size() > 1 && activeObstacles < _maxObstacleGroupsOnScreen) {
        auto getDistanceBetweenObstacleAndRightSideOfState = [stageSize](const ObstacleGroupDefinition& def) -> float {
            const auto selfPosition = def.getPosition();
            const auto selfOffset = oxygine::Vector2 { .0f /* def.Width */, .0f };
            const auto epPosition = oxygine::Vector2 { stageSize.x, selfPosition.y };
            return (selfPosition + selfOffset).distance(epPosition);
        };

        //1) We need to find obstacle group who is nearest to right side of stage border
        size_t nearestObstacleIndex = 999'999;
        float nearestObstacleDistance = 999'999.f;

        for (size_t i = 0; i < _obstacleGroups.size(); i++) {
            auto& obstacle = _obstacleGroups[i];
            if (!obstacle.Active) {
                continue;
            }

            float dist = getDistanceBetweenObstacleAndRightSideOfState(obstacle);
            if (dist < nearestObstacleDistance) {
                nearestObstacleDistance = dist;
                nearestObstacleIndex = i;
            }
        }

        // If we haven't any active obstacle or obstacle who nearest to the right side of the stage
        // is located further than the required distance to spawn next obstacle
        if (nearestObstacleDistance >= _requiredDistanceToNextSpawn) {
            calculateRequiredDistanceToSpawn(); //calculate new distance to spawn

            //2) Find obstacle to spawn
            for (auto& obstacle : _obstacleGroups) {
                if (!obstacle.Active) {
                    obstacle.Active = true;
                    obstacle.LastMoveTime = .0f;
                    obstacle.generateSizeConstraints();
                    obstacle.resetPosition();
                    return;
                }
            }
        }
    }
}

void ObstacleController::incrementPassedObstaclesCount() {
    ++_totalPassedObstaclesCount;
}


void ObstacleController::calculateRequiredDistanceToSpawn() {
    _requiredDistanceToNextSpawn = Random::getRandomFloat(Settings::kMinDistanceToNextSpawn, Settings::kMaxDistanceToNextSpawn);
}