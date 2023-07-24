#include "ai.h"

#include "../pesto.h"

#include <AStar.hpp>

AStar::Generator generator;
bool diagonal = false;

namespace ai
{
    // Functions

    // Utility
    sol::table registerAiAPI(sol::state& lua)
    {
        sol::table ai = lua.create_table();

        ai["setWorldSize"] = setWorldSize;
        ai["setDiagonalMovement"] = setDiagonalMovement;
        ai["findPath"] = findPath;
        ai["addCollision"] = addCollision;
        ai["removeCollision"] = removeCollision;
        ai["clearCollisions"] = clearCollisions;
        ai["getDiagonalMovement"] = getDiagonalMovement;

        return ai;
    }

    // API
    void setWorldSize(int width, int height)
    {
        generator.setWorldSize({width, height});
    }

    void setDiagonalMovement(bool diagonalMovement)
    {
        generator.setDiagonalMovement(diagonalMovement);

        diagonal = diagonalMovement;
    }

    sol::table findPath(int startX, int startY, int endX, int endY)
    {
        AStar::CoordinateList coordinates = generator.findPath({startX, startY}, {endX, endY});

        sol::table path = pesto::getLua().create_table();

        for (int i = 0; i < coordinates.size(); i++)
        {
            sol::table coordinate = pesto::getLua().create_table();

            coordinate["x"] = coordinates[i].x;
            coordinate["y"] = coordinates[i].y;

            path[i + 1] = coordinate;
        }

        return path;
    }

    void addCollision(int x, int y)
    {
        generator.addCollision({x, y});
    }

    void removeCollision(int x, int y)
    {
        generator.removeCollision({x, y});
    }

    void clearCollisions()
    {
        generator.clearCollisions();
    }

    bool getDiagonalMovement()
    {
        return diagonal;
    }
}
