#pragma once

#include <string>
#include <functional>

// This file exists because raylib and enet have namespace conflicts

namespace net
{
    void registerCallbacks(std::function<void(std::string, int)> connect, std::function<void(unsigned char*, int)> receive, std::function<void(std::string, int)> disconnect);
    int init();
    void deinit();
    int createHost(int port, int maxClients);
    void destroyHost();
    void service();
    void sendReliable(unsigned char* data, int dataSize);
    void sendUnreliable(unsigned char* data, int dataSize);
    int getIncomingBandwidth();
    int getOutgoingBandwidth();
    int getPacketLossPercentage();
    int getPing();
}
