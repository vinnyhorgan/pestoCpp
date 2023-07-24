#include "net.h"

#include <enet/enet.h>

ENetAddress serverAddress;
ENetHost* server;
ENetEvent serverEvent;

std::function<void(std::string, int)> connectCallback;
std::function<void(unsigned char*, int)> receiveCallback;
std::function<void(std::string, int)> disconnectCallback;

namespace net
{
    void registerCallbacks(std::function<void(std::string, int)> connect, std::function<void(unsigned char*, int)> receive, std::function<void(std::string, int)> disconnect)
    {
        connectCallback = connect;
        receiveCallback = receive;
        disconnectCallback = disconnect;
    }

    int init()
    {
        return enet_initialize();
    }

    void deinit()
    {
        enet_deinitialize();
    }

    int createHost(int port, int maxClients)
    {
        serverAddress.host = ENET_HOST_ANY;
        serverAddress.port = port;

        server = enet_host_create(&serverAddress, maxClients, 1, 0, 0);

        if (server == NULL)
        {
            return -1;
        }

        return 0;
    }

    void destroyHost()
    {
        enet_host_destroy(server);
    }

    void service()
    {
        while (enet_host_service(server, &serverEvent, 0) > 0)
        {
            switch (serverEvent.type)
            {
            case ENET_EVENT_TYPE_CONNECT:
            {
                char clientIP[16]; // max size of an IPv4 address is 16 bytes
                enet_address_get_host_ip(&serverEvent.peer->address, clientIP, 16);

                connectCallback(std::string(clientIP), serverEvent.peer->address.port);

                break;
            }
            case ENET_EVENT_TYPE_RECEIVE:
            {
                receiveCallback(serverEvent.packet->data, (int)serverEvent.packet->dataLength);
                enet_packet_destroy(serverEvent.packet);

                break;
            }
            case ENET_EVENT_TYPE_DISCONNECT:
            {
                char clientIP[16];
                enet_address_get_host_ip(&serverEvent.peer->address, clientIP, 16);

                disconnectCallback(std::string(clientIP), serverEvent.peer->address.port);

                break;
            }
            }
        }
    }

    void sendReliable(unsigned char* data, int dataSize)
    {
        ENetPacket* packet = enet_packet_create(data, dataSize, ENET_PACKET_FLAG_RELIABLE);

        enet_host_broadcast(server, 0, packet);
    }

    void sendUnreliable(unsigned char* data, int dataSize)
    {
        ENetPacket* packet = enet_packet_create(data, dataSize, ENET_PACKET_FLAG_UNSEQUENCED);

        enet_host_broadcast(server, 0, packet);
    }

    int getIncomingBandwidth()
    {
        return server->incomingBandwidth;
    }

    int getOutgoingBandwidth()
    {
        return server->outgoingBandwidth;
    }
}
