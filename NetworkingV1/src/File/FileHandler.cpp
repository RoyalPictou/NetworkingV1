
#define _CRT_SECURE_NO_WARNINGS
#include "FileHandler.h"
#include "FileBuffer.h"
#include "../json/json.hpp"
//#include "TrimHelper.h"
#include <fstream>
#include <iostream>
#include <sstream>
using json::JSON;
FileHandler::FileHandler( std::string path )
    : m_Dilimiter( ';' )
    , m_ParamCount( 1 )
    , m_Spacing( 20 )
    , m_Path( path )
    , m_Ending( ".json" )
{
    Init();
}

FileHandler::FileHandler( std::string path, char delimiter, std::string ending )
    : m_Dilimiter( delimiter )
    , m_Path( path )
    , m_Ending( ending )
{
    Init();
}

FileHandler::~FileHandler() {}
// Takes the next position and the end of a buffer as first two arguments and the
// character to convert from ISO-Latin-1 as third argument.
// Returns a pointer to end of the produced sequence.
char *
iso_latin_1_to_utf8( char *buffer, char *end, unsigned char c )
{
    if ( c < 128 )
    {
        if ( buffer == end )
        {
            throw std::runtime_error( "out of space" );
        }
        *buffer++ = c;
    }
    else
    {
        if ( end - buffer < 2 )
        {
            throw std::runtime_error( "out of space" );
        }
        *buffer++ = 0xC0 | ( c >> 6 );
        *buffer++ = 0x80 | ( c & 0x3f );
    }
    return buffer;
}
void
FileHandler::Init()
{
    for ( uint32_t i = 0; i < MAX_COLUMNS; ++i )
    {
        m_ColumnSpacing.push_back( m_Spacing );
    }
    size_t pathLen = m_Path.length();
    if ( 0 == pathLen )
    {
        return;
    }
    size_t lastSeperator = m_Path.find_last_of( "/" );

    if ( lastSeperator != pathLen )
    {
        m_Path += "/";
    }
}

std::string
FileHandler::readDataFromFile( std::string fileName )
{
    std::string path = m_Path + fileName;
    FileBuffer  fb( path );
    std::string rv = fb.GetFileContentString();
#if 1

#else

    std::vector< std::string > rv;
    std::string                line = "";
    std::stringstream          appendFileToWorkWith;
    appendFileToWorkWith << content;
    while ( std::getline( appendFileToWorkWith, line ) )
    {
        if ( line.empty() )
        {
            continue;
        }
        rv.push_back( line );
    }
#endif

    return rv;
}
FileHandler::Config_t
FileHandler::readJsonData( std::string data )
{
    Config_t    rv;
    //std::string ss = readDataFromFile( filename );
    m_JsonParser   = new jsonParser( data.c_str() );
    m_Json         = m_JsonParser->getRoot();
    if ( m_Json )
    {
        // getJsonData(m_Json);
        m_GlobalCounter = 0;
        m_FileData.clear();
        m_DataEntry.arrayEntry.clear();
        m_DataArray.values.clear();
        m_DataEntry.singleEntry.clear();
        dump( m_Json );
        rv = std::move( m_FileData );
        m_FileData.clear();
    }
    else
    {
        puts( "Error json create." );
        // rv = EXIT_FAILURE;
    }
    return rv;
}

