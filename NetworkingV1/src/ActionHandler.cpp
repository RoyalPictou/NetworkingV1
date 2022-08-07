#include "ActionHandler.h"
#include <iostream>


ActionHandler::ActionHandler() {



    //m_KeyDataHandler = new FileHandler( "res/Server" );

    //m_KeyDataHex = m_KeyDataHandler->getConfigData( "SDLKeyData.csv" );
}
ActionHandler::~ActionHandler() {}


void ::ActionHandler::findKeyAction() 
{
    //FileHandler::ConfigIt_t it = m_KeyDataHex.find( std::to_string( m_EventResult.environmentType ) );
    //FileHandler::StringPairMap_t valueMap;
    //if (m_KeyDataHex.end() == it)
    //{
        #ifdef REFACTOR
        valueMap = it->second.value;   
        for (auto &entry : valueMap)
        {
            std::vector< std::string > keys = entry.second;
            size_t                     keysCount = keys.size();
            for (size_t i = 0; i < keysCount; ++i)
            {
                
}
        }
        #endif

    //}
}
void
ActionHandler::startListenToEvents()
{
    // EventListener::EventType_t eventData = m_EventHandler->checkEvents();
    EventListener::EventType_t eventData;
    printf( "Choose WindowStates: available are \n" );

    printf( "WINDOW_UNKOWN    = 0, \n" );
    printf( "WINDOW_QUIT      = 1, \n" );
    printf( "WINDOW_INACTIVE  = 2, \n" );
    printf( "WINDOW_ACTIVE    = 3, \n" );
    printf( "WINDOW_REDRAW    = 4, \n" );
    printf( "WINDOW_MAXIMIZED = 5, \n" );
    printf( "WINDOW_ACTION,        \n" );

    std::cin >> eventData.windowType;
    printf( "Choose EnvironmentStates: available are \n" );

    printf( "IDLE,            = 0\n" );
    printf( "ACCELERATE_LEFT, = 1\n" );
    printf( "ACCELERATE_RIGHT,= 2\n" );
    printf( "SHOOT_MISSILE,   = 3\n" );
    printf( "ANGLE_TURN_RIGHT,= 4\n" );
    printf( "ANGLE_TURN_LEFT, = 5\n" );
    std::cin >> eventData.environmentType; //
    m_EventResult = eventData;
    //return eventData;
}