#include "Obstacle.h"
#include "res.h"

Obstacle::Obstacle(b2World* world, Location location)
    : PhysicsBasedActor(world, oxygine::Vector2(0.f, 0.f), 1.f, b2_staticBody)
    , _location(location)
{
}

void Obstacle::init() {
    if (!_view) {
        _view = new oxygine::Box9Sprite();
    }

    _view->setResAnim(res::ui.getResAnim((_location == Obstacle::Location::Top ? "obstacle" : "obstacle_bottom")));
    _view->attachTo(this);
    _view->setGuides(30.f, 30.f, 30.f, 30.f);
    _view->setSize(getSize());

    setMarkerId(Obstacle::kObstacleBodyName);

    const auto pos = getPosition();

    applyTransform(pos, getRotation());
    updatePhysicsSize();
}

void Obstacle::updatePhysicsSize() {
    PhysicsBasedActor::updatePhysicsSize();
    if (_view) {
        _view->setSize(getSize());
    }
}