#pragma once

#include "data.h"

#include <sol/sol.hpp>
#include <raylib.h>

#include <string>

namespace graphics
{
    // Enums
    enum DrawMode
    {
        Fill,
        Line
    };

    // Types
    class pCamera
    {
    public:
        Camera2D camera;

        std::string type();

        void setTarget(int x, int y);
        void setOffset(int x, int y);
        void setRotation(float rotation);
        void setZoom(float zoom);
        std::tuple<int, int> getTarget();
        std::tuple<int, int> getOffset();
        float getRotation();
        float getZoom();
        std::tuple<int, int> getScreenToWorld(int x, int y);
        std::tuple<int, int> getWorldToScreen(int x, int y);
    };

    class Canvas
    {
    public:
        RenderTexture2D texture;

        TextureFilter filter;
        TextureWrap wrap;

        std::string type();
        void unload();

        std::tuple<int, int> getDimensions();
        TextureFilter getFilter();
        PixelFormat getFormat();
        int getHeight();
        int getMipmapCount();
        int getWidth();
        TextureWrap getWrap();
        void setFilter(TextureFilter filter);
        void setWrap(TextureWrap wrap);
    };

    class pFont
    {
    public:
        Font font;

        TextureFilter filter;

        std::string type();
        void unload();

        TextureFilter getFilter();
        int getHeight();
        int getWidth(std::string text);
        void setFilter(TextureFilter filter);
    };

    class pImage
    {
    public:
        Texture texture;

        TextureFilter filter;
        TextureWrap wrap;

        std::string type();
        void unload();

        void replacePixels(data::Data data);
        std::tuple<int, int> getDimensions();
        TextureFilter getFilter();
        PixelFormat getFormat();
        int getHeight();
        int getMipmapCount();
        int getWidth();
        TextureWrap getWrap();
        void setFilter(TextureFilter filter);
        void setWrap(TextureWrap wrap);
    };

    class pShader
    {
    public:
        Shader shader;

        std::string type();
        void unload();

        bool hasUniform(std::string name);
        void send(std::string name, float value);
        void send(std::string name, bool value);
        void send(std::string name, sol::table value);
        void send(std::string name, pImage image);
    };

    // Functions

    // Utility
    sol::table registerGraphicsAPI(sol::state& lua);
    void resetState();

    // API

    // Drawing

    // using floats for positions to avoid teal compiler errors with rounding
    // maybe should be applied to all functions for positions

    void circle(DrawMode mode, float x, float y, float radius);
    void clear(int r, int g, int b);
    void clear(int r, int g, int b, int a);
    void draw(pImage image, float x, float y);
    void draw(pImage image, float x, float y, float rotation, float scale);
    void draw(Canvas canvas, float x, float y);
    void ellipse(DrawMode mode, float x, float y, float radiusX, float radiusY);
    void line(float x1, float y1, float x2, float y2);
    void point(float x, float y);
    void polygon(DrawMode mode, float x, float y, int sides, float radius);
    void polygon(DrawMode mode, float x, float y, int sides, float radius, float rotation);
    void print(std::string text, float x, float y);
    void print(std::string text, float x, float y, float rotation, float scale);
    void rectangle(DrawMode mode, float x, float y, int width, int height);
    void rectangle(DrawMode mode, float x, float y, int width, int height, float rotation);
    void triangle(DrawMode mode, float x1, float y1, float x2, float y2, float x3, float y3);

    // Object creation
    void captureScreenshot(std::string filename);
    pCamera newCamera();
    Canvas newCanvas(int width, int height);
    pFont newFont(std::string filename, int size);
    pImage newImage(std::string filename);
    pShader newShader(std::string vertex, std::string fragment);

    // Graphics state
    std::tuple<int, int, int, int> getBackgroundColor();
    std::tuple<int, int, int, int> getColor();
    TextureFilter getDefaultFilter();
    pFont getFont();
    void setBackgroundColor(int r, int g, int b);
    void setBackgroundColor(int r, int g, int b, int a);
    void setBlendMode(BlendMode mode);
    void setBlendMode();
    void setCamera(pCamera camera);
    void setCamera();
    void setCanvas(Canvas canvas);
    void setCanvas();
    void setColor(int r, int g, int b);
    void setColor(int r, int g, int b, int a);
    void setDefaultFilter(TextureFilter filter);
    void setFont(pFont font);
    void setShader(pShader shader);
    void setShader();

    // TODO: Add particle system, other raylib functions and ideally video support
}
