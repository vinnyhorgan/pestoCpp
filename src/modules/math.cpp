#include "math.h"

#include <raymath.h>
#include <FastNoiseLite.h>

FastNoiseLite fastnoiselite;
math::NoiseType noiseType = math::NoiseType::OpenSimplex;
int noiseSeed = 1337;
float noiseFrequency = 0.01f;
int randomSeed = 0;

namespace math
{
    // Types
    std::string Vector::type()
    {
        return "Vector";
    }

    Vector Vector::add(Vector other)
    {
        Vector2 result = Vector2Add(toVector2(), other.toVector2());
        return Vector{result.x, result.y};
    }

    Vector Vector::add(float value)
    {
        Vector2 result = Vector2AddValue(toVector2(), value);
        return Vector{result.x, result.y};
    }

    Vector Vector::sub(Vector other)
    {
        Vector2 result = Vector2Subtract(toVector2(), other.toVector2());
        return Vector{result.x, result.y};
    }

    Vector Vector::sub(float value)
    {
        Vector2 result = Vector2SubtractValue(toVector2(), value);
        return Vector{result.x, result.y};
    }

    Vector Vector::scale(float scale)
    {
        Vector2 result = Vector2Scale(toVector2(), scale);
        return Vector{result.x, result.y};
    }

    Vector Vector::mul(Vector other)
    {
        Vector2 result = Vector2Multiply(toVector2(), other.toVector2());
        return Vector{result.x, result.y};
    }

    Vector Vector::div(Vector other)
    {
        Vector2 result = Vector2Divide(toVector2(), other.toVector2());
        return Vector{result.x, result.y};
    }

    Vector Vector::normalize()
    {
        Vector2 result = Vector2Normalize(toVector2());
        return Vector{result.x, result.y};
    }

    Vector Vector::rotate(float angle)
    {
        Vector2 result = Vector2Rotate(toVector2(), angle);
        return Vector{result.x, result.y};
    }

    float Vector::length()
    {
        return Vector2Length(toVector2());
    }

    float Vector::distance(Vector other)
    {
        return Vector2Distance(toVector2(), other.toVector2());
    }

    float Vector::dot(Vector other)
    {
        return Vector2DotProduct(toVector2(), other.toVector2());
    }

    float Vector::angle(Vector other)
    {
        return Vector2Angle(toVector2(), other.toVector2());
    }

    Vector2 Vector::toVector2()
    {
        return Vector2{x, y};
    }

    // Functions

    // Utility
    sol::table registerMathAPI(sol::state& lua)
    {
        lua.new_enum<NoiseType>("NoiseType", {
            {"OpenSimplex", NoiseType::OpenSimplex},
            {"Perlin", NoiseType::Perlin}
        });

        sol::usertype<Vector> vectorType = lua.new_usertype<Vector>("Vector");

        vectorType["x"] = &Vector::x;
        vectorType["y"] = &Vector::y;
        vectorType["type"] = &Vector::type;
        vectorType["add"] = sol::overload(sol::resolve<Vector(Vector)>(&Vector::add), sol::resolve<Vector(float)>(&Vector::add));
        vectorType["sub"] = sol::overload(sol::resolve<Vector(Vector)>(&Vector::sub), sol::resolve<Vector(float)>(&Vector::sub));
        vectorType["scale"] = &Vector::scale;
        vectorType["mul"] = &Vector::mul;
        vectorType["div"] = &Vector::div;
        vectorType["normalize"] = &Vector::normalize;
        vectorType["rotate"] = &Vector::rotate;
        vectorType["length"] = &Vector::length;
        vectorType["distance"] = &Vector::distance;
        vectorType["dot"] = &Vector::dot;
        vectorType["angle"] = &Vector::angle;

        sol::table math = lua.create_table();

        math["clamp"] = clamp;
        math["getNoiseFrequency"] = getNoiseFrequency;
        math["getNoiseType"] = getNoiseType;
        math["getNoiseSeed"] = getNoiseSeed;
        math["getRandomSeed"] = getRandomSeed;
        math["lerp"] = lerp;
        math["newVector2"] = newVector2;
        math["noise"] = sol::overload(sol::resolve<float(float, float)>(noise), sol::resolve<float(float, float, float)>(noise));
        math["normalize"] = normalize;
        math["random"] = sol::overload(sol::resolve<float()>(random), sol::resolve<int(int)>(random), sol::resolve<int(int, int)>(random));
        math["remap"] = remap;
        math["setNoiseFrequency"] = setNoiseFrequency;
        math["setNoiseType"] = setNoiseType;
        math["setNoiseSeed"] = setNoiseSeed;
        math["setRandomSeed"] = setRandomSeed;
        math["wrap"] = wrap;

        return math;
    }

    void resetState()
    {
        noiseType = NoiseType::OpenSimplex;
        noiseSeed = 1337;
        noiseFrequency = 0.01f;
        randomSeed = 0;
    }

    // API
    float clamp(float value, float min, float max)
    {
        return Clamp(value, min, max);
    }

    float getNoiseFrequency()
    {
        return noiseFrequency;
    }

    NoiseType getNoiseType()
    {
        return noiseType;
    }

    int getNoiseSeed()
    {
        return noiseSeed;
    }

    int getRandomSeed()
    {
        return randomSeed;
    }

    float lerp(float from, float to, float amount)
    {
        return Lerp(from, to, amount);
    }

    Vector newVector2(float x, float y)
    {
        return Vector{ x, y };
    }

    float noise(float x, float y)
    {
        return fastnoiselite.GetNoise(x, y);
    }

    float noise(float x, float y, float z)
    {
        return fastnoiselite.GetNoise(x, y, z);
    }

    float normalize(float value, float from, float to)
    {
        return Normalize(value, from, to);
    }

    float random()
    {
        return GetRandomValue(0, 100) / 100.0f;
    }

    int random(int max)
    {
        return GetRandomValue(1, max);
    }

    int random(int min, int max)
    {
        return GetRandomValue(min, max);
    }

    float remap(float value, float inputStart, float inputEnd, float outputStart, float outputEnd)
    {
        return Remap(value, inputStart, inputEnd, outputStart, outputEnd);
    }

    void setNoiseFrequency(float frequency)
    {
        noiseFrequency = frequency;
        fastnoiselite.SetFrequency(frequency);
    }

    void setNoiseType(NoiseType type)
    {
        noiseType = type;
        fastnoiselite.SetNoiseType((FastNoiseLite::NoiseType)type);
    }

    void setNoiseSeed(int seed)
    {
        noiseSeed = seed;
        fastnoiselite.SetSeed(seed);
    }

    void setRandomSeed(int seed)
    {
        SetRandomSeed(seed);
    }

    float wrap(float value, float min, float max)
    {
        return Wrap(value, min, max);
    }
}
