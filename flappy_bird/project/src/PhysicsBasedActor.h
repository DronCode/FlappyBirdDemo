#pragma once

#include "oxygine-framework.h"
#include "box2d/box2d.h"
#include "PhyData.h"

class PhysicsBasedActor : public oxygine::Actor
{
public:
    static constexpr const char* kNullMarker = "#NULL_MARKER";

    // Helpers
    static b2Vec2 v2tob2v2(const oxygine::Vector2& v);
    static oxygine::Vector2 b2v2tov2(const b2Vec2& v);

    PhysicsBasedActor(b2World* world, const oxygine::Vector2& position, float scale = 1.f, b2BodyType bodyType = b2_dynamicBody);
    ~PhysicsBasedActor();

    b2Body* getBody() { return _body; }
    virtual void updatePhysicsSize();
    void updatePhysicsPosition();

    void applyTransform(const oxygine::Vector2& v2, float angleRad);

protected:
    void setMarkerId(const std::string& markerId);

protected:
    b2Body* _body { nullptr };
    PhyData* _marker { nullptr };
};