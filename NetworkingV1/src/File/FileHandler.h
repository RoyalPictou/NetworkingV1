#ifndef FILEHANDLER__H_
#define FILEHANDLER__H_
//#include "../myTypes.h"
//#include "TrimHelper.h"

#include "../json/jsonParser.h"

#include <string>
#include <unordered_map>
#include <vector>
//#include "json/json.hpp"
class FileHandler
{
public:
    enum
    {
        MAX_COLUMNS = 20,
    };
#if 0
    struct FileData_t
    {
        template < typename T > static const T pair; // declaration of a static data member template
        template < typename T > static const std::vector< T > arrayPair;
    };
#else
    typedef std::unordered_map< std::string, std::string > StringSingleMap_t;
    typedef StringSingleMap_t::iterator                    StringSingleMapIt_t;
    typedef struct
    {
        StringSingleMap_t values;
        //std::vector< std::string > values;
        
    } DataArray_t;
    typedef std::unordered_map< std::string, DataArray_t > StringPairMap_t;
    typedef StringPairMap_t::iterator                      StringPairMapIt_t;
    typedef struct
    {
        StringPairMap_t   arrayEntry;
        StringSingleMap_t singleEntry;
        
    } DataEntry_t;

#endif
    typedef std::unordered_map< std::string, DataEntry_t > Config_t;
    typedef Config_t::iterator                             ConfigIt_t;
    FileHandler( std::string path );
    FileHandler( std::string path, char delimiter, std::string ending );
    void Init();
    ~FileHandler();

    bool writeDataToFile( std::string fileName, Config_t &data );

    bool createEmptyFile( std::string fileName );

    bool writeDataToFile( std::string fileName, const char *data );

    std::string readDataFromFile( std::string fileName );

    Config_t readJsonData( std::string data );

    FileHandler::Config_t getJsonData( json_t const *json );

    void writeJsonData( std::string filename, FileHandler::Config_t data );

    void dump( json_t const *json );

    StringPairMap_t packLineData( std::string &line, std::string &mapKey );

    //FileHandler::Config_t getConfigData( std::string fileName );

    // uint32_t addAttribute( std::string input );

private:
    bool isPartOf( const char *a, char *b );

    std::string getCellRemainingSpaces( uint32_t pos, uint32_t cellWidth );

private:
    // Config_t                            m_FileData;
    // TrimHelper *            m_Trimer;
    jsonParser *            m_JsonParser;
    json_t const *          m_Json;
    Config_t                m_FileData;
    DataEntry_t              m_DataEntry;
    DataArray_t              m_DataArray;
    StringSingleMap_t      m_ArrayMap;
    std::string             m_ArrayKey;
    std::string             m_NewEntryName;
    uint32_t                m_GlobalCounter;
    std::vector< uint32_t > m_ColumnSpacing;
    std::string             m_Path;
    std::string             m_Ending;
    char                    m_Dilimiter;
    uint32_t                m_ParamCount;
    uint32_t                m_Spacing;
};

#endif
