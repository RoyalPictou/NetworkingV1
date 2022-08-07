#include "Client.h"



Client::Client()
    : m_LastFloatingClientData( "unwritenData" ) // Risky client data can't input "unwritenData" in his own files

{
    m_Data.existingData = true;
    
    m_Data.configPath = "res/User/client.json";

    m_EventHandler = new EventListener();
    m_ClientFile   = new FileBuffer( m_Data.configPath );
    
#ifdef TESTINGFILEHANDLER
    int counter = 0;

    for ( ;; )
    {
        counter++;
        printf( "Counter %d\n", counter );
        FileHandler::Config_t data = m_ConfigHandler->getConfigData( fileName );
        m_ConfigHandler->addAttribute( "level=10" );
#if 0
        for (auto &entry : data)
        {
            for ( auto &keyTable : entry.second.pairs )
            {
                if(0 == strcmp(keyTable.first.c_str(),"name"))
                {
                    std::string key = keyTable.second;
                    if ( 0 == strcmp( key.c_str(), "Torben" ) )
                    {
                        //keyTable[ key ] = "Torben Schwarzer";
                    }
                }
            }
        }
#endif
        m_ConfigHandler->writeDataToFile( fileName, data );
    }
#endif
}

Client::~Client() {}

std::string
Client::getClientData()
{
    std::string rv = m_FloatingClientData;//std::move( m_FloatingClientData );
    if ( rv.empty() )
    {
        rv = m_ClientFile->GetFileContentString(); // just reads the buffer 
    }
    return rv;
}
bool
Client::hasExistingData()
{
    return m_Data.existingData;
}
std::string
Client::getLastClientData()
{
    return m_LastFloatingClientData;
}
    void Client::setClientData( std::string data)
{
    m_LastFloatingClientData = m_FloatingClientData;
    m_FloatingClientData = data;
    m_Data.existingData      = false;
}
void
Client::setFinalClientData(  )
{
    m_ClientFile->SetFileContentString( m_FloatingClientData );
}
std::string
Client::getCommandData( std::string &comandoPath )
{
    FileBuffer fb( comandoPath );
    return fb.GetFileContentString();
}

void
Client::handle()

{
    bool                       running     = true;
    EventListener::EventType_t eventResult = { 0, 0 };
    do
    {
        // m_ActionHandler->startListenToEvents();
        // m_ActionHandler->findKeyAction();
    } while ( running );
}
