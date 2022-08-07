#ifndef LOBBY__H_
#define LOBBY__H_

#include "../Client.h"
#include "../ThreadWrapper.h"
#include "../math/SimpleUUID.h"
#include <SDL.h>
#include <SDL_net.h>
#include <cstring>
#include <iostream>
#include <cstdio>

#include <thread>
#include <vector>

class Lobby
{
public:
    enum
    {
        SERVER,
        CLIENT,
        SOCKETERROR =  -1,

        SOCKETNOACTION = 0,


    };
    typedef struct
    {
        void *       server;
        TCPsocket    client;
        std::thread *t;
        IPaddress *  ip;
        bool         online;
    } ThreadParamBlock_t;

    typedef std::vector< ThreadParamBlock_t * > ThreadParamBlockArr_t;
    typedef ThreadParamBlockArr_t::iterator     ThreadParamBlockArrIt_t;
    Lobby();
    Lobby( std::string host, uint32_t playerCount );
    void InitServerThread();
    ~Lobby();

    bool isReadyToStart();

    uint32_t sendData( TCPsocket &tcpsock, std::string &data );

    void recieveData( TCPsocket &tcpsock, std::string &data );

    void startClient();
    void TaskClient( ThreadParamBlock_t *tpb );
    static inline std::string returnCommandoPath( std::string &value );
    //void fun( ThreadParamBlock_t const &value );

private:
    void      startServer();
    TCPsocket createSocket( uint32_t type );

    // Rest we will see
private:
    std::vector< Client * > m_Clients;
    // Server *                m_ServerHandler;
    // ThreadWrapper     *     m_ServerThread;
    std::thread *m_ServerThread;

    // TCPsocket   m_Server;
    // TCPsocket   m_Client;
    std::string m_Host;
    std::string m_LastRecievedData;
    uint32_t    m_PlayerCount;
    bool        m_Ready;
    bool        m_Done;

protected:
    SDLNet_SocketSet m_Set; // socket set
};
#endif