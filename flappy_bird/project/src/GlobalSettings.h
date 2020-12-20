#pragma once

namespace Settings {
    /**
     * Special literal for conversion between MPS (meters per seconds) to PPS (pixels per seconds)
     * @param arg
     * @return
     */
    constexpr float operator ""_PPS(long double arg) {
        return 1.f / static_cast<float>(arg);
    }

    static constexpr float  kScale                      = 100.f;
    static constexpr float  kGravity                    = 9.81f;
    static constexpr bool   kGodMode                    = false;

    static constexpr int    kMaxObstacleGroupsInPool    = 15;
    static constexpr int    kMaxObstacleGroupsOnScreen  = 7;

    static constexpr float  kMinSafeAreaSpacePercent    = .35f; //min safe area space percent
    static constexpr float  kMaxSafeAreaSpacePercent    = .60f; //max safe area space percent
    static constexpr float  kMinObstacleSpacePercent    = .10f; //min top obstacle size percent (without safe area allocated space)
    static constexpr float  kMaxObstacleSpacePercent    = .75f; //max top obstacle size percent (without safe area allocated space)

    static constexpr int    kMinObstacleWidth           = 45;   //min obstacle width in pixels
    static constexpr int    kMaxObstacleWidth           = 75;   //max obstacle width in pixels

    static constexpr float  kLockAreaHeight             = 20.f;

    static constexpr int    kTimescaleGradeDelta        = 3;      //Each 3 passed obstacles will increment timescale by kTimescaleGradeUpStep
    static constexpr float  kTimescaleGradeUpStep       = 0.022f;

    static constexpr float kDefaultObstacleMovementSpeed = 2.5_PPS;
    static constexpr float kMinDistanceToNextSpawn = static_cast<float>(kMaxObstacleWidth) * 2.f;
    static constexpr float kMaxDistanceToNextSpawn = static_cast<float>(kMaxObstacleWidth) * 2.5f;

    static_assert(kMaxObstacleGroupsInPool > kMaxObstacleGroupsOnScreen, "Max objects on screen must be less to the max objects in pool");
}