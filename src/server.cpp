#include "server.h"

#include "net.h"
#include "pesto.h"
#include "logger.h"
#include "error.h"
#include "debug.h"
#include "modules/event.h"

#include <raylib.h>

#include <vector>

#define DEFAULT_PORT 7777
#define DEFAULT_MAX_CLIENTS 32

struct Client
{
    std::string address;
    int port;
};

std::vector<Client> clients;

void onConnect(std::string address, int port)
{
    debug::message("Client connected.");

    Client client;

    client.address = address;
    client.port = port;

    clients.push_back(client);
}

void onReceive(unsigned char* data, int dataSize)
{
    debug::message("Received packet from client of size: " + std::to_string(dataSize) + " bytes.");
    net::sendReliable(data, dataSize);
}

void onDisconnect(std::string address, int port)
{
    debug::message("Client disconnected.");

    for (int i = 0; i < clients.size(); i++)
    {
        if (clients[i].address == address && clients[i].port == port)
        {
            clients.erase(clients.begin() + i);
        }
    }
}

namespace server
{
    void init()
    {
        logger::info("Initializing server...");

        if (net::init() != 0)
        {
            error::throwError("Failed to initialize ENet.");

            return;
        }

        if (net::createHost(DEFAULT_PORT, DEFAULT_MAX_CLIENTS) != 0)
        {
            error::throwError("Failed to create ENet host.");

            return;
        }

        net::registerCallbacks(onConnect, onReceive, onDisconnect);

        SetConfigFlags(FLAG_WINDOW_ALWAYS_RUN);

        logger::info("Server started on port " + std::to_string(DEFAULT_PORT) + ".");

        debug::message("Server started.");
    }

    void update()
    {
        if (IsKeyPressed(KEY_ESCAPE))
        {
            event::quit();
        }

        net::service();
    }

    void draw()
    {
        Font robotoSmall = pesto::getSmallFont();

        BeginDrawing();

        ClearBackground(BLACK);

        DrawTextEx(robotoSmall, TextFormat("Server listening on port %d with max %d clients...", DEFAULT_PORT, DEFAULT_MAX_CLIENTS), {10, 10}, (float)robotoSmall.baseSize, 0, GREEN);

        DrawTextEx(robotoSmall, TextFormat("Uptime: %d days | %d hours | %d minutes | %d seconds.", (int)GetTime() / 86400, (int)GetTime() / 3600, (int)GetTime() / 60, (int)GetTime()), { 10, 30 }, (float)robotoSmall.baseSize, 0, GREEN);

        DrawTextEx(robotoSmall, TextFormat("Incoming bandwidth: %d bytes/s", net::getIncomingBandwidth()), { 10, 50 }, (float)robotoSmall.baseSize, 0, GREEN);
        DrawTextEx(robotoSmall, TextFormat("Outgoing bandwidth: %d bytes/s", net::getOutgoingBandwidth()), { 10, 70 }, (float)robotoSmall.baseSize, 0, GREEN);

        DrawTextEx(robotoSmall, TextFormat("Connected clients: %d", clients.size()), { 10, 90 }, (float)robotoSmall.baseSize, 0, GREEN);

        for (int i = 0; i < clients.size(); i++)
        {
            DrawTextEx(robotoSmall, TextFormat("Client %d: %s:%d", i, clients[i].address.c_str(), clients[i].port), { 10, (float)110 + (i * 20) }, (float)robotoSmall.baseSize, 0, GREEN);
        }

        // add ping and packet loss percentage

        EndDrawing();
    }

    void deinit()
    {
        net::destroyHost();

        net::deinit();

        logger::info("Server stopped.");
    }
}
