#pragma once

#include <sol/sol.hpp>

namespace timer
{
    // Enums
    enum TweenType
    {
        Linear,
        LinearIn,
        LinearOut,
        LinearInOut,
        SineIn,
        SineOut,
        SineInOut,
        CircularIn,
        CircularOut,
        CircularInOut,
        CubicIn,
        CubicOut,
        CubicInOut,
        QuadraticIn,
        QuadraticOut,
        QuadraticInOut,
        ExponentialIn,
        ExponentialOut,
        ExponentialInOut,
        BackIn,
        BackOut,
        BackInOut,
        BounceIn,
        BounceOut,
        BounceInOut,
        ElasticIn,
        ElasticOut,
        ElasticInOut
    };

    // Functions

    // Utility
    sol::table registerTimerAPI(sol::state& lua);
    void update();
    void resetState();

    // API

    // Generic
    float getAverageDelta();
    float getDelta();
    int getFPS();
    float getTime();
    void sleep(float seconds);

    // HUMP-like API
    int after(float seconds, sol::function function);
    int every(float seconds, sol::function function);
    int every(float seconds, sol::function function, int times);
    int during(float seconds, sol::function function);
    int during(float seconds, sol::function function, sol::function after);
    void cancel(int id);
    void clear();
    float tween(float timer, float from, float to, float seconds);
    float tween(float timer, float from, float to, float seconds, TweenType type);
}
