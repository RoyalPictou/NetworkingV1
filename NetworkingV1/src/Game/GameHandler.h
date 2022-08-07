#include <cstdint>
#include "../Network.h"
#ifndef GAMEHANDLER__H_
#define GAMEHANDLER__H_

class GameHandler
{
public:
    
    enum CoreGameState_t
    {
        GAME_MODE_SINGLEPLAYER = 1,
        GAME_TYPE_SINGLEPLAYER_NEW,
        GAME_TYPE_SINGLEPLAYER_LOAD,

        GAME_MODE_MULTIPLAYER,
        GAME_TYPE_MULTIPLAYER_HOST,
        GAME_TYPE_MULTIPLAYER_JOIN,

        GAME_INPUT_REQUIRED_PLAYER_COUNT,
        GAME_INPUT_REQUIRED_PLAYER_NAME,
        

    };
    typedef struct
    {
        std::string host;
        uint8_t realPlayer;
        uint8_t comPlayer;
        uint8_t mode; //Single/ Multi
        uint8_t type;
        uint8_t playerCount;

    } GameSetupData_t;
    GameHandler();
    ~GameHandler();

    void handleStartScreen();
    void createLobbyFromSetup( GameSetupData_t &data );
 private:
    std::vector< Network * > m_Networks;
};
#endif