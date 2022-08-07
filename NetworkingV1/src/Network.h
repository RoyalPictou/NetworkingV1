#ifndef NETWORK__H_
#define NETWORK__H_
//
#include "Loader/Lobby.h"
#include <string>
#include <SDL.h>
#include <SDL_net.h>

#include <vector>
#if 0
#include "Client.h"
#include <SDL.h>

#include <iostream>
#include <cstdio>
#include <thread>
#endif
class Network
{
public:

    enum
    {

        SERVER,
        CLIENT

    };
    #if 0
    typedef struct
    {
        TCPsocket    client;
        std::thread *t;
        IPaddress*    ip;
    } ThreadParamBlock_t;   
    
    typedef std::vector< ThreadParamBlock_t * > ThreadParamBlockArr_t;
#endif
    Network();
    ~Network();

    uint32_t InitSDL(); // returns 1 if Init was successfull

    void createLobby( uint8_t type, std::string host, uint32_t playerCount );

    // returns LobbyId, -1 if failed
    //int32_t createLobby( std::string host, uint32_t playerCount );


    //create Server for players to join, Threadbased
    bool hostLobby( uint32_t id );

    bool joinLobby( uint32_t id );

    void usage( std::string usageString );

    uint32_t ChooseConnectionType( uint32_t argc, char **argv );


    //void startClient();

    //void TaskClient( ThreadParamBlock_t *tpb );

    void startServer();
    TCPsocket createSocket( uint32_t type );

    // Rest we will see
private:
    std::vector< Lobby * > m_Lobbys;
    //std::vector< Client * > m_Clients;
    //TCPsocket m_Server;
    //TCPsocket m_Client;
    
    //bool      m_Done;
    
};
#endif