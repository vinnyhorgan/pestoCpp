#pragma once

#include <sol/sol.hpp>

namespace ai
{
    // Functions

    // Utility
    sol::table registerAiAPI(sol::state& lua);

    // API
    void setWorldSize(int width, int height);
    void setDiagonalMovement(bool diagonalMovement);
    sol::table findPath(int startX, int startY, int endX, int endY);
    void addCollision(int x, int y);
    void removeCollision(int x, int y);
    void clearCollisions();
    bool getDiagonalMovement();
}
