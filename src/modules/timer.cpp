#include "timer.h"

#include <raylib.h>
#include <reasings.h>

#include <chrono>
#include <thread>
#include <map>

float averageDelta = 0.0f;
int framesCount = 0;
float averageDeltaTimer = 0.0f;

struct AfterTimer
{
    float currentTime;
    sol::function function;
};

struct EveryTimer
{
    float currentTime;
    float time;
    sol::function function;
    int times;
    int currentTimes;
};

struct DuringTimer
{
    float currentTime;
    sol::function function;
    sol::function after;
    bool hasAfter;
};

std::map<int, AfterTimer> afterTimers;
std::map<int, EveryTimer> everyTimers;
std::map<int, DuringTimer> duringTimers;

namespace timer
{
    // Functions

    // Utility
    sol::table registerTimerAPI(sol::state& lua)
    {
        lua.new_enum<TweenType>("TweenType", {
            {"Linear", TweenType::Linear},
            {"LinearIn", TweenType::LinearIn},
            {"LinearOut", TweenType::LinearOut},
            {"LinearInOut", TweenType::LinearInOut},
            {"SineIn", TweenType::SineIn},
            {"SineOut", TweenType::SineOut},
            {"SineInOut", TweenType::SineInOut},
            {"CircularIn", TweenType::CircularIn},
            {"CircularOut", TweenType::CircularOut},
            {"CircularInOut", TweenType::CircularInOut},
            {"CubicIn", TweenType::CubicIn},
            {"CubicOut", TweenType::CubicOut},
            {"CubicInOut", TweenType::CubicInOut},
            {"QuadraticIn", TweenType::QuadraticIn},
            {"QuadraticOut", TweenType::QuadraticOut},
            {"QuadraticInOut", TweenType::QuadraticInOut},
            {"ExponentialIn", TweenType::ExponentialIn},
            {"ExponentialOut", TweenType::ExponentialOut},
            {"ExponentialInOut", TweenType::ExponentialInOut},
            {"BackIn", TweenType::BackIn},
            {"BackOut", TweenType::BackOut},
            {"BackInOut", TweenType::BackInOut},
            {"BounceIn", TweenType::BounceIn},
            {"BounceOut", TweenType::BounceOut},
            {"BounceInOut", TweenType::BounceInOut},
            {"ElasticIn", TweenType::ElasticIn},
            {"ElasticOut", TweenType::ElasticOut},
            {"ElasticInOut", TweenType::ElasticInOut}
        });

        sol::table timer = lua.create_table();

        timer["getAverageDelta"] = getAverageDelta;
        timer["getDelta"] = getDelta;
        timer["getFPS"] = getFPS;
        timer["getTime"] = getTime;
        timer["sleep"] = sleep;
        timer["after"] = after;
        timer["every"] = sol::overload(sol::resolve<int(float, sol::function)>(every), sol::resolve<int(float, sol::function, int)>(every));
        timer["during"] = sol::overload(sol::resolve<int(float, sol::function)>(during), sol::resolve<int(float, sol::function, sol::function)>(during));
        timer["cancel"] = cancel;
        timer["clear"] = clear;
        timer["tween"] = sol::overload(sol::resolve<float(float, float, float, float)>(tween), sol::resolve<float(float, float, float, float, TweenType)>(tween));

        return timer;
    }

    void update()
    {
        averageDeltaTimer += GetFrameTime();
        framesCount++;

        if (averageDeltaTimer >= 1.0f)
        {
            averageDelta = averageDeltaTimer / framesCount;

            framesCount = 0;
            averageDeltaTimer = 0.0f;
        }

        for (auto it = afterTimers.begin(); it != afterTimers.end();)
        {
            auto& timer = it->second;
            timer.currentTime -= GetFrameTime();

            if (timer.currentTime <= 0.0f)
            {
                timer.function();
                it = afterTimers.erase(it);
            }
            else
            {
                ++it;
            }
        }

        for (auto it = everyTimers.begin(); it != everyTimers.end();)
        {
            auto& timer = it->second;
            timer.currentTime -= GetFrameTime();

            if (timer.currentTime <= 0.0f)
            {
                timer.function();
                timer.currentTime = timer.time;

                if (timer.times > -1)
                {
                    timer.currentTimes++;

                    if (timer.currentTimes >= timer.times)
                    {
                        it = everyTimers.erase(it);
                    }
                    else
                    {
                        ++it;
                    }
                }
                else
                {
                    ++it;
                }
            }
            else
            {
                ++it;
            }
        }

        for (auto it = duringTimers.begin(); it != duringTimers.end();)
        {
            auto& timer = it->second;
            timer.currentTime -= GetFrameTime();

            if (timer.currentTime <= 0.0f)
            {
                if (timer.hasAfter)
                {
                    auto after = timer.after;
                    it = duringTimers.erase(it);
                    after();
                }
                else
                {
                    it = duringTimers.erase(it);
                }
            }
            else
            {
                timer.function();
                ++it;
            }
        }
    }

    void resetState()
    {
        afterTimers.clear();
        everyTimers.clear();
        duringTimers.clear();
    }