void
FileHandler::writeJsonData( std::string filename, FileHandler::Config_t data )
{
    //testing();
    bool rv   = false;
    JSON root   = json::Array();
    do
    {
        if ( data.empty() )
        {
            break;
        }
        std::string                uuid       = "";
        JSON                       player     = json::Array();
        for ( auto &entry : data )
        {
            player = json::Array();
            uuid = entry.first;
            DataEntry_t &      maps            = entry.second;
            StringPairMap_t &  arrayMap        = maps.arrayEntry;
            StringSingleMap_t &singleMap       = maps.singleEntry;
            size_t             singleEntrySize = singleMap.size();
            size_t             arrayEntrySize  = arrayMap.size();
            if ( 0 != singleEntrySize )
            {
                JSON element = json::Object();
                for ( auto &singleEntry : singleMap )
                {
                    player[ singleEntry.first ] = singleEntry.second;
                    
                }
                root[ uuid ] =  player ;
            }
            if ( 0 == arrayEntrySize )
            {
                continue;
            }
            JSON list;
            for ( auto &multiEntry : arrayMap )
            {
                list = json::Array();
                DataArray_t &entryMap = multiEntry.second;
                StringSingleMap_t &values    = entryMap.values;
                JSON attribute = json::Array();
                for ( auto &entryPair : values )
                 {
                    attribute[ entryPair.first ] = entryPair.second;
                 }
                 player[ multiEntry.first ] = attribute;
            }
            root[uuid]=( player );
            
        }
        rv = true;
    } while ( false );
    if ( rv )
    {
        std::stringstream result;
        result << root;
        writeDataToFile( filename, result.str().c_str() );
    }
    else
    {
        writeDataToFile( "failed" + filename, "jsoncreation failed\n" );
        printf( "%s failed\n",filename.c_str() );
    }
    
}
void
FileHandler::dump( json_t const *json )
{
    
    jsonType_t const type = json_getType( json );
    if ( type != JSON_OBJ && type != JSON_ARRAY )
    {
        puts( "error" );
        return;
    }
    json_t const *child;
    for ( child = json_getChild( json ); child != 0; child = json_getSibling( child ) )
    {
        jsonType_t  propertyType = json_getType( child );
        char const *name         = json_getName( child );
        if ( !name )
        {
            puts( "error with name in json data" );
            return;
        }
        m_NewEntryName           = m_NewEntryName.empty() ? name : m_NewEntryName;

        if ( propertyType == JSON_OBJ || propertyType == JSON_ARRAY )
        {
            m_ArrayKey = ( 1 == m_GlobalCounter ) ? name : m_ArrayKey;
            m_GlobalCounter++; // Increase recursion level
            dump( child );
            m_GlobalCounter--; // decrease recursion level when finished inner dump
        }
        else
        {
            char const *value = json_getValue( child );
            if ( value )
            {
#if 0
                bool const  text = JSON_TEXT == json_getType(child);
                char const* fmt = text ? " \"%s\"" : " %s";
                printf(fmt, value);
                bool const last = !json_getSibling(child);
                if (!last)
                    putchar(',');
#endif
                
                std::string arrayKey = m_ArrayKey;
                if ( arrayKey.empty() )
                {
                    m_DataEntry.singleEntry[ name ] = value;
                }
                else
                {
                    m_ArrayMap[ name ] = value;
                }
            }
        }
    }
    switch ( m_GlobalCounter )
    {
        default:
            break;
        case 1:
            m_FileData[ m_NewEntryName ] = m_DataEntry;
            m_DataEntry.singleEntry.clear();
            m_DataEntry.arrayEntry.clear();
            m_NewEntryName.clear(); // Clear NewEntryString for new record
            break;
        case 2:
            m_DataEntry.arrayEntry[ m_ArrayKey ].values = std::move( m_ArrayMap );
            m_ArrayKey.clear();
            break;
    }
}

bool
FileHandler::createEmptyFile( std::string fileName )
{
    bool        rv      = false;
    const char *data    = "\0";
    std::string path    = m_Path + fileName;
    FILE *      newFile = fopen( ( const char * ) path.c_str(), "w+b" );
    if ( newFile )
    {
        size_t l = strlen( data ); // should equal 0
        size_t n = fwrite( data, l, 1, newFile );
        rv       = ( 1 == n );
        fclose( newFile );
    }
    return rv;
}
bool
FileHandler::writeDataToFile( std::string fileName, const char *data )
{
    bool rv = false;
    // const char* ending = ".txt";
    if ( !isPartOf( m_Ending.c_str(), ( char * ) fileName.c_str() ) )
    {
        fileName.append( m_Ending );
    }
    std::string path    = m_Path + fileName;
    FILE *newFile =  fopen( path.c_str(), "w+b" );
    if ( newFile )
    {
        // std::string indexhtml = htmlHead + (std::string)(data) + htmlfooter;
        size_t l = strlen( data );
        // printf( "SizeData=%d\n", l );
        size_t n = fwrite( data, l, 1, newFile );
        rv       = ( 1 == n );
        fclose( newFile );
    }
    return rv;
}

