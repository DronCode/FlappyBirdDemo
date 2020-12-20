#include "GamePhysicsWorldContactProcessor.h"

#include "box2d/box2d.h"

#include "SafeAreaTrigger.h"
#include "Obstacle.h"
#include "DeadZone.h"
#include "Player.h"

enum class ContactEntityKind {
    CEK_UNKNOWN,                /// Some physics body
    CEK_PLAYER,                 /// Player
    CEK_COIN,                   /// Coin entity (unused)
    CEK_OBSTACLE,               /// Obstacle entity (up or down, it's doesn't matter)
    CEK_OBSTACLE_SAFE_ZONE,     /// Safe zone (sensor)
    CEK_DEADZONE                /// Dead zone (like obstacle but invisible)
};

static ContactEntityKind GetContactEntityKindByEntityName(const std::string& entityName) {
    if (entityName == Player::kBodyName) {
        return ContactEntityKind::CEK_PLAYER;
    }
    if (entityName == DeadZone::kDeadZoneName) {
        return ContactEntityKind::CEK_DEADZONE;
    }
    if (entityName == Obstacle::kObstacleBodyName) {
        return ContactEntityKind::CEK_OBSTACLE;
    }
    if (entityName == SafeAreaTrigger::kBodyName) {
        return ContactEntityKind::CEK_OBSTACLE_SAFE_ZONE;
    }

    return ContactEntityKind::CEK_UNKNOWN;
}

static PhyData* GetPhyDataSafe(std::intptr_t ptr) {
    auto marker = reinterpret_cast<PhyData*>(ptr);
    if (PhyData::isPhyDataValid(marker)) {
        return marker;
    }
    return nullptr;
}

static ContactEntityKind GetContactEntityKindByUnknownPtr(intptr_t ptr)
{
    auto marker = GetPhyDataSafe(ptr);
    if (marker) {
        return GetContactEntityKindByEntityName(marker->BodyName);
    }

    return ContactEntityKind::CEK_UNKNOWN;
}

void GamePhysicsWorldContactProcessor::BeginContact(b2Contact* contact)
{
    auto fixtureA = contact->GetFixtureA();
    auto fixtureB = contact->GetFixtureB();

    ContactEntityKind fixtureAKind = ContactEntityKind::CEK_UNKNOWN;
    ContactEntityKind fixtureBKind = ContactEntityKind::CEK_UNKNOWN;
    Player* playerInstance = nullptr;

    if (fixtureA->GetBody()->GetUserData().pointer) {
        fixtureAKind = GetContactEntityKindByUnknownPtr(fixtureA->GetBody()->GetUserData().pointer);
        if (fixtureAKind == ContactEntityKind::CEK_PLAYER) {
            PhyData* marker = GetPhyDataSafe(fixtureA->GetBody()->GetUserData().pointer);
            if (marker) {
                assert(playerInstance == nullptr);
                playerInstance = reinterpret_cast<Player*>(marker->ActorInstance);
            }
        }
    }

    if (fixtureB->GetBody()->GetUserData().pointer) {
        fixtureBKind = GetContactEntityKindByUnknownPtr(fixtureB->GetBody()->GetUserData().pointer);
        if (fixtureBKind == ContactEntityKind::CEK_PLAYER) {
            PhyData* marker = GetPhyDataSafe(fixtureB->GetBody()->GetUserData().pointer);
            if (marker) {
                assert(playerInstance == nullptr);
                playerInstance = reinterpret_cast<Player*>(marker->ActorInstance);
            }
        }
    }

    if (!playerInstance) {
        return; // Not interested for us in this case
    }

    const ContactEntityKind nonPlayerEntityType = (fixtureAKind == ContactEntityKind::CEK_PLAYER ? fixtureBKind : fixtureAKind);
    static const b2Vec2 kMoveDeadBodyOut { -32.f, -32.f };

    switch (nonPlayerEntityType) {
        case ContactEntityKind::CEK_COIN:
            playerInstance->addCoin();
            break;
        case ContactEntityKind::CEK_DEADZONE:
            playerInstance->die();
            break;
        case ContactEntityKind::CEK_OBSTACLE:
            playerInstance->getBody()->ApplyLinearImpulseToCenter(kMoveDeadBodyOut, true);
            playerInstance->die();
            break;
        case ContactEntityKind::CEK_PLAYER:
            oxygine::logs::messageln("WARNING: COLLISION BETWEEN PLAYER AND PLAYER. IMPOSSIBLE CASE!!!");
            assert(false);
            break;
        case ContactEntityKind::CEK_OBSTACLE_SAFE_ZONE:
            playerInstance->joinToObstacleSafeZone();
            break;
        case ContactEntityKind::CEK_UNKNOWN:
        default:
            break; // unknown body kind
    }
}

void GamePhysicsWorldContactProcessor::EndContact(b2Contact* contact)
{
    auto fixtureA = contact->GetFixtureA();
    auto fixtureB = contact->GetFixtureB();

    ContactEntityKind fixtureAKind = ContactEntityKind::CEK_UNKNOWN;
    ContactEntityKind fixtureBKind = ContactEntityKind::CEK_UNKNOWN;
    Player* playerInstance = nullptr;

    if (fixtureA->GetBody()->GetUserData().pointer) {
        fixtureAKind = GetContactEntityKindByUnknownPtr(fixtureA->GetBody()->GetUserData().pointer);
        if (fixtureAKind == ContactEntityKind::CEK_PLAYER) {
            PhyData* marker = GetPhyDataSafe(fixtureA->GetBody()->GetUserData().pointer);
            if (marker) {
                assert(playerInstance == nullptr);
                playerInstance = reinterpret_cast<Player*>(marker->ActorInstance);
            }
        }
    }

    if (fixtureB->GetBody()->GetUserData().pointer) {
        fixtureBKind = GetContactEntityKindByUnknownPtr(fixtureB->GetBody()->GetUserData().pointer);
        if (fixtureBKind == ContactEntityKind::CEK_PLAYER) {
            PhyData* marker = GetPhyDataSafe(fixtureB->GetBody()->GetUserData().pointer);
            if (marker) {
                assert(playerInstance == nullptr);
                playerInstance = reinterpret_cast<Player*>(marker->ActorInstance);
            }
        }
    }

    if (!playerInstance) {
        return; // Not interested for us in this case
    }

    ContactEntityKind nonPlayerEntityType = (fixtureAKind == ContactEntityKind::CEK_PLAYER ? fixtureBKind : fixtureAKind);

    switch (nonPlayerEntityType) {

        case ContactEntityKind::CEK_OBSTACLE_SAFE_ZONE:
            playerInstance->leaveFromObstacleSafeZone();
            break;
        default:
            break; // unknown body kind
    }
}