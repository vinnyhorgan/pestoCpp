#include "graphics.h"

#include "../pesto.h"

Color backgroundColor;
Color color;
TextureFilter defaultFilter;
Font currentFont;

namespace graphics
{
    // Types

    // Camera
    std::string pCamera::type()
    {
        return "Camera";
    }

    void pCamera::setTarget(int x, int y)
    {
        camera.target = { (float)x, (float)y };
    }

    void pCamera::setOffset(int x, int y)
    {
        camera.offset = { (float)x, (float)y };
    }

    void pCamera::setRotation(float rotation)
    {
        camera.rotation = rotation;
    }

    void pCamera::setZoom(float zoom)
    {
        camera.zoom = zoom;
    }

    std::tuple<int, int> pCamera::getTarget()
    {
        return std::make_tuple((int)camera.target.x, (int)camera.target.y);
    }

    std::tuple<int, int> pCamera::getOffset()
    {
        return std::make_tuple((int)camera.offset.x, (int)camera.offset.y);
    }

    float pCamera::getRotation()
    {
        return camera.rotation;
    }

    float pCamera::getZoom()
    {
        return camera.zoom;
    }

    std::tuple<int, int> pCamera::getScreenToWorld(int x, int y)
    {
        Vector2 vector2 = GetScreenToWorld2D({ (float)x, (float)y }, camera);
        return std::make_tuple((int)vector2.x, (int)vector2.y);
    }

    std::tuple<int, int> pCamera::getWorldToScreen(int x, int y)
    {
        Vector2 vector2 = GetWorldToScreen2D({ (float)x, (float)y }, camera);
        return std::make_tuple((int)vector2.x, (int)vector2.y);
    }

    // Canvas
    std::string Canvas::type()
    {
        return "Canvas";
    }

    void Canvas::unload()
    {
        UnloadRenderTexture(texture);
    }

    std::tuple<int, int> Canvas::getDimensions()
    {
        return std::make_tuple(texture.texture.width, texture.texture.height);
    }

    TextureFilter Canvas::getFilter()
    {
        return filter;
    }

    PixelFormat Canvas::getFormat()
    {
        return (PixelFormat)texture.texture.format;
    }

    int Canvas::getHeight()
    {
        return texture.texture.height;
    }

    int Canvas::getMipmapCount()
    {
        return texture.texture.mipmaps;
    }

    int Canvas::getWidth()
    {
        return texture.texture.width;
    }

    TextureWrap Canvas::getWrap()
    {
        return wrap;
    }

    void Canvas::setFilter(TextureFilter filter)
    {
        this->filter = filter;

        SetTextureFilter(texture.texture, filter);
    }

    void Canvas::setWrap(TextureWrap wrap)
    {
        this->wrap = wrap;

        SetTextureWrap(texture.texture, wrap);
    }

    // Font
    std::string pFont::type()
    {
        return "Font";
    }

    void pFont::unload()
    {
        UnloadFont(font);
    }

    TextureFilter pFont::getFilter()
    {
        return filter;
    }

    int pFont::getHeight()
    {
        return font.baseSize;
    }

    int pFont::getWidth(std::string text)
    {
        return (int)MeasureTextEx(font, text.c_str(), (float)font.baseSize, 0).x;
    }

    void pFont::setFilter(TextureFilter filter)
    {
        this->filter = filter;

        SetTextureFilter(font.texture, filter);
    }

    // Image
    std::string pImage::type()
    {
        return "Image";
    }

    void pImage::unload()
    {
        UnloadTexture(texture);
    }

    void pImage::replacePixels(data::Data data)
    {
        UpdateTexture(texture, data.data.data());
    }

    std::tuple<int, int> pImage::getDimensions()
    {
        return std::make_tuple(texture.width, texture.height);
    }

    TextureFilter pImage::getFilter()
    {
        return filter;
    }

    PixelFormat pImage::getFormat()
    {
        return (PixelFormat)texture.format;
    }

