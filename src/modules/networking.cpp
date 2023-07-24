#include "networking.h"

#include "../pesto.h"
#include "../logger.h"
#include "../error.h"
#include "../debug.h"

#include <enet/enet.h>

#define DEFAULT_PORT 7777

bool isInitialized = false;
bool isConnected = false;

ENetAddress clientAddress;
ENetHost* client;
ENetPeer* server;
ENetEvent clientEvent;

namespace networking
{
    // Functions

    // Utility
    sol::table registerNetworkingAPI(sol::state& lua)
    {
        sol::table networking = lua.create_table();

        networking["connect"] = connect;
        networking["disconnect"] = disconnect;
        networking["sendReliable"] = sendReliable;
        networking["sendUnreliable"] = sendUnreliable;

        return networking;
    }

    void update()
    {
        if (isInitialized)
        {
            while (enet_host_service(client, &clientEvent, 0) > 0)
            {
                switch (clientEvent.type)
                {
                case ENET_EVENT_TYPE_CONNECT:
                {
                    debug::message("Connected to server.");
                    isConnected = true;

                    break;
                }
                case ENET_EVENT_TYPE_RECEIVE:
                {
                    debug::message("Received packet from server of size: " + std::to_string(clientEvent.packet->dataLength) + " bytes.");

                    data::Data data;

                    data.data = std::vector<unsigned char>(clientEvent.packet->data, clientEvent.packet->data + clientEvent.packet->dataLength);

                    sol::state& lua = pesto::getLua();

                    sol::protected_function luaPacketreceived = lua["packetreceived"];

                    if (luaPacketreceived.valid())
                    {
                        auto result = luaPacketreceived(data);

                        if (!result.valid())
                        {
                            sol::error err = result;
                            error::throwError(err.what());
                        }
                    }

                    break;
                }
                case ENET_EVENT_TYPE_DISCONNECT:
                {
                    debug::message("Disconnected from server.");
                    isConnected = false;

                    break;
                }
                }
            }
        }
    }

    void resetState()
    {
        disconnect();
    }

    // API
    void connect(std::string address)
    {
        if (!isInitialized)
        {
            if (enet_initialize() != 0)
            {
                error::throwError("Failed to initialize ENet.");

                return;
            }

            client = enet_host_create(NULL, 1, 1, 0, 0);

            if (client == NULL)
            {
                error::throwError("Failed to create ENet client.");

                return;
            }

            enet_address_set_host(&clientAddress, address.c_str());
            clientAddress.port = DEFAULT_PORT;

            server = enet_host_connect(client, &clientAddress, 1, 0);

            if (server == NULL)
            {
                error::throwError("Failed to connect to server.");

                return;
            }

            isInitialized = true;
        }
    }

    void disconnect()
    {
        if (isConnected)
        {
            enet_peer_disconnect(server, 0);

            isConnected = false;
        }

        if (isInitialized)
        {
            enet_host_destroy(client);
            enet_deinitialize();

            isInitialized = false;
        }
    }

    void sendReliable(data::Data data)
    {
        if (isConnected)
        {
            ENetPacket* packet = enet_packet_create(data.data.data(), data.data.size(), ENET_PACKET_FLAG_RELIABLE);

            enet_peer_send(server, 0, packet);
        }
        else
        {
            logger::warn("Attempted to send data while not connected to server.");
        }
    }

    void sendUnreliable(data::Data data)
    {
        if (isConnected)
        {
            ENetPacket* packet = enet_packet_create(data.data.data(), data.data.size(), ENET_PACKET_FLAG_UNSEQUENCED);

            enet_peer_send(server, 0, packet);
        }
        else
        {
            logger::warn("Attempted to send data while not connected to server.");
        }
    }
}
