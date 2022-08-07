
#ifndef ACTIONHANDLER__H_
#define ACTIONHANDLER__H_

#include "File/FileHandler.h"

#include "EventListener.h"
#include <stdint.h>
#include <vector>
#include <string>
class ActionHandler
{

public:
    typedef struct
    {
    } History_t;
    typedef struct
    {
        std::string                action;
        uint32_t                   key;
        std::vector< History_t * > history;
    } Action_t;
    enum States
    {
        GAME_ACTIVE_PLAYER_SHOOT_SLOT_1,
        GAME_ACTIVE_PLAYER_SHOOT_SLOT_2,
        GAME_ACTIVE_PLAYER_SHOOT_SLOT_3,
        GAME_ACTIVE_PLAYER_MOVE_FORWARD,
        GAME_ACTIVE_PLAYER_MOVE_BACKWARD
    };
    ActionHandler();
    ~ActionHandler();

    void startListenToEvents();

    void findKeyAction();

private:
    //FileHandler *             m_KeyDataHandler; // reads SDL_KeyData from File
    ActionHandler *           m_ActionHandler;
    //FileHandler::Config_t     m_KeyDataHex;
    EventListener::EventType_t m_EventResult;
    std::vector< Action_t * > m_Data;
};
#endif