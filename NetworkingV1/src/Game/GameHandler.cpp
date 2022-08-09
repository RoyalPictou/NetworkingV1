#include "GameHandler.h"
#include <stdio.h>
#include <iostream>

GameHandler::GameHandler() {}

GameHandler::~GameHandler() {}

void
cInput( const char *displayText, std::string &ref )
{
    printf( displayText );
    //std::cin >> ref;
    if ( std::getline( std::cin, ref ) )
    {
        if ( ref.empty() ) // alternative form: if(line.empty())
        {
            ref.push_back( '\0' );
            std::cout << "You pressed enter without typing anything" << std::endl;
        }
    }
}
void
cInput( const char *displayText, uint8_t &ref )
{
    std::string text = "";
    printf( displayText );
    std::cin >> text;
    ref = (uint8_t)atoi( text.c_str() );
}
void
GameHandler::handleStartScreen()
{
    GameSetupData_t data = { "",0, 0, 0, 0, 0 }; // Weird Init

    std::string input   = "";
    bool        working = true;

    do
    {
        //cInput( "Choose between single and multiplayer [s/m]\n", input );
        if (0 == strcmp( input.c_str(), "s" ) || 0 == strcmp( input.c_str(), "S" ) )
        {
            data.mode       = GAME_MODE_SINGLEPLAYER;
            data.realPlayer = 1;
            cInput( "\n Real player set to 1, Choose com Player count: ", data.comPlayer );
            if ( 0 < data.comPlayer )
            {
                data.type = GAME_TYPE_SINGLEPLAYER_NEW; // Todo: Loading game ?
                break;
            }
        }
        else if (true ||  0 == strcmp( input.c_str(), "m" ) || 0 == strcmp( input.c_str(), "M" ) )
        {
            data.mode = GAME_MODE_MULTIPLAYER;
            cInput( "Choose between Host Game and Join Game [h/j]\n", input );
            if ( 0 == strcmp( input.c_str(), "h" ) || 0 == strcmp( input.c_str(), "H" ) )
            {
                data.type = data.type = GAME_TYPE_MULTIPLAYER_HOST;
                working               = false;
            }
            else if ( 0 == strcmp( input.c_str(), "j" ) || 0 == strcmp( input.c_str(), "J" ) )
            {
                data.type = GAME_TYPE_MULTIPLAYER_JOIN;
                working = false;
            }
            //cInput( "Choose real Player count: ", data.realPlayer );
            data.realPlayer = 20;
            data.comPlayer = 0;
            //printf( "Leave Empty for Localhost | " );
            //cInput( "Set IP: ", data.host );
            //if ( data.host.empty() )
            //{
            //    data.host = "127.0.0.1"; // local
            //}
            //cInput( "\nChoose com Player count: ", data.comPlayer ); No computer in multiplayer first 
        }
    } while ( working );
    createLobbyFromSetup( data );
#if 0
    printf( "Choose between New Game and Load Game [n/l]\n" );
    std::cin >> input;
    if ( 0 == strcmp( input.c_str(), "n" ) || 0 == strcmp( input.c_str(), "N" ) )
    {
        data.type = GAME_TYPE_SINGLEPLAYER;
    }
    else if ( 0 == strcmp( input.c_str(), "l" ) || 0 == strcmp( input.c_str(), "L" ) )
    {
        data.type = GAME_TYPE_MULTIPLAYER;
    }
#endif
}

void
GameHandler::createLobbyFromSetup( GameSetupData_t &data )
{
    bool rv = false;
    do
    {
        Network *nw = new Network();
        if ( !nw )
        {
            printf( "Memory Error in Network Class\n" );
            continue;
        }
        // check player count for Computer and Real player
        data.playerCount = data.comPlayer + data.realPlayer;
        if ( data.playerCount < 2 )
        {
            printf( "Not enough player for game creation. Choose at least 2 player\n" );
            break;
        }
        //uint32_t lobbyId = -1; // Error state
        switch ( data.type ) // TODO joinEmptyLobby, createlobby should return corresponding Id
        {
            default:
                printf( "Error in game creation, wrong game Type\n" );
                break;
            case GAME_TYPE_MULTIPLAYER_JOIN:
                nw->createLobby( Network::CLIENT, "127.0.0.1", data.playerCount );
                //nw->createLobby(Network::CLIENT, "192.168.0.90", data.playerCount );
                rv = nw->joinLobby( 0 );
                rv = true;
                break;

            case GAME_TYPE_MULTIPLAYER_HOST:
                nw->createLobby( Network::SERVER, "127.0.0.1", data.playerCount );
                rv = true;
                //rv = nw->joinLobby( 0 );
                break;
            case GAME_TYPE_SINGLEPLAYER_NEW:
                // No network required start game with given data
                printf( "No game action here yet + buggy case \n" );
                rv = true;
                break;
        }
        if (nw)
        {
            m_Networks.push_back( nw );
        }

    } while ( !rv );
}
