#pragma once

#include <string>
#include "oxygine-framework.h"

struct PhyData
{
    static constexpr const char* kNoName = "#NONAME";
    static constexpr uint32_t kMAGIC = 0xDEADBEEF;

    PhyData()
        : MAGIC(kMAGIC)
        , BodyName(kNoName)
        , ActorInstance(nullptr)
        , ScheduledToBeDestroyed(false)
    {
    }

    uint32_t MAGIC;
    std::string BodyName;
    oxygine::Actor* ActorInstance;
    bool ScheduledToBeDestroyed;

    static bool isPhyDataValid(PhyData* pd) {
        return pd && pd->MAGIC == PhyData::kMAGIC;
    }
};