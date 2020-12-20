#include "ObstacleController.h"
#include "SafeAreaTrigger.h"
#include "Obstacle.h"
#include "Random.hpp"

void ObstacleController::ObstacleGroupDefinition::reset(b2World* phyWorld, oxygine::Actor* parent) {
    if (!Top) {
        Top = new Obstacle(phyWorld, Obstacle::Location::Top);
        Top->attachTo(parent);
    }

    if (!Bottom) {
        Bottom = new Obstacle(phyWorld, Obstacle::Location::Bottom);
        Bottom->attachTo(parent);
    }

    if (!SafeArea) {
        SafeArea = new SafeAreaTrigger(phyWorld);
        SafeArea->attachTo(parent);
    }

    LastMoveTime = .0f;
    Active = false;

    generateSizeConstraints();
    Top->init();
    SafeArea->init();
    Bottom->init();
}

void ObstacleController::ObstacleGroupDefinition::moveTo(float x, float y) {
    assert(Top);
    assert(Bottom);
    assert(SafeArea);

    Top->setPosition(x + (Top->getSize().x * .5f), y + Top->getSize().y * .5f);
    Top->updatePhysicsPosition();

    SafeArea->setPosition(x + (SafeArea->getSize().x * .5f),  Top->getPosition().y + (Top->getSize().y * .5f) + (SafeArea->getSize().y * .5f));
    SafeArea->updatePhysicsPosition();

    Bottom->setPosition(x + (Bottom->getSize().x * .5f), SafeArea->getPosition().y + (SafeArea->getSize().y * .5f) + (Bottom->getSize().y * .5f));
    Bottom->updatePhysicsPosition();
}

void ObstacleController::ObstacleGroupDefinition::resetPosition() {
    const auto stageSize = oxygine::getStage()->getSize();
    moveTo(stageSize.x, .0f);
}

void ObstacleController::ObstacleGroupDefinition::generateSizeConstraints() {
    assert(Top);
    assert(Bottom);
    assert(SafeArea);

    const float totalHeight = static_cast<float>(oxygine::getStage()->getScaledSize().y);

    const float safeAreaSpacePercent    = Random::getRandomFloat(Settings::kMinSafeAreaSpacePercent, Settings::kMaxSafeAreaSpacePercent);
    const float availableHeightSpace    = totalHeight - (totalHeight * safeAreaSpacePercent); // Free space
    const float topAreaSpacePercent     = Random::getRandomFloat(Settings::kMinObstacleSpacePercent, Settings::kMaxObstacleSpacePercent);
    const float bottomAreaSpacePercent  = 1.f - topAreaSpacePercent;
    const auto  obstacleWidth           = static_cast<float>(Random::getRandomInt(Settings::kMinObstacleWidth, Settings::kMaxObstacleWidth));

    const oxygine::Vector2 topObstacleSize      { obstacleWidth, topAreaSpacePercent    * availableHeightSpace }; //Select from free space (some space was taken by safe area)
    const oxygine::Vector2 bottomObstacleSize   { obstacleWidth, bottomAreaSpacePercent * availableHeightSpace }; //Select from free space (some space was taken by safe area)
    const oxygine::Vector2 safeAreaSize         { obstacleWidth, safeAreaSpacePercent   * totalHeight          };

    SafeArea->setSize(safeAreaSize);
    SafeArea->updatePhysicsSize();

    Top->setSize(topObstacleSize);
    Top->updatePhysicsSize();

    Bottom->setSize(bottomObstacleSize);
    Bottom->updatePhysicsSize();

    Width = obstacleWidth;

    moveTo(0.f, 0.f);
}

oxygine::Vector2 ObstacleController::ObstacleGroupDefinition::getPosition() const {
    assert(Top);

    const auto topSize = Top->getSize();
    const auto topPosition = Top->getPosition();

    return { topPosition.x - (.5f * topSize.x), topPosition.y - (.5f * topSize.y) };
}