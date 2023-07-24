#pragma once

#include <sol/sol.hpp>
#include <raylib.h>

#include <string>

namespace audio
{
    // Enums
    enum SourceType
    {
        Static,
        Stream
    };

    // Types
    class Source
    {
    public:
        SourceType sourceType;
        Sound sound;
        Music music;

        float pitch = 1.0f;
        float volume = 1.0f;
        float pan = 0.5f;
        Vector2 position = { 0.0f, 0.0f };
        float maxDistance = 200.0f;

        std::string type();
        void unload();

        SourceType getSourceType();
        int getChannelCount();
        int getSampleRate();
        int getSampleSize();
        int getSampleCount();
        float getPitch();
        float getVolume();
        float getPan();
        float getDuration();
        std::tuple<float, float> getPosition();
        float getMaxDistance();
        bool isPlaying();
        bool isLooping();
        void play();
        void playSpatial(float listenerX, float listenerY);
        void pause();
        void resume();
        void stop();
        void seek(float time);
        void setPitch(float pitch);
        void setVolume(float volume);
        void setPan(float pan);
        void setLooping(bool looping);
        void setPosition(float x, float y);
        void setMaxDistance(float maxDistance);
    };

    // Functions

    // Utility
    sol::table registerAudioAPI(sol::state& lua);

    // API
    int getActiveSourceCount();
    float getVolume();
    Source newSource(std::string filename, SourceType type);
    void setVolume(float volume);
    void stopAllSources();
}