bool
FileHandler::isPartOf( const char *a, char *b )
{
    if ( std::strstr( b, a ) != NULL )
    { // Strstr says does b contain a
        return true;
    }
    return false;
}
std::string
FileHandler::getCellRemainingSpaces( uint32_t pos, uint32_t cellWidth )
{
    std::string rv             = "";
    uint32_t    remainingWidth = m_ColumnSpacing.at( pos ) - cellWidth;
    if ( remainingWidth > m_Spacing )
    {
        m_ColumnSpacing.at( pos ) = cellWidth - m_ColumnSpacing.at( pos );
        remainingWidth            = 0;
    }
    for ( uint32_t i = 0; i < remainingWidth; ++i )
    {
        rv += " ";
    }
    return rv;
}
#if 0
uint32_t
FileHandler::addAttribute(std::string input)
{
    uint32_t rv = 0;

    return rv;
}
#endif

#if 0
bool
FileHandler::writeDataToFile( std::string fileName, Config_t &data )
{
    bool        rv       = false;
    std::string fileData = "";
    // uint32_t    cellWidth = 0;
    std::string cell = "";
    DataArray_t dataPair;
    // size_t          pairLen = 0;
    StringPairMap_t            lineData;
    std::vector< std::string > properties;
    // size_t                     propertieslen = 0;
    JSON objData = json::Object();
    for ( auto &entry : data )
    {
        //dataPair = entry.second;

        fileData += entry.first + ":";
        // objData[entry.first] =
        // cellWidth = cell.length();
        // cell += getCellRemainingSpaces( 0, cellWidth ); // CellAddWhiteSpace(0, cell );
        // fileData += cell;
#if 1

#else
        // size_t dataLen = entry.second.pairs.size();
        lineData = entry.second.value;
        for ( auto &keyList : lineData )
        {
            fileData += keyList.first + "=";
            properties    = keyList.second;
            propertieslen = properties.size();
            for ( size_t i = 0; i < propertieslen; ++i )
            {
                cell += keyList.second.at( i ) + ";";
            }
            cell.pop_back(); // No need to check length here because cell will always be filled at least with 2 elements so cutting ; at the end of {3;4;} is possible
            if ( propertieslen > 1 )
            {

                fileData += "{" + cell + "},";
            }
            else
            {
                fileData += cell + ",";
            }
            cell = "";
        }
        fileData.pop_back(); // Removes last comma
        fileData += "\r\n";
#endif
    }

    rv = writeDataToFile( fileName, ( char * ) fileData.c_str() );
    return rv;
}

cell = entry.first + ":";
cellWidth = cell.length();
cell += getCellRemainingSpaces(0, cellWidth); // CellAddWhiteSpace(0, cell );

uint32_t remainingParamPair = m_ParamCount;
// std::vector< std::string > data               = entry.second.arrayPair< std::string >;
size_t dataLen = data.size();

