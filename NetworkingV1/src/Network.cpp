#include "Network.h"
#include "math/getopt.h"

static struct option long_options[] = // structured options
    {
        { "help", no_argument, 0, 'h' }, //
        { NULL, 0, NULL, 0 }             // end marker
};
Network::Network()
{

    InitSDL();
}

Network::~Network()
{
#if 0
    SDLNet_Quit();
    SDL_Quit();
#endif
}

uint32_t
Network::InitSDL()
{
    uint32_t rv = 0;
    do
    {
        rv = SDL_Init( 0 );
        if ( -1 == rv )
        {
            printf( "SDL_Init: %s\n", SDL_GetError() );
            break;
        }
        rv = SDLNet_Init();
        if ( -1 == rv )
        {
            printf( "SDLNet_Init: %s\n", SDLNet_GetError() );
            break;
        }
        rv = 1;
    } while ( false );

    return rv;
}

void
Network::createLobby( uint8_t type, std::string host, uint32_t playerCount )
{
    int32_t rv       = -1;
    Lobby * newLobby = new Lobby( host, playerCount );

    if ( newLobby )
    {
        rv = (int32_t)m_Lobbys.size();
        m_Lobbys.push_back( newLobby );

        switch ( type )
        {
            default:
                break;

            case SERVER:
                newLobby->InitServerThread();
                break;

            case CLIENT:

                break;
        }
    }

    //return rv;
}
bool
Network::hostLobby( uint32_t id )
{
    size_t dataLen = m_Lobbys.size();
    bool   rv      = false;
    if ( id < dataLen )
    {

        Lobby *lobbyPtr = m_Lobbys.at( id );
        lobbyPtr->InitServerThread();
        rv = true; // Sense?
    }
    return rv;
}

bool
Network::joinLobby( uint32_t id )
{
    bool   rv         = false;
    size_t lobbyCount = m_Lobbys.size();
    if ( id < lobbyCount )
    {
        Lobby *lobbyPtr  = m_Lobbys.at( id );
        bool   fullLobby = lobbyPtr->isReadyToStart();
        if ( !fullLobby )
        {
            lobbyPtr->startClient();
            rv = true;
        }
    }
    else
    {
        printf( "Lobby not found\n" );
    }
    return rv;
}
void
Network::usage( std::string usageString )
{
    printf( "Usage:\n" );
    printf( "%s [-s] [-h|-?]\n", usageString.c_str() );
    printf( "-s : Server Mode\n" );
}
uint32_t
Network::ChooseConnectionType( uint32_t argc, char **argv )
{
    uint32_t rv = 0;
    int      opt;
    //uint32_t verbose      = 0;
    bool     optEnd       = false;
    int      option_index = 0;
    uint32_t mode         = CLIENT;

    while ( !optEnd )
    {
        opt = getopt_long( argc, argv, "sc", long_options, &option_index );
        switch ( opt )
        {
            case -1:
                optEnd = true;
                break;
            case 0:
            {
                if ( long_options[ option_index ].flag != 0 )
                {
                    break;
                }
                break;
            }
            case 's':
            {
                mode = SERVER;
                break;
            }
            case 'c':
            {
                mode = CLIENT;
                break;
            }
            case '?':
            case 'h':
            {
                usage( argv[ 0 ] );
                exit( 0 );
                break;
            }
            default:
            {
                break;
            }
        }
    }
    switch ( mode )
    {
        default:
        case CLIENT:
            // startClient();
            break;
        case SERVER:
            // startServer();
            break;
    }
    return rv;
}
#if 0
TCPsocket
Network::createSocket( uint32_t type )
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
                check = SDLNet_ResolveHost( &ip, "127.0.0.1", 9999 );
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

            SDL_Delay( 500 );
            continue;
        }
        check = 1;
    } while ( !check );
    return tcpsock;
}

