#pragma once

#include <sol/sol.hpp>
#include <raylib.h>

namespace math
{
    // Enums
    enum NoiseType
    {
        OpenSimplex,
        Perlin
    };

    // Types
    class Vector
    {
    public:
        float x;
        float y;

        std::string type();

        Vector add(Vector other);
        Vector add(float value);
        Vector sub(Vector other);
        Vector sub(float value);
        Vector scale(float scale);
        Vector mul(Vector other);
        Vector div(Vector other);
        Vector normalize();
        Vector rotate(float angle);
        float length();
        float distance(Vector other);
        float dot(Vector other);
        float angle(Vector other);

        // Utility
        Vector2 toVector2();
    };

    // Functions

    // Utility
    sol::table registerMathAPI(sol::state& lua);
    void resetState();

    // API
    float clamp(float value, float min, float max);
    float getNoiseFrequency();
    NoiseType getNoiseType();
    int getNoiseSeed();
    int getRandomSeed();
    float lerp(float from, float to, float amount);
    Vector newVector2(float x, float y);
    float noise(float x, float y);
    float noise(float x, float y, float z);
    float normalize(float value, float from, float to);
    float random();
    int random(int max);
    int random(int min, int max);
    float remap(float value, float inputStart, float inputEnd, float outputStart, float outputEnd);
    void setNoiseFrequency(float frequency);
    void setNoiseType(NoiseType type);
    void setNoiseSeed(int seed);
    void setRandomSeed(int seed);
    float wrap(float value, float min, float max);
}
