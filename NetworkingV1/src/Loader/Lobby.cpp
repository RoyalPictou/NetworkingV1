#include "Lobby.h"
#include "../TrimHelper.h"
#include "../Server.h"
#include <algorithm>
#define COMMUNICATION
Lobby::Lobby()
    : m_Done( false )
    , m_PlayerCount( 2 )
    , m_Host( "127.0.0.1" )
{

    m_Set = SDLNet_AllocSocketSet( m_PlayerCount );
    if ( !m_Set )
    {
        printf( "SDLNet_AllocSocketSet: %s\n", SDLNet_GetError() );
        exit( 1 ); // most of the time this is a major error, but do what you want.
    }
    // Init();
}

Lobby::Lobby( std::string host, uint32_t playerCount )
    : m_Done( false )
    , m_PlayerCount( playerCount )
    , m_Host( host )
{
    m_Set = SDLNet_AllocSocketSet( m_PlayerCount );
    if ( !m_Set )
    {
        printf( "SDLNet_AllocSocketSet: %s\n", SDLNet_GetError() );
        exit( 1 ); // most of the time this is a major error, but do what you want.
    }
    // Init();
}

void
Lobby::InitServerThread()
{
#if 1
    m_ServerThread = new std::thread( &Lobby::startServer, this );
    // m_ServerThread( &Lobby::startServer, this );
#else
    std::function< void() > fc = std::bind( &Lobby::startServer, this );

    // std::function< void( void ) > f = std::bind( &Lobby::startServer, this );
    // std::function< void() > f = [ = ]( ) { this->startServer(); }

    // auto                    callable = std::mem_fn( &Lobby::startServer );
    // std::function< void(  ) > binding  = std::bind( callable, this);
    // std::move( obj.m_ThreadHandler );
    m_ServerThread = new ThreadWrapper( fc );
#endif
}
Lobby::~Lobby()
{
    SDLNet_Quit();
    SDL_Quit();
}

bool
Lobby::isReadyToStart()
{
    return m_Ready;
}

TCPsocket
Lobby::createSocket( uint32_t type )
{
    int32_t   check   = -1;
    IPaddress ip      = { 0, 0 };
    TCPsocket tcpsock = NULL;
    do
    {
        switch ( type )
        {
            default:
                break;
            case CLIENT:
                check = SDLNet_ResolveHost( &ip, m_Host.c_str(), 9999 );
                break;
            case SERVER:
                check = SDLNet_ResolveHost( &ip, NULL, 9999 );
                break;
        }

        if ( -1 == check )
        {
            printf( "SDLNet_ResolveHost: %s\n", SDLNet_GetError() );
            continue;
        }

        tcpsock = SDLNet_TCP_Open( &ip );
        if ( !tcpsock )
        {
            printf( "SDLNet_TCP_Open: %s\n", SDLNet_GetError() );
            break;
            // SDL_Delay( 500 );
            // continue;
        }
        check = 1;
    } while ( !check );
    return tcpsock;
}
#define PACKAGESTART ( 0 )

