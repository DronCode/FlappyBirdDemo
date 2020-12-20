#pragma once

#include <random>

struct Random {
    static float getRandomFloat(float min, float max) {
        std::mt19937 gen(static_cast<unsigned int>(time(nullptr))); // NOLINT(cert-msc51-cpp)
        std::uniform_real_distribution<> distribution(min, max);
        return static_cast<float>(distribution(gen));
    }

    static int getRandomInt(int min, int max) {
        std::mt19937 gen(static_cast<unsigned int>(time(nullptr))); // NOLINT(cert-msc51-cpp)
        std::uniform_int_distribution<> distribution(min, max);
        return distribution(gen);
    }
};