void
Network::startClient()
{
    TCPsocket tcpsock = createSocket( CLIENT );

    Client *client = new Client();
    printf( "Starting client...\n" );

    while ( 1 )
    {
        printf( "message: " );
        char message[ 1024 ];
        fgets( message, 1024, stdin );
        int len = strlen( message );

        /* strip the newline */
        message[ len-1 ] = '\0';

        if ( len )
        {
            int result;

            /* print out the message */
            // printf( "Sending: %.*s\n", len, message );
            //printf( "Client send data\n" );
            result = SDLNet_TCP_Send( tcpsock, message, len  ); /* add 1 for the NULL */
            if ( result < len )
                printf( "SDLNet_TCP_Send: %s\n", SDLNet_GetError() );
#if 1
            //TCPsocket tcpsockServer = createSocket( SERVER );
            
            len = SDLNet_TCP_Recv( tcpsock, message, 1024 );

            if ( 0 > len )
            {
                printf( "SDLNet_TCP_Recv: %s\n", SDLNet_GetError() );
                break;
            }
            printf( "Answer to client: %s\n", message );
#endif
        }

        if ( len == 2 && tolower( message[ 0 ] ) == 'q' )
        {
            break;
        }
    }

    SDLNet_TCP_Close( tcpsock );
}

void
Network::TaskClient( ThreadParamBlock_t *tpb )
{
    /* get the clients IP and port number */
    IPaddress *remoteip;
    remoteip = SDLNet_TCP_GetPeerAddress( tpb->client );
    if ( remoteip )
    {
        /* print out the clients IP and port number */
        Uint32 ipaddr;
        ipaddr = SDL_SwapBE32( remoteip->host );
        printf( "Accepted a connection from %d.%d.%d.%d port %hu\n", ipaddr >> 24, ( ipaddr >> 16 ) & 0xff, ( ipaddr >> 8 ) & 0xff, ipaddr & 0xff, remoteip->port );
        tpb->ip = remoteip;
        for ( ;; )
        {
            /* read the buffer from client */
            char message[ 1024 ];
            //printf( "ClientSocket: %p\n", tpb->client );
            int len = SDLNet_TCP_Recv( tpb->client, message, 1024 );

            if ( 0 > len )
            {
                printf( "SDLNet_TCP_Recv: %s\n", SDLNet_GetError() );
                break;
            }
           
            char *msg = (char*)"hallo\n";//            itoa( tpb->ip->host, ( char * ) "Hallo ", 1 ); 
            /* print out the message */
            len        = strlen( msg ) + 1;
            int result = SDLNet_TCP_Send( tpb->client, msg, len ); /* add 1 for the NULL */
            if ( result < len )
                 printf( "SDLNet_TCP_Send: %s\n", SDLNet_GetError() );

            printf( "Received: %d %s\n", len, message );
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
        }
    }
    else
    {
        printf( "SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError() );
    }
    SDLNet_TCP_Close( tpb->client );
}

void
Network::startServer()
{
    printf( "Starting server...\n" );
    TCPsocket             server, client;
    ThreadParamBlockArr_t tpba;
    IPaddress             ip;
    server = createSocket( SERVER );
#if 0
    if ( SDLNet_ResolveHost( &ip, NULL, 9999 ) == -1 )
    {
        printf( "SDLNet_ResolveHost: %s\n", SDLNet_GetError() );
        exit( 1 );
    }
    server = SDLNet_TCP_Open( &ip );
    if ( !server )
    {
        printf( "SDLNet_TCP_Open: %s\n", SDLNet_GetError() );
        exit( 2 );
    }
#endif
    while ( !m_Done )
    {
        /* try to accept a connection */
        client = SDLNet_TCP_Accept( server );
        if ( !client )
        { /* no connection accepted */
            /*printf("SDLNet_TCP_Accept: %s\n",SDLNet_GetError()); */
            SDL_Delay( 100 ); /*sleep 1/10th of a second */
            continue;
        }
        ThreadParamBlock_t *tpb = new ThreadParamBlock_t;
        tpba.push_back( tpb );
        tpb->client = client;
        tpb->t      = new std::thread( &Network::TaskClient, this, tpb );
        // std::thread t1( TaskClient, tpb );
        // t1.join();
    }
}
#endif