for (size_t i = 0; i < dataLen; ++i)
{
    // cell      = data.at( i ) + ",";
    cellWidth = cell.length();
    cell += getCellRemainingSpaces(i + 1, cellWidth); // CellAddWhiteSpace(i, cell );
    fileData += cell;
}
#endif
#if 0
FileHandler::StringPairMap_t
FileHandler::packLineData(std::string & line, std::string & mapKey)
{
    StringPairMap_t                   rv;
    static std::vector< std::string > lineAttributes = { "\0", ":", ",", "=", "{", "}", ";", "/" };
    static const size_t               attributeLen = lineAttributes.size();

    // data.pairs.clear();
    size_t      attributePos = 0;
    Datapair_t  pair;
    std::string pairInput = "";

    // std::string     attribute = "";
    // StringPairMap_t lineInfo; // std::map< std::string, std::string >
    do
    {

        m_Trimer->trimLeftRight(line); // makes sure not whitespaces get saved in DataStructure
        size_t seperator = 0;
        size_t firstAttribute = 10000;
        size_t lowestId = 0;
        for (size_t i = 0; i < attributeLen; ++i)
        {
            attributePos = line.find_first_of(lineAttributes.at(i));
            if (attributePos < firstAttribute)
            {
                lowestId = i;
                firstAttribute = attributePos;
            }
        }
        pairInput = "";
        switch (lowestId)
        {
        default:
            if (!pair.key.empty())
            {
                rv[pair.key].push_back(line); // ? good
                // pair.pair.push_back( line );
                // data.pairs.push_back( pair );
            }
            line.clear();
            // rv = lineInfo;

            // printf( "zeile vermutlich leer %s\n", line.c_str() );
            break;

        case 1:
            mapKey = line.substr(0, firstAttribute);
            m_Trimer->trimLeftRight(mapKey);
            line = line.substr(firstAttribute + 1);
            break;
        case 2:
            pairInput = line.substr(0, firstAttribute);
            m_Trimer->trimLeftRight(pairInput);
            rv[pair.key].push_back(pairInput);
            line = line.substr(firstAttribute + 1);
            pair.key.clear();
            pair.value.clear();
            break;
        case 3:
            pair.key = line.substr(0, firstAttribute);
            m_Trimer->trimLeftRight(pair.key);
            if (pair.key.empty())
            {
                // error = 1;
            }
            line = line.substr(firstAttribute + 1);
            break;
        case 4:
            line = line.substr(firstAttribute + 1);
            break;
        case 5:
            seperator = line.find_first_of(',');
            pairInput = line.substr(0, firstAttribute);
            rv[pair.key].push_back(pairInput);
            line = line.substr(seperator + 1);
            pair.key.clear();
            pair.value.clear();
            break;
        case 6:
            pairInput = line.substr(0, firstAttribute);
            rv[pair.key].push_back(pairInput);
            line = line.substr(firstAttribute + 1);

            break;
        case 7:
            if (!pair.key.empty())
            {
                pairInput = line.substr(0, firstAttribute);
                m_Trimer->trimLeftRight(pairInput);
                rv[pair.key].push_back(pairInput);
            }
            line.clear(); // Exit loop because we reached a comment
        }
    } while (!line.empty());
    return rv;
}
#endif

#if 0
FileHandler::Config_t
FileHandler::getConfigData(std::string fileName)
{
    Config_t rv;
    //uint32_t error = 0;

    m_Trimer = new TrimHelper();
    if (!m_Trimer)
    {
        return rv;
    }
    const std::vector< std::string > myFileVec = readDataFromFile(fileName);
    const size_t                     FileVecLen = myFileVec.size();
    std::string                      line, colname;
    std::stringstream                lineStream;
    std::string                      cell;
    //uint32_t                         row = 0;
    //uint32_t                         col = 0;
    // FileData_t        data;
    std::string primaryMapKey = "";
    for (size_t i = 0; i < FileVecLen; ++i)
    {
        line = myFileVec.at(i);
        StringPairMap_t data = packLineData(line, primaryMapKey);

        if (data.empty())
        {
            printf("%d error\n", (uint32_t)i);
            continue;
        }
        printf("PrimaryKey %i: %s\t %s\n", (uint32_t)i, primaryMapKey.c_str(), data["keycode"].at(0).c_str());
        if (rv.end() == rv.find(primaryMapKey))
        {
            rv[primaryMapKey].value = data;
        }
        else
        {
            // printf( "Multiple UUID values!\n" );
        }
    }

    return rv;
}
#endif