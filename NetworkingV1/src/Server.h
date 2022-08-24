#ifndef SERVER__H_
#define SERVER__H_
#include <string>
#include "math/SimpleUUID.h"
#include "File/FileHandler.h"
#include <unordered_map>
class Server
{
public:
    enum
    {
        TYPE_UNKNOWN,
        TYPE_PUBLIC,
        TYPE_PRIVATE,
        TYPE_ALL,
        MEMORY_ERROR,
        GENERATE_PLAYER_UUID = 1,
        FIND_PLAYER_UUID,
        GATHER_CLIENT_DATA,
        LOAD_SERVER_DATA,
        RETRIEVE_ALLOWED_SERVER_DATA,
        KEY_LIST_NOACTION,
        KEY_LIST_MARKED,
        KEY_LIST_EMPTY,
    };
    typedef std::unordered_map< std::string ,std::string> StringMap_t;
    typedef StringMap_t::iterator StringMapIt_t;
    typedef std::vector< std::string >           StringVec_t;
    typedef StringVec_t::iterator                                    StringVecIt_t;
    typedef std::pair< const std::string, FileHandler::DataEntry_t > EntryPair_t;
    typedef struct
    {
        
        FileHandler::DataEntry_t      * data;
        FileHandler::StringPairMap_t  * pairMap;
        FileHandler::StringSingleMap_t * singleMap;
        std::string                     key;
        size_t                          singleCount;
        size_t                          pairCount;
        uint8_t                         type;
        uint8_t                         action;
        uint8_t                         command;
        uint8_t                         placeholder;
        
    } Entry_t;

    typedef struct
    {
        std::vector< std::string > keyMulti;
        std::string keyPair;//only needed if we erase keys in a specific array
        uint32_t action;

    }Key_t;
    Server();
    ~Server();

    void readData( std::string &data );

    std::string getData();

    //void createEntry();

    //bool createEntry( Entry_t &data ); // returns true if success
    Key_t       findKey( StringMap_t &keyMap, std::string key );
    StringMap_t                getKeys( std::string key, FileHandler::DataEntry_t &userEntryFromUUID, Entry_t *entry );
    
    bool createEntry( EntryPair_t entry, Entry_t *data );

    bool performAction( uint32_t actionType );

    bool performAction2(uint32_t actionType);

private:
    FileHandler *           m_File;
    FileHandler::Config_t   m_ServerData; // saving Data
    FileHandler::Config_t   m_UserData;   // retrieved Data from client message
    FileHandler::ConfigIt_t m_UserIt;     // until now useless
    FileHandler::Config_t   m_OutputData;
    std::string             m_OutputDataString; // Client get this data
    Entry_t *               m_LocalEntry;
};

#endif