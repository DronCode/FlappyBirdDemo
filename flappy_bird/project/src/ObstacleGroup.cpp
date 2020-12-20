#include "ObstacleGroup.h"

#include "Obstacle.h"
#include "SafeAreaTrigger.h"
#include "GlobalSettings.h"
#include "Random.hpp"

#include "res.h"

ObstacleGroup::ObstacleGroup() : oxygine::Actor() {
}

void ObstacleGroup::init(b2World* world)
{
    // DESTROY
    if (_topObstacle) {
        _topObstacle->detach();
        _topObstacle = nullptr;
    }
    if (_topView) {
        _topView->detach();
        _topView = nullptr;
    }
    if (_bottomObstacle) {
        _bottomObstacle->detach();
        _bottomObstacle = nullptr;
    }
    if (_bottomView) {
        _bottomView->detach();
        _bottomView = nullptr;
    }
    if (_safeAreaTrigger) {
        _safeAreaTrigger->detach();
        _safeAreaTrigger = nullptr;
    }
    // SETUP
    _topObstacle = new Obstacle(world);
    _topObstacle->attachTo(this);
    _topObstacle->

//    if (!_topObstacle) {
//        _topObstacle = new Obstacle(world);
//        _topObstacle->attachTo(this);
//        _topView = new oxygine::Box9Sprite();
//        _topView->setResAnim(res::ui.getResAnim("obstacle"));
//        _topView->setGuides(30.f, 30.f, 30.f, 30.f);
//        _topView->attachTo(this);
//    }
//
//    if (!_bottomObstacle) {
//        _bottomObstacle = new Obstacle(world);
//        _bottomObstacle->attachTo(this);
//        _bottomView = new oxygine::Box9Sprite();
//        _bottomView->setResAnim(res::ui.getResAnim("obstacle_bottom"));
//        _bottomView->setGuides(30.f, 30.f, 30.f, 30.f);
//        _bottomView->attachTo(this);
//    }
//
//    if (!_safeAreaTrigger) {
//        _safeAreaTrigger = new SafeAreaTrigger(world);
//        _safeAreaTrigger->attachTo(this);
//    }

    reset();
}

void ObstacleGroup::reset() {
    generateRandomArea();
}

void ObstacleGroup::generateRandomArea() {
    const float totalHeight = static_cast<float>(oxygine::getStage()->getScaledSize().y);

    const float safeAreaSpacePercent    = Random::getRandomFloat(Settings::kMinSafeAreaSpacePercent, Settings::kMaxSafeAreaSpacePercent);
    const float availableHeightSpace    = totalHeight - (totalHeight * safeAreaSpacePercent); // Free space
    const float topAreaSpacePercent     = Random::getRandomFloat(Settings::kMinObstacleSpacePercent, Settings::kMaxObstacleSpacePercent);
    const float bottomAreaSpacePercent  = 1.f - topAreaSpacePercent;
    const auto  obstacleWidth           = static_cast<float>(Random::getRandomInt(Settings::kMinObstacleWidth, Settings::kMaxObstacleWidth));

    const oxygine::Vector2 topObstacleSize      { obstacleWidth, topAreaSpacePercent    * availableHeightSpace }; //Select from free space (some space was taken by safe area)
    const oxygine::Vector2 bottomObstacleSize   { obstacleWidth, bottomAreaSpacePercent * availableHeightSpace }; //Select from free space (some space was taken by safe area)
    const oxygine::Vector2 safeAreaSize         { obstacleWidth, safeAreaSpacePercent   * totalHeight          };

    setSize(obstacleWidth, totalHeight);

    const oxygine::Vector2 basePosition = getPosition();

    /// Top space
    const oxygine::Vector2 topObstaclePosition = { basePosition.x,basePosition.y + (topObstacleSize.y / 2.f) };
    {
//        _topObstacle->setSize(topObstacleSize);
//        _topView->setSize(topObstacleSize);
//        _topView->setPosition({ topObstaclePosition.x / 2.f, topObstaclePosition.y / 2.f });
//        _topObstacle->setPosition(topObstaclePosition);
//        _topObstacle->init();
    }

    /// Safe space
    const oxygine::Vector2 safeAreaPosition = { basePosition.x, topObstaclePosition.y + (topObstacleSize.y / 2.f) + (safeAreaSize.y / 2.f) };
    {
//        _safeAreaTrigger->setSize(safeAreaSize);
//        _safeAreaTrigger->setPosition(safeAreaPosition);
//        _safeAreaTrigger->init();
    }

    /// Bottom space
    const oxygine::Vector2 bottomObstaclePosition = { basePosition.x,safeAreaPosition.y + (safeAreaSize.y / 2.f) + (bottomObstacleSize.y / 2.f) };
    {
//        _bottomObstacle->setSize(bottomObstacleSize);
//        _bottomView->setSize(bottomObstacleSize);
//        _bottomView->setPosition(bottomObstaclePosition);
//        _bottomObstacle->setPosition(bottomObstaclePosition);
//        _bottomObstacle->init();
    }
}