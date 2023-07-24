#pragma once

#include "data.h"

#include <sol/sol.hpp>

namespace networking
{
    // Functions

    // Utility
    sol::table registerNetworkingAPI(sol::state& lua);
    void update();
    void resetState();

    // API
    void connect(std::string address);
    void disconnect();
    void sendReliable(data::Data data);
    void sendUnreliable(data::Data data);
}