uint32_t
Lobby::sendData( TCPsocket &tcpsock, std::string &data )
{
    uint32_t rv      = 0;
    size_t   dataLen = data.length();
    if ( 0 == dataLen )
        return rv;
    rv                                     = dataLen;
    size_t         dataSendLen             = dataLen;
    size_t         dataRestLen             = dataLen;
    std::string    message                 = "";
    const uint32_t maxinputLen             = 20;
    std::string    multipartInstruction    = "";
    size_t         multipartInstructionLen = 0;
    if ( dataLen > maxinputLen )
    {
        // Multipart Message
        // Provide in first package MultipartKey with package count
        uint32_t packages = dataLen / maxinputLen + 1;
        // Multipart:Filesize->nextline Data
        message.append( "Multipart:" + std::to_string( packages ) );
        multipartInstructionLen = message.length();
        // reduce Datapackage size for first package because of the multipartInstruction data
#ifdef SMALLBUFFER
        if ( multipartInstructionLen > maxinputLen )
        {
            puts( "Error in Lobby::sendData -> Buffer to small for MultipartKey" );
        }
#endif
        dataSendLen = maxinputLen - multipartInstructionLen;
        dataRestLen = dataSendLen;
    }
    do
    {
        // Prepare package for transmission
        message += data.substr( 0, dataSendLen );
        data = data.substr( dataRestLen );
        // printf( "%s", message.c_str() );
        message.clear();
        // Evaluate remaining data
        dataLen = data.length();
        if ( 0 == dataLen )
        {
            break; // sending done exit loop TODO evaluate rv from sockets
        }
        if ( dataLen > maxinputLen )
        {
            dataSendLen = maxinputLen;
            dataRestLen = maxinputLen;
        }
        else
        {
            dataRestLen = dataLen;
        }
    } while ( true );

#if 0
if ( dataLen > maxinputLen )
    {
        dataSendLen = maxinputLen;
    }
    uint32_t       packages = dataLen * ( 1 / dataSendLen );
    if ( 0 == packages )
    {
        printf( "Error in DataTransmission, see Lobby->sendData\n" );
    }
    uint32_t end = ( 1 == packages ) ? 1 : ( packages - 1 );
   // const uint32_t packageEnd = std::move(end);
    SimpleUUID     uid;
    uint8_t       uidLen = (uint8_t) std::to_string( uid ).length();
    std::string    transmissionData;
    for ( uint32_t i = 0; i < packages; ++i )
    {
        if ( end == i )
        {
            transmissionData = data.substr( dataSendLen * i );
            break;
        }
        if ( PACKAGESTART == i )
        {
            transmissionData = data.substr( 0, dataSendLen * i );
            continue;
        }

    }

    do
    {
        // dataLen            = data.length();
        dataSendLen = dataLen;
        message     = "";
        if ( dataLen > maxinputLen )
        {
            // Multipart:Filesize->nextline Data
            dataSendLen = maxinputLen;
        }
        message = data.substr( 0, dataSendLen );
        data    = data.substr( dataSendLen - 1 );
        dataLen -= dataSendLen;
        rv = ( int32_t ) SDLNet_TCP_Send( tcpsock, message.c_str(), dataSendLen + 1 ); /* add 1 for the NULL */
        if ( rv < dataSendLen )
        {
            printf( "SDLNet_TCP_Send: %s\n", SDLNet_GetError() );
            break;
        }
    } while ( dataLen > 0 );
#endif
    return rv;
}
void
Lobby::recieveData( TCPsocket &tcpsock, std::string &data )
{
    uint32_t rv = 0;
    // size_t         dataLen     = 0;
    const uint32_t maxinputLen = 20;
    char           message[ maxinputLen ];
    do
    {
        rv = SDLNet_TCP_Recv( tcpsock, message, maxinputLen + 1 ); /* add 1 for the NULL */
        if ( message )
        {
            data.append( message );
        }

        if ( rv < maxinputLen )
        {
            // printf( "SDLNet_TCP_Send: %s\n", SDLNet_GetError() );
            break;
        }
    } while ( true );
}
std::string
Lobby::returnCommandoPath( std::string &value )
{
    std::string rv        = "res/Command/";
    const char  space     = ' ';
    const char  delimiter = '/';
    // size_t      valueLen    = value.length();
    // size_t      lastSpace   = 0;
    std::size_t doubleSpace = value.find( "  " );
    while ( doubleSpace != std::string::npos )
    {
        value.erase( doubleSpace, 1 );
        doubleSpace = value.find( "  " );
    }
    TrimHelper th;
    th.trimLeftRight( value );
    std::replace( value.begin(), value.end(), ' ', delimiter );
    value.append( ".json" );
    return rv.append( std::move( value ) );
}
#include <time.h>
void
Lobby::startClient()
{
    TCPsocket tcpsock = createSocket( CLIENT );
    int32_t   numused = SDLNet_TCP_AddSocket( m_Set, tcpsock );
    if ( numused == -1 )
    {
        printf( "SDLNet_AddSocket: %s\n", SDLNet_GetError() );
        // perhaps you need to restart the set and make it bigger...
    }
    do
    {
        if ( !tcpsock )
        {
            break;
        }
        Client *client = new Client();
        if ( !client )
        {
            break;
        }
        printf( "Starting client...\n" );
        int32_t rv = 0;

        const uint32_t maxinputLen = 1024;
        // TCPsocket      tcpsockServer = createSocket( SERVER );
        std::string comandoPath = ""; // great idea
        do
        {
            std::string value = "";
            std::string data  = "";
            if ( client->hasExistingData() )
            {

                // std::string lastData = client->getLastClientData();
                data = client->getClientData();
                client->setClientData( data );
                // if ( 0 == data.compare( lastData ) )
                //{
                //    continue;
                //}
            }
            else
            {
                printf( "Comando: " );
                std::getline( std::cin, value );
                if ( ( 1 == value.length() ) && ( tolower( value.at( 0 ) ) == 'q' ) )
                {
                    break;
                }
                comandoPath = returnCommandoPath( value );
                value.clear();
                // printf( "Path: %s\n", comandoPath.c_str() );
                data = client->getCommandData( comandoPath );
                if ( data.empty() )
                {
                    continue;
                }
            }
            TrimHelper *th = new TrimHelper();
            if ( th )
            {
                th->findAndReplaceAll( data, "\r\r", "\r" );
                th->findAndReplaceAll( data, "\n\n", "\n" );
                th->findAndReplaceAll( data, "\r", "\r\n" );
                delete ( th );
                th = NULL;
            }
            comandoPath.clear(); // make sure if client gets new input existing data gets executed properly
            // rv = SDLNet_TCP_Send( tcpsock, data.c_str(), (int32_t)data.length() + 1 ); /* add 1 for the NULL */
            clock_t tStart = clock();
            rv             = sendData( tcpsock, data );
            printf( "Time taken: %.2fs\n", ( double ) ( clock() - tStart ) / CLOCKS_PER_SEC );
            printf( "\nSEND - Return value: %d\n%s\n", rv, data.c_str() );
            char message[ maxinputLen ];
            rv = SDLNet_TCP_Recv( tcpsock, message, maxinputLen + 1 ); /* add 1 for the NULL */

            if ( -1 == rv )
            {
                rv = 0;
                // printf( "Error occured, closing connection\n" );
                // break;
            }
            message[ rv ] = '\0';
            // recieveData( tcpsockServer, data );
            printf( "RECIEVE - Return value: %d\n%s\n", rv, message );
            client->setClientData( message );
            // if ( len == 2 && tolower( message[ 0 ] ) == 'q' )
        } while ( true );
        client->setFinalClientData();
    } while ( false );

    SDLNet_TCP_Close( tcpsock );
}

