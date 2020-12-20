#include "PhysicsBasedActor.h"
#include "GlobalSettings.h"

b2Vec2 PhysicsBasedActor::v2tob2v2(const oxygine::Vector2& v)
{
    return {v.x / Settings::kScale, v.y / Settings::kScale};
}

oxygine::Vector2 PhysicsBasedActor::b2v2tov2(const b2Vec2& v)
{
    return { v.x * Settings::kScale, v.y * Settings::kScale };
}

PhysicsBasedActor::PhysicsBasedActor(b2World* world, const oxygine::Vector2& position, float scale, b2BodyType bodyType)
    : oxygine::Actor()
{
    _marker = new PhyData();
    _marker->MAGIC = PhyData::kMAGIC;
    _marker->ActorInstance = this;
    _marker->ScheduledToBeDestroyed = false;

    setAnchor(oxygine::Vector2(.5f, .5f));

    b2BodyDef bodyDef;
    bodyDef.type = bodyType;
    bodyDef.position = PhysicsBasedActor::v2tob2v2(position);
    bodyDef.userData.pointer = reinterpret_cast<intptr_t>(_marker); //Assign marker to the physics body

    _body = world->CreateBody(&bodyDef);
    setScale(scale);

    b2PolygonShape shape;
    b2Vec2 sz = PhysicsBasedActor::v2tob2v2(getSize() / 2);
    shape.SetAsBox(sz.x, sz.y);

    _body->CreateFixture(&shape, .0f);

    setMarkerId(PhysicsBasedActor::kNullMarker);
}

PhysicsBasedActor::~PhysicsBasedActor()
{
    if (_marker) {
        //Detach our actor instance from our marker. Out body will be freed after next physics update
        _marker->ActorInstance = nullptr;
        _marker->ScheduledToBeDestroyed = true;
    }
}

void PhysicsBasedActor::updatePhysicsSize()
{
    const b2Vec2 sz = PhysicsBasedActor::v2tob2v2(getSize() / 2);

    if (auto fixture = getBody()->GetFixtureList()) {
        auto shape = fixture->GetShape();
        if (shape->m_type == b2Shape::Type::e_polygon) {
            auto polyShape = reinterpret_cast<b2PolygonShape*>(fixture->GetShape());
            polyShape->SetAsBox(sz.x, sz.y);
        }
    }
}

void PhysicsBasedActor::updatePhysicsPosition()
{
    getBody()->SetTransform(PhysicsBasedActor::v2tob2v2(getPosition()), getRotation());
}

void PhysicsBasedActor::applyTransform(const oxygine::Vector2& v2, float angleRad)
{
    getBody()->SetTransform(PhysicsBasedActor::v2tob2v2(v2), angleRad);
}

void PhysicsBasedActor::setMarkerId(const std::string& markerId)
{
    if (_marker) {
        _marker->BodyName = markerId;
        _marker->ActorInstance = this;
    }
}