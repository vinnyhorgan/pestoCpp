#include "physics.h"

namespace physics
{
    // Functions

    // Utility
    sol::table registerPhysicsAPI(sol::state& lua)
    {
        sol::table physics = lua.create_table();

        return physics;
    }

    // API
}