void
Lobby::TaskClient( ThreadParamBlock_t *tpb )
{

    Server *serverHandler = new Server();
    if ( !serverHandler )
    {
        printf( "Server memoryLeak\n" );
        return;
    }
    // int numready;

    /* get the clients IP and port number */
    IPaddress *remoteip;
    remoteip = SDLNet_TCP_GetPeerAddress( tpb->client );
    if ( remoteip )
    {
        /* print out the clients IP and port number */
        Uint32 ipaddr;
        ipaddr = SDL_SwapBE32( remoteip->host );
        printf( "Accepted a connection from %d.%d.%d.%d port %hu\n", ipaddr >> 24, ( ipaddr >> 16 ) & 0xff, ( ipaddr >> 8 ) & 0xff, ipaddr & 0xff, remoteip->port );
        tpb->ip     = remoteip;
        tpb->online = true;
        for ( ;; )
        {

            /* read the buffer from client */
            char message[ 1024 ];
            // printf( "ClientSocket: %p\n", tpb->client );
            int32_t len = SDLNet_TCP_Recv( tpb->client, message, 1024 );
            if ( 0 == len )
            {
                tpb->online = false;
                printf( "Player left the server\n" );
                break;
            }
            if ( 1 > len )
            {
                printf( "SDLNet_TCP_Recv: %s\n", SDLNet_GetError() );
                break;
                // continue;
            }
            printf( "Received: %d %s\n", len, message );
            std::string msgOut( message );
            serverHandler->readData( msgOut );

#if 1 // send data back to client
            msgOut    = serverHandler->getData();
            char *msg = ( char * ) ( msgOut.c_str() ); //            itoa( tpb->ip->host, ( char * ) "Hallo ", 1 );
            /* print out the message */
            len = ( int32_t ) msgOut.length();
            if ( 0 == len || len > 1023 ) // lose number!!!
            {
                printf( "Invalid answer length/data of server\n" ); // TODO
                break;
            }
            int result = SDLNet_TCP_Send( tpb->client, msg, len ); /* add 1 for the NULL */
            if ( result < len )
                printf( "SDLNet_TCP_Send: %s\n", SDLNet_GetError() );
#endif
#if 0
            if ( message[ 0 ] == 'q' )
            {
                printf( "Disconecting on a q\n" );
                break;
            }
            if ( message[ 0 ] == 'Q' )
            {
                printf( "Closing server on a Q.\n" );
                m_Done = true;
                break;
            }
#endif
        }
    }
    else
    {
        printf( "SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError() );
    }
    SDLNet_TCP_Close( tpb->client );
}
struct ThreadAnalyser
{ // function object type:
    void
    operator()( Lobby::ThreadParamBlock_t *task )
    {

        // Lobby::ThreadParamBlock_t task = it.;
    }
} ThreadAnalyser_t;

void
Lobby::startServer()
{
    // Server *  serverHandler;
    TCPsocket server;
    server          = createSocket( SERVER );
    int32_t numused = 0;

    do
    {
        if ( !server )
        {
            printf( "Server Socket null \n" );
            break;
        }
        numused = SDLNet_TCP_AddSocket( m_Set, server );
        if ( SOCKETERROR == numused )
        {
            printf( "SDLNet_AddSocket: %s\n", SDLNet_GetError() );
            // perhaps you need to restart the set and make it bigger...
            break;
        }
        printf( "Starting server...\n" );

        ThreadParamBlockArr_t tpba;
        TCPsocket             client;
        while ( !m_Done )
        {
            /* try to accept a connection */
            if ( m_Ready )
            {
                printf( "Waiting for game to start\n" );
                continue;
            }
            // client = SDLNet_TCP_Accept( server );
            int numready = SDLNet_CheckSockets( m_Set, 0 );
            switch ( numready )
            {
                case SOCKETNOACTION:
                    std::for_each( std::begin( tpba ), std::end( tpba ), ThreadAnalyser_t );
                    break;
                case SOCKETERROR:
                    printf( "SDLNet_CheckSockets: %s\n", SDLNet_GetError() );
                    // most of the time this is a system error, where perror might help you.
                    perror( "SDLNet_CheckSockets" );
                    break;
                default:
                    printf( "There are %d sockets with activity!\n", numready );
                    // check all sockets with SDLNet_SocketReady and handle the active ones.
                    if ( SDLNet_SocketReady( server ) )
                    {
                        client = SDLNet_TCP_Accept( server );
                        if ( client )
                        {
                            ThreadParamBlock_t *tpb = new ThreadParamBlock_t;
                            tpba.push_back( tpb );
                            // size_t playerCount = tpba.size();
                            // printf( "PlayerCount: %i \n", ( uint32_t ) playerCount );
                            if ( tpba.size() >= m_PlayerCount )
                            {
                                m_Ready = true; // accept no more connection !
                            }
                            tpb->client = client;
                            tpb->t      = new std::thread( &Lobby::TaskClient, this, tpb );
                        }
                        else
                        {

                            SDL_Delay( 100 );
                        }
                    }
                    break;
            }
#if 0
            if ( !client )
            {
                // Do server stuff
                size_t activeThreads = tpba.size();
                // Using for_each algorithm, using a function or functor:
                
                int numready = SDLNet_CheckSockets( m_Set, 2000 );
                if ( numready == -1 )
                {
                    printf( "SDLNet_CheckSockets: %s\n", SDLNet_GetError() );
                    // most of the time this is a system error, where perror might help you.
                    perror( "SDLNet_CheckSockets" );
                }
                else if ( numready )
                {
                    printf( "There are %d sockets with activity!\n", numready );
                    // check all sockets with SDLNet_SocketReady and handle the active ones.
                }
                std::for_each( std::begin( tpba ), std::end( tpba ), ThreadAnalyser_t );


                SDL_Delay( 100 );
                continue;
            }
            ThreadParamBlock_t *tpb = new ThreadParamBlock_t;
            tpba.push_back( tpb );
            //size_t playerCount = tpba.size();
            //printf( "PlayerCount: %i \n", ( uint32_t ) playerCount );
            if ( tpba.size() >= m_PlayerCount )
            {
                m_Ready = true; // accept no more connection !
            }
            tpb->client = client;
            tpb->t = new std::thread( &Lobby::TaskClient, this, tpb );
#endif
        }
    } while ( false );
}