    int pImage::getHeight()
    {
        return texture.height;
    }

    int pImage::getMipmapCount()
    {
        return texture.mipmaps;
    }

    int pImage::getWidth()
    {
        return texture.width;
    }

    TextureWrap pImage::getWrap()
    {
        return wrap;
    }

    void pImage::setFilter(TextureFilter filter)
    {
        this->filter = filter;

        SetTextureFilter(texture, filter);
    }

    void pImage::setWrap(TextureWrap wrap)
    {
        this->wrap = wrap;

        SetTextureWrap(texture, wrap);
    }

    // Shader
    std::string pShader::type()
    {
        return "Shader";
    }

    void pShader::unload()
    {
        UnloadShader(shader);
    }

    bool pShader::hasUniform(std::string name)
    {
        return GetShaderLocation(shader, name.c_str()) != -1;
    }

    void pShader::send(std::string name, float value)
    {
        SetShaderValue(shader, GetShaderLocation(shader, name.c_str()), &value, SHADER_UNIFORM_FLOAT);
    }

    void pShader::send(std::string name, bool value)
    {
        SetShaderValue(shader, GetShaderLocation(shader, name.c_str()), &value, SHADER_UNIFORM_INT);
    }

    void pShader::send(std::string name, sol::table value)
    {
        if (value.size() == 2)
        {
            Vector2 vector2 = {value[1], value[2]};
            SetShaderValue(shader, GetShaderLocation(shader, name.c_str()), &vector2, SHADER_UNIFORM_VEC2);
        }
        else if (value.size() == 3)
        {
            Vector3 vector3 = {value[1], value[2], value[3]};
            SetShaderValue(shader, GetShaderLocation(shader, name.c_str()), &vector3, SHADER_UNIFORM_VEC3);
        }
        else if (value.size() == 4)
        {
            Vector4 vector4 = {value[1], value[2], value[3], value[4]};
            SetShaderValue(shader, GetShaderLocation(shader, name.c_str()), &vector4, SHADER_UNIFORM_VEC4);
        }
        else if (value.size() == 16)
        {
            Matrix matrix = {
                value[1], value[2], value[3], value[4],
                value[5], value[6], value[7], value[8],
                value[9], value[10], value[11], value[12],
                value[13], value[14], value[15], value[16]
            };

            SetShaderValueMatrix(shader, GetShaderLocation(shader, name.c_str()), matrix);
        }
    }

    void pShader::send(std::string name, pImage image)
    {
        SetShaderValueTexture(shader, GetShaderLocation(shader, name.c_str()), image.texture);
    }

    // Functions