    // API
    float getAverageDelta()
    {
        return averageDelta;
    }

    float getDelta()
    {
        return GetFrameTime();
    }

    int getFPS()
    {
        return GetFPS();
    }

    float getTime()
    {
        return (float)GetTime();
    }

    void sleep(float seconds)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds((int)(seconds * 1000)));
    }

    int after(float seconds, sol::function function)
    {
        int id = GetRandomValue(0, 10000);

        afterTimers[id] = { seconds, function };

        return id;
    }

    int every(float seconds, sol::function function)
    {
        int id = GetRandomValue(0, 10000);

        everyTimers[id] = { seconds, seconds, function, -1, -2 };

        return id;
    }

    int every(float seconds, sol::function function, int times)
    {
        int id = GetRandomValue(0, 10000);

        everyTimers[id] = { seconds, seconds, function, times, 0 };

        return id;
    }

    int during(float seconds, sol::function function)
    {
        int id = GetRandomValue(0, 10000);

        sol::function emptyFunction;

        duringTimers[id] = { seconds, function, emptyFunction, false };

        return id;
    }

    int during(float seconds, sol::function function, sol::function after)
    {
        int id = GetRandomValue(0, 10000);

        duringTimers[id] = { seconds, function, after, true };

        return id;
    }

    void cancel(int id)
    {
        if (afterTimers.find(id) != afterTimers.end())
        {
            afterTimers.erase(id);
        }

        if (everyTimers.find(id) != everyTimers.end())
        {
            everyTimers.erase(id);
        }

        if (duringTimers.find(id) != duringTimers.end())
        {
            duringTimers.erase(id);
        }
    }

    void clear()
    {
        afterTimers.clear();
        everyTimers.clear();
        duringTimers.clear();
    }

    float tween(float timer, float from, float to, float seconds)
    {
        return EaseLinearNone(timer, from, to - from, seconds);
    }

    float tween(float timer, float from, float to, float seconds, TweenType type)
    {
        switch (type)
        {
            case TweenType::Linear:
                return EaseLinearNone(timer, from, to - from, seconds);
                break;
            case TweenType::LinearIn:
                return EaseLinearIn(timer, from, to - from, seconds);
                break;
            case TweenType::LinearOut:
                return EaseLinearOut(timer, from, to - from, seconds);
                break;
            case TweenType::LinearInOut:
                return EaseLinearInOut(timer, from, to - from, seconds);
                break;
            case TweenType::SineIn:
                return EaseSineIn(timer, from, to - from, seconds);
                break;
            case TweenType::SineOut:
                return EaseSineOut(timer, from, to - from, seconds);
                break;
            case TweenType::SineInOut:
                return EaseSineInOut(timer, from, to - from, seconds);
                break;
            case TweenType::CircularIn:
                return EaseCircIn(timer, from, to - from, seconds);
                break;
            case TweenType::CircularOut:
                return EaseCircOut(timer, from, to - from, seconds);
                break;
            case TweenType::CircularInOut:
                return EaseCircInOut(timer, from, to - from, seconds);
                break;
            case TweenType::CubicIn:
                return EaseCubicIn(timer, from, to - from, seconds);
                break;
            case TweenType::CubicOut:
                return EaseCubicOut(timer, from, to - from, seconds);
                break;
            case TweenType::CubicInOut:
                return EaseCubicInOut(timer, from, to - from, seconds);
                break;
            case TweenType::QuadraticIn:
                return EaseQuadIn(timer, from, to - from, seconds);
                break;
            case TweenType::QuadraticOut:
                return EaseQuadOut(timer, from, to - from, seconds);
                break;
            case TweenType::QuadraticInOut:
                return EaseQuadInOut(timer, from, to - from, seconds);
                break;
            case TweenType::ExponentialIn:
                return EaseExpoIn(timer, from, to - from, seconds);
                break;
            case TweenType::ExponentialOut:
                return EaseExpoOut(timer, from, to - from, seconds);
                break;
            case TweenType::ExponentialInOut:
                return EaseExpoInOut(timer, from, to - from, seconds);
                break;
            case TweenType::BackIn:
                return EaseBackIn(timer, from, to - from, seconds);
                break;
            case TweenType::BackOut:
                return EaseBackOut(timer, from, to - from, seconds);
                break;
            case TweenType::BackInOut:
                return EaseBackInOut(timer, from, to - from, seconds);
                break;
            case TweenType::BounceIn:
                return EaseBounceIn(timer, from, to - from, seconds);
                break;
            case TweenType::BounceOut:
                return EaseBounceOut(timer, from, to - from, seconds);
                break;
            case TweenType::BounceInOut:
                return EaseBounceInOut(timer, from, to - from, seconds);
                break;
            case TweenType::ElasticIn:
                return EaseElasticIn(timer, from, to - from, seconds);
                break;
            case TweenType::ElasticOut:
                return EaseElasticOut(timer, from, to - from, seconds);
                break;
            case TweenType::ElasticInOut:
                return EaseElasticInOut(timer, from, to - from, seconds);
                break;
            default:
                break;
        }

        return 0;
    }
}
