
#ifndef CLIENT__H_
#define CLIENT__H_
//#include "ActionHandler.h" maybe needed later
#include "File/FileHandler.h"
#include "File/FileBuffer.h"
#include "EventListener.h"
#include "math/SimpleUUID.h"
#include <cstdint>
#include <string>
#include <vector>
class Client
{
    enum Connection_t
    {
        TYPE_TCP,
        TYPE_UDP
    };

    typedef struct
    {

        // std::vector< Action_t * > actions;
        std::string  configPath;
        uint32_t     warningLevel;
        uint32_t     statesCount;
        Connection_t type;
        uint32_t     hostAddress;
        uint32_t     joinedAt;
        bool         existingData;
    } Client_t;

public:
    Client();
    ~Client();

    std::string getClientData();
    bool        hasExistingData();
    std::string getLastClientData();
    void        setClientData( std::string data );

    void setFinalClientData();

    std::string getCommandData( std::string &comandoPath );

    void handle();


private:
    //FileHandler *  m_ConfigHandler;
    FileBuffer  *   m_ClientFile;
    EventListener *m_EventHandler;
    //ActionHandler *m_ActionHandler;
    Client_t       m_Data;
    FileHandler::Config_t m_CCMData; //client communicationdata
    std::string           m_FloatingClientData; // stores data while runtime and stores it when exiting the game

    std::string m_LastFloatingClientData;
};
#endif