    // Utility
    sol::table registerGraphicsAPI(sol::state &lua)
    {
        resetState();

        lua.new_enum<BlendMode>("BlendMode", {
            {"Alpha", BLEND_ALPHA},
            {"Additive", BLEND_ADDITIVE},
            {"Multiplied", BLEND_MULTIPLIED},
            {"AddColors", BLEND_ADD_COLORS},
            {"SubtractColors", BLEND_SUBTRACT_COLORS},
            {"AlphaPremultiply", BLEND_ALPHA_PREMULTIPLY}
        });

        lua.new_enum<DrawMode>("DrawMode", {
            {"Fill", DrawMode::Fill},
            {"Line", DrawMode::Line}
        });

        lua.new_enum<TextureFilter>("FilterMode", {
            {"Point", TEXTURE_FILTER_POINT},
            {"Bilinear", TEXTURE_FILTER_BILINEAR},
            {"Trilinear", TEXTURE_FILTER_TRILINEAR},
            {"Anisotropic4x", TEXTURE_FILTER_ANISOTROPIC_4X},
            {"Anisotropic8x", TEXTURE_FILTER_ANISOTROPIC_8X},
            {"Anisotropic16x", TEXTURE_FILTER_ANISOTROPIC_16X}
        });

        lua.new_enum<PixelFormat>("PixelFormat", {
            {"Grayscale", PIXELFORMAT_UNCOMPRESSED_GRAYSCALE},
            {"GrayscaleAlpha", PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA},
            {"RGB565", PIXELFORMAT_UNCOMPRESSED_R5G6B5},
            {"RGB888", PIXELFORMAT_UNCOMPRESSED_R8G8B8},
            {"RGBA5551", PIXELFORMAT_UNCOMPRESSED_R5G5B5A1},
            {"RGBA4444", PIXELFORMAT_UNCOMPRESSED_R4G4B4A4},
            {"RGBA8888", PIXELFORMAT_UNCOMPRESSED_R8G8B8A8},
            {"R32", PIXELFORMAT_UNCOMPRESSED_R32},
            {"R32G32B32", PIXELFORMAT_UNCOMPRESSED_R32G32B32},
            {"R32G32B32A32", PIXELFORMAT_UNCOMPRESSED_R32G32B32A32},
            {"DXT1", PIXELFORMAT_COMPRESSED_DXT1_RGB},
            {"DXT1Alpha", PIXELFORMAT_COMPRESSED_DXT1_RGBA},
            {"DXT3", PIXELFORMAT_COMPRESSED_DXT3_RGBA},
            {"DXT5", PIXELFORMAT_COMPRESSED_DXT5_RGBA},
            {"ETC1", PIXELFORMAT_COMPRESSED_ETC1_RGB},
            {"ETC2", PIXELFORMAT_COMPRESSED_ETC2_RGB},
            {"ETC2Alpha", PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA},
            {"PVRT", PIXELFORMAT_COMPRESSED_PVRT_RGB},
            {"PVRTAlpha", PIXELFORMAT_COMPRESSED_PVRT_RGBA},
            {"ASTC4x4", PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA},
            {"ASTC8x8", PIXELFORMAT_COMPRESSED_ASTC_8x8_RGBA}
        });

        lua.new_enum<TextureWrap>("WrapMode", {
            {"Repeat", TEXTURE_WRAP_REPEAT},
            {"Clamp", TEXTURE_WRAP_CLAMP},
            {"MirrorRepeat", TEXTURE_WRAP_MIRROR_REPEAT},
            {"MirrorClamp", TEXTURE_WRAP_MIRROR_CLAMP}
        });

        sol::usertype<pCamera> cameraType = lua.new_usertype<pCamera>("Camera");

        cameraType["type"] = &pCamera::type;
        cameraType["setTarget"] = &pCamera::setTarget;
        cameraType["setOffset"] = &pCamera::setOffset;
        cameraType["setRotation"] = &pCamera::setRotation;
        cameraType["setZoom"] = &pCamera::setZoom;
        cameraType["getTarget"] = &pCamera::getTarget;
        cameraType["getOffset"] = &pCamera::getOffset;
        cameraType["getRotation"] = &pCamera::getRotation;
        cameraType["getZoom"] = &pCamera::getZoom;
        cameraType["getScreenToWorld"] = &pCamera::getScreenToWorld;
        cameraType["getWorldToScreen"] = &pCamera::getWorldToScreen;

        sol::usertype<Canvas> canvasType = lua.new_usertype<Canvas>("Canvas");

        canvasType["type"] = &Canvas::type;
        canvasType["unload"] = &Canvas::unload;
        canvasType["getSize"] = &Canvas::getDimensions;
        canvasType["getFilter"] = &Canvas::getFilter;
        canvasType["getFormat"] = &Canvas::getFormat;
        canvasType["getHeight"] = &Canvas::getHeight;
        canvasType["getMipmaps"] = &Canvas::getMipmapCount;
        canvasType["getWidth"] = &Canvas::getWidth;
        canvasType["getWrap"] = &Canvas::getWrap;
        canvasType["setFilter"] = &Canvas::setFilter;
        canvasType["setWrap"] = &Canvas::setWrap;

        sol::usertype<pFont> fontType = lua.new_usertype<pFont>("Font");

        fontType["type"] = &pFont::type;
        fontType["unload"] = &pFont::unload;
        fontType["getFilter"] = &pFont::getFilter;
        fontType["getHeight"] = &pFont::getHeight;
        fontType["getWidth"] = &pFont::getWidth;
        fontType["setFilter"] = &pFont::setFilter;

        sol::usertype<pImage> imageType = lua.new_usertype<pImage>("Image");

        imageType["type"] = &pImage::type;
        imageType["unload"] = &pImage::unload;
        imageType["replacePixels"] = &pImage::replacePixels;
        imageType["getSize"] = &pImage::getDimensions;
        imageType["getFilter"] = &pImage::getFilter;
        imageType["getFormat"] = &pImage::getFormat;
        imageType["getHeight"] = &pImage::getHeight;
        imageType["getMipmaps"] = &pImage::getMipmapCount;
        imageType["getWidth"] = &pImage::getWidth;
        imageType["getWrap"] = &pImage::getWrap;
        imageType["setFilter"] = &pImage::setFilter;
        imageType["setWrap"] = &pImage::setWrap;

        sol::usertype<pShader> shaderType = lua.new_usertype<pShader>("Shader");

        shaderType["type"] = &pShader::type;
        shaderType["unload"] = &pShader::unload;
        shaderType["hasUniform"] = &pShader::hasUniform;
        shaderType["send"] = sol::overload(sol::resolve<void(std::string, float)>(&pShader::send), sol::resolve<void(std::string, bool)>(&pShader::send), sol::resolve<void(std::string, sol::table)>(&pShader::send), sol::resolve<void(std::string, pImage)>(&pShader::send));

        sol::table graphics = lua.create_table();

        graphics["circle"] = circle;
        graphics["clear"] = sol::overload(sol::resolve<void(int, int, int)>(clear), sol::resolve<void(int, int, int, int)>(clear));
        graphics["draw"] = sol::overload(sol::resolve<void(pImage, float, float)>(draw), sol::resolve<void(pImage, float, float, float, float)>(draw), sol::resolve<void(Canvas, float, float)>(draw));
        graphics["ellipse"] = ellipse;
        graphics["line"] = line;
        graphics["point"] = point;
        graphics["polygon"] = sol::overload(sol::resolve<void(DrawMode, float, float, int, float)>(polygon), sol::resolve<void(DrawMode, float, float, int, float, float)>(polygon));
        graphics["print"] = sol::overload(sol::resolve<void(std::string, float, float)>(print), sol::resolve<void(std::string, float, float, float, float)>(print));
        graphics["rectangle"] = sol::overload(sol::resolve<void(DrawMode, float, float, int, int)>(rectangle), sol::resolve<void(DrawMode, float, float, int, int, float)>(rectangle));
        graphics["triangle"] = triangle;
        graphics["captureScreenshot"] = captureScreenshot;
        graphics["newCamera"] = newCamera;
        graphics["newCanvas"] = newCanvas;
        graphics["newFont"] = newFont;
        graphics["newImage"] = newImage;
        graphics["newShader"] = newShader;
        graphics["getBackgroundColor"] = getBackgroundColor;
        graphics["getColor"] = getColor;
        graphics["getDefaultFilter"] = getDefaultFilter;
        graphics["getFont"] = getFont;
        graphics["setBackgroundColor"] = sol::overload(sol::resolve<void(int, int, int)>(setBackgroundColor), sol::resolve<void(int, int, int, int)>(setBackgroundColor));
        graphics["setBlendMode"] = sol::overload(sol::resolve<void(BlendMode)>(setBlendMode), sol::resolve<void()>(setBlendMode));
        graphics["setCamera"] = sol::overload(sol::resolve<void(pCamera)>(setCamera), sol::resolve<void()>(setCamera));
        graphics["setCanvas"] = sol::overload(sol::resolve<void(Canvas)>(setCanvas), sol::resolve<void()>(setCanvas));
        graphics["setColor"] = sol::overload(sol::resolve<void(int, int, int)>(setColor), sol::resolve<void(int, int, int, int)>(setColor));
        graphics["setDefaultFilter"] = setDefaultFilter;
        graphics["setFont"] = setFont;
        graphics["setShader"] = sol::overload(sol::resolve<void(pShader)>(setShader), sol::resolve<void()>(setShader));

        return graphics;
    }

    void resetState()
    {
        backgroundColor = BLACK;
        color = WHITE;
        defaultFilter = TEXTURE_FILTER_POINT;
        currentFont = pesto::getSmallFont();
    }

    // API
    void circle(DrawMode mode, float x, float y, float radius)
    {
        if (mode == DrawMode::Fill)
        {
            DrawCircle((int)x, (int)y, radius, color);
        }
        else if (mode == DrawMode::Line)
        {
            DrawCircleLines((int)x, (int)y, radius, color);
        }
    }

    void clear(int r, int g, int b)
    {
        ClearBackground({(unsigned char)r, (unsigned char)g, (unsigned char)b, 255});
    }

    void clear(int r, int g, int b, int a)
    {
        ClearBackground({(unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a});
    }

    void draw(pImage image, float x, float y)
    {
        DrawTexture(image.texture, (int)x, (int)y, color);
    }

    void draw(pImage image, float x, float y, float rotation, float scale)
    {
        DrawTextureEx(image.texture, {x, y}, rotation, scale, color);
    }

    void draw(Canvas canvas, float x, float y)
    {
        DrawTexture(canvas.texture.texture, (int)x, (int)y, WHITE);
    }

    void ellipse(DrawMode mode, float x, float y, float radiusX, float radiusY)
    {
        if (mode == DrawMode::Fill)
        {
            DrawEllipse((int)x, (int)y, radiusX, radiusY, color);
        }
        else if (mode == DrawMode::Line)
        {
            DrawEllipseLines((int)x, (int)y, radiusX, radiusY, color);
        }
    }

    void line(float x1, float y1, float x2, float y2)
    {
        DrawLine((int)x1, (int)y1, (int)x2, (int)y2, color);
    }

    void point(float x, float y)
    {
        DrawPixel((int)x, (int)y, color);
    }

    void polygon(DrawMode mode, float x, float y, int sides, float radius)
    {
        if (mode == DrawMode::Fill)
        {
            DrawPoly({x, y}, sides, radius, 0, color);
        }
        else if (mode == DrawMode::Line)
        {
            DrawPolyLines({x, y}, sides, radius, 0, color);
        }
    }

    void polygon(DrawMode mode, float x, float y, int sides, float radius, float rotation)
    {
        if (mode == DrawMode::Fill)
        {
            DrawPoly({x, y}, sides, radius, rotation, color);
        }
        else if (mode == DrawMode::Line)
        {
            DrawPolyLines({x, y}, sides, radius, rotation, color);
        }
    }

    void print(std::string text, float x, float y)
    {
        DrawTextEx(currentFont, text.c_str(), {x, y}, (float)currentFont.baseSize, 0, color);
    }

    void print(std::string text, float x, float y, float rotation, float scale)
    {
        DrawTextPro(currentFont, text.c_str(), {x, y}, {0, 0}, rotation, (float)currentFont.baseSize * scale, 0, color);
    }

    void rectangle(DrawMode mode, float x, float y, int width, int height)
    {
        if (mode == DrawMode::Fill)
        {
            DrawRectangle((int)x, (int)y, width, height, color);
        }
        else if (mode == DrawMode::Line)
        {
            DrawRectangleLines((int)x, (int)y, width, height, color);
        }
    }

    void rectangle(DrawMode mode, float x, float y, int width, int height, float rotation)
    {
        if (mode == DrawMode::Fill)
        {
            DrawRectanglePro({x, y, (float)width, (float)height}, {0, 0}, rotation, color);
        }
        else if (mode == DrawMode::Line)
        {
            DrawRectanglePro({x, y, (float)width, (float)height}, {0, 0}, rotation, color);
        }
    }

    void triangle(DrawMode mode, float x1, float y1, float x2, float y2, float x3, float y3)
    {
        if (mode == DrawMode::Fill)
        {
            DrawTriangle({x1, y1}, {x2, y2}, {x3, y3}, color);
        }
        else if (mode == DrawMode::Line)
        {
            DrawTriangleLines({x1, y1}, {x2, y2}, {x3, y3}, color);
        }
    }

    void captureScreenshot(std::string filename)
    {
        TakeScreenshot((pesto::getCurrentProject() + "/" + filename).c_str());
    }

    pCamera newCamera()
    {
        pCamera camera;
        camera.camera.target = { 0, 0 };
        camera.camera.offset = { 0, 0 };
        camera.camera.rotation = 0;
        camera.camera.zoom = 1;

        return camera;
    }

    Canvas newCanvas(int width, int height)
    {
        Canvas canvas;
        canvas.texture = LoadRenderTexture(width, height);
        canvas.setFilter(defaultFilter);
        canvas.setWrap(TEXTURE_WRAP_CLAMP);

        return canvas;
    }

    pFont newFont(std::string filename, int size)
    {
        pFont font;
        font.font = LoadFontEx((pesto::getCurrentProject() + "/" + filename).c_str(), size, 0, 0);
        font.setFilter(defaultFilter);

        return font;
    }

    pImage newImage(std::string filename)
    {
        pImage image;
        image.texture = LoadTexture((pesto::getCurrentProject() + "/" + filename).c_str());
        image.setFilter(defaultFilter);
        image.setWrap(TEXTURE_WRAP_CLAMP);

        return image;
    }

    pShader newShader(std::string vertex, std::string fragment)
    {
        pShader shader;
        shader.shader = LoadShader((pesto::getCurrentProject() + "/" + vertex).c_str(), (pesto::getCurrentProject() + "/" + fragment).c_str());

        return shader;
    }

    std::tuple<int, int, int, int> getBackgroundColor()
    {
        return std::make_tuple(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    }

    std::tuple<int, int, int, int> getColor()
    {
        return std::make_tuple(color.r, color.g, color.b, color.a);
    }

    TextureFilter getDefaultFilter()
    {
        return defaultFilter;
    }

    pFont getFont()
    {
        pFont font;
        font.font = currentFont;

        return font;
    }

    void setBackgroundColor(int r, int g, int b)
    {
        backgroundColor = {(unsigned char)r, (unsigned char)g, (unsigned char)b, 255};
    }

    void setBackgroundColor(int r, int g, int b, int a)
    {
        backgroundColor = {(unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a};
    }

    void setBlendMode(BlendMode mode)
    {
        BeginBlendMode(mode);
    }

    void setBlendMode()
    {
        EndBlendMode();
    }

    void setCamera(pCamera camera)
    {
        BeginMode2D(camera.camera);
    }

    void setCamera()
    {
        EndMode2D();
    }

    void setCanvas(Canvas canvas)
    {
        BeginTextureMode(canvas.texture);
    }

    void setCanvas()
    {
        EndTextureMode();
    }

    void setColor(int r, int g, int b)
    {
        color = {(unsigned char)r, (unsigned char)g, (unsigned char)b, 255};
    }

    void setColor(int r, int g, int b, int a)
    {
        color = {(unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a};
    }

    void setDefaultFilter(TextureFilter filter)
    {
        defaultFilter = filter;
    }

    void setFont(pFont font)
    {
        currentFont = font.font;
    }

    void setShader(pShader shader)
    {
        BeginShaderMode(shader.shader);
    }

    void setShader()
    {
        EndShaderMode();
    }
}
