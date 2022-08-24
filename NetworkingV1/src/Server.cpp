#include "Server.h"
#include <time.h>
#include <algorithm>

Server::Server()
{
    m_File = new FileHandler("res/Server");
    if (!m_File)
    {
        printf("Error memoryLeak\n");
    }
}

Server::~Server() {}

void
Server::readData(std::string& data)
{

    m_OutputData = m_File->readJsonData(data);
    bool result = false;
    do
    {
        if (m_OutputData.empty() && m_UserData.empty())
        {
            performAction2(LOAD_SERVER_DATA);
            performAction2(GENERATE_PLAYER_UUID);
            continue; // need to Gather client data
        }
        else
        {
            result = performAction2(GATHER_CLIENT_DATA);
            if (result)
            {
                break;
            }
            performAction2(GENERATE_PLAYER_UUID);
            break;
        }
    } while (true);
}

std::string
Server::getData()
{
    std::string rv = m_File->readDataFromFile("specificUser.json");
    m_File->createEmptyFile("specificUser.json");
    return rv;
}

void
gen128UUID()
{
    SimpleUUID::GUID_t myGUID;

    SimpleUUID id1;
    SimpleUUID id2;
    myGUID.guid.high = id1;
    myGUID.guid.low = id2;

    for (int i = 0; i < sizeof(myGUID); ++i)
        printf("%2.2x ", myGUID.array[i]);
    printf("\n");

    // No User Found generate Unike CLIENT Entry
    SimpleUUID  id;
    std::string entry = std::to_string(id) + ":";
}
const std::string
currentDateTime()
{
    time_t    now = time(0);
    struct tm tstruct;
    char      buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

bool
Server::createEntry(EntryPair_t entry, Entry_t* data)
{
    bool rv = true;
    // data    = new Entry_t;
    // if ( data )
    //{
    data->key = entry.first;
    data->data = &entry.second;
    data->pairMap = &data->data->arrayEntry; // .data->arrayEntry;
    data->singleMap = &data->data->singleEntry;
    data->singleCount = data->singleMap->size();
    data->pairCount = data->pairMap->size();
    data->type = TYPE_UNKNOWN;
    if (data->pairCount + data->singleCount)
    {
        rv = true; //  EntryMap Count not empty
    }
    // }
    return rv;
}

/*
Takes in a key list and a key to find (in future switch maybe to a Map
Returns KEY_LIST_EMPTY if key list is empty
Returns KEY_LIST_MARKED if key was found and element has been removed
Returns KEY_LIST_NOACTION if nothing happend
*/
Server::Key_t
Server::findKey(StringMap_t& keyMap, std::string key)
{
    Server::Key_t rv = { {}, "", KEY_LIST_MARKED }; // Init of vector, pair, action

    std::string helperString;
    do
    {
        if (keyMap.empty())
        {
            rv.action = KEY_LIST_EMPTY;
            break;
        }
        StringMapIt_t it = keyMap.find(key);
        if (keyMap.end() == it)
        {
            rv.action = KEY_LIST_NOACTION;
        }
        else
        {
            helperString = it->second;
            size_t pos = helperString.find(",");
            if (std::string::npos == pos)
            {
                rv.keyPair = it->second;
            }
            else
            {
                do

                {
                    rv.keyMulti.push_back(helperString.substr(0, pos));
                    helperString = helperString.substr(pos + 1);
                    pos = helperString.find(",");
                    if (std::string::npos == pos)
                    {
                        rv.keyPair = helperString; // put last array key (from deleteKeys array) on keyPair slot.
                        // if this slot isnt used there are no Multi keys in storage.
                        break;
                    }

                } while (true);
            }

            keyMap.erase(it);
        }
    } while (false);

    return rv;
}
Server::StringMap_t
Server::getKeys(std::string key, FileHandler::DataEntry_t& userEntryFromUUID, Entry_t* entry)
{
    Server::StringMap_t            rv;
    do
    {


while(false);///<pseudoloop
}
    FileHandler::StringPairMap_t* pairMap = entry->pairMap;

    FileHandler::StringPairMapIt_t publicKeyIt = pairMap->find(key);
    if (pairMap->end() == publicKeyIt)
    {
        entry->type = TYPE_PRIVATE;
        FileHandler::StringSingleMap_t nodataEntry;
        nodataEntry["NoData"] = "No " + key + " data";
        nodataEntry["Error"] = "Error data Manegment";
        userEntryFromUUID.singleEntry = nodataEntry;
        // m_OutputData[ serverEntry->key ] = userEntryFromUUID;
        // No public data found
    }
    else
    {
        entry->type = TYPE_PUBLIC;
        // Add public keys to strig vector for retrieving public data only
        FileHandler::DataArray_t       arrayObject = publicKeyIt->second;
        FileHandler::StringSingleMap_t publicKeyMap = arrayObject.values;
        for (auto& keys : publicKeyMap)
        {
            if (0 == keys.second.compare("true")) // handle parameter from json
            {
                rv[keys.first] = "true";
            }
            else
            {
                rv[keys.first] = keys.second;
            }
        }
    }
    return rv;
}
bool
Server::performAction(uint32_t actionType)
{
    bool       rv = false;
    SimpleUUID id;
    // FileHandler::Config_t          writeData; replaced by m_OutputData
    FileHandler::StringSingleMap_t firstData;
    std::string                    fileStringData = m_File->readDataFromFile("server.json");
    m_ServerData = m_File->readJsonData(fileStringData);
    std::vector< std::string > keyList = { "dataType", "public" };
    StringMap_t                publicKeyMap;
    StringMap_t                deleteKeyMap;
    std::string                uuidString = "";
    // const size_t               keyListLen         = keyList.size();
    //uint8_t  containerType = TYPE_PRIVATE;
    Entry_t* serverEntry = new Entry_t;
    Entry_t* userEntry = new Entry_t;
    FileHandler::ConfigIt_t currentServerIt;
    if (!(serverEntry && userEntry))
    {
        actionType = MEMORY_ERROR;
        printf("Memory Error in Server::performAction\n");
    }
    switch (actionType)
    {
    default:
        printf("Unkown communication type -> Check Server::performAction\n");
        break;
    case GENERATE_PLAYER_UUID:

        firstData["name"] = "unkownUser";
        firstData["level"] = "1";
        firstData["firstLogin"] = currentDateTime();
        uuidString = std::to_string(id);
        m_ServerData[uuidString].singleEntry = firstData;
        m_UserData[uuidString].singleEntry = firstData;
        rv = true;
        break;
    case FIND_PLAYER_UUID:
        break;
    case GATHER_CLIENT_DATA:
        m_OutputData.clear();



    }


    if (rv)
    {
        m_File->writeJsonData("specificUser.json", m_UserData);
        m_File->writeJsonData("server.json", m_ServerData);
    }
    return rv;
}
bool
Server::performAction2(uint32_t actionType)
{
    bool       rv = false;


    switch (actionType)
    {
    default:
        printf("Unkown communication type -> Check Server::performAction\n");
        break;
    case GENERATE_PLAYER_UUID:
    {
        SimpleUUID id;

        std::string                uuidString = std::to_string(id);
        FileHandler::StringSingleMap_t firstData;
        firstData["name"] = "unkownUser";
        firstData["level"] = "1";
        firstData["firstLogin"] = currentDateTime();

        m_ServerData[uuidString].singleEntry = firstData;
        m_UserData[uuidString].singleEntry = firstData;
        break;
    }
    case LOAD_SERVER_DATA:
    {
        std::string                    fileStringData = m_File->readDataFromFile("server.json");
        m_ServerData = m_File->readJsonData(fileStringData);
        break;
    }
    case GATHER_CLIENT_DATA:
    {
        Entry_t* serverEntry = new Entry_t;
        Entry_t* userEntry = new Entry_t;
        if (!(serverEntry && userEntry))
        {
            actionType = MEMORY_ERROR;
            printf("Memory Error in Server::performAction\n");
        }
        std::vector< std::string > keyList = { "dataType", "public" };
        StringMap_t                publicKeyMap;
        StringMap_t                deleteKeyMap;
        FileHandler::ConfigIt_t currentServerIt;
        //m_OutputData.clear();
        for (EntryPair_t& userEntryElement : m_UserData)
        {
            rv = createEntry(userEntryElement, userEntry);
            if (!rv) // check if UUID has any content DONE
            {
                printf("Failed to create Entry\n");
                continue;
            }
            FileHandler::ConfigIt_t serverEntryElement = m_ServerData.begin();
            //for ( EntryPair_t &serverEntryElement : m_ServerData )
            while (serverEntryElement != m_ServerData.cend())
            {

                rv = createEntry(*serverEntryElement, serverEntry);
                // increase serverUUIDEntry Iterator
                currentServerIt = serverEntryElement++;
                if (!rv) // check if UUID has any content DONE
                {
                    printf("Failed to create Entry\n");
                    continue;
                }
                FileHandler::DataEntry_t userEntryFromUUID;
                // FileHandler::StringSingleMap_t &serverSingleMap = serverEntry->singleMap;
                 //FileHandler::StringPairMap_t &  pairMap         = serverEntry->pairMap;
                 // check if UUID has any public keys
                if (0 == serverEntry->key.compare(userEntry->key))
                {
                    // First append userData to serverData -> standardcommunication with server


                    // look up key for deleting whole UUID entry
                    FileHandler::StringSingleMapIt_t deleteUserIt = userEntry->singleMap->find("deleteUser");
                    if (deleteUserIt != userEntry->singleMap->cend())
                    {
                        //Entry found, now check if param ist set to true
                        if (0 == deleteUserIt->second.compare("true"))
                        {
                            //erase current element saved in currentServerIt

                            m_ServerData.erase(serverEntry->key);
                            continue;
                        }
                    }

                    for (auto& userSingleKey : *userEntry->singleMap)
                    {

                        serverEntry->singleMap->at(userSingleKey.first) = userSingleKey.second;
                    }
                    FileHandler::StringPairMap_t& pairMap = *userEntry->pairMap;
                    FileHandler::StringPairMapIt_t deleteIt = pairMap.find("deleteKeys");
                    if (pairMap.end() != deleteIt)
                    {
                        deleteKeyMap = getKeys("deleteKeys", userEntryFromUUID, userEntry);
                    }
                    for (auto& userPairMapEntry : pairMap)
                    {
                        if (0 == userPairMapEntry.first.compare("deleteKeys"))
                        {
                            continue;
                        }
                        FileHandler::DataArray_t       userDataArray = userPairMapEntry.second;
                        FileHandler::StringSingleMap_t userDataArrayMap = userDataArray.values;
                        for (auto& userPairMapValues : userDataArrayMap)
                        {
                            serverEntry->pairMap->at(userPairMapEntry.first).values[userPairMapValues.first] = userPairMapValues.second;
                        }
                    }

                    // Than write return data for user
                    serverEntry->type = TYPE_ALL;
                    // userEntryFromUUID = serverEntry->data;
                    printf("UserFound add full entry\n");
                }
                else // add public data
                {
                    serverEntry->type = TYPE_PUBLIC;
                    publicKeyMap = getKeys("public", userEntryFromUUID, serverEntry);
                    if (publicKeyMap.empty())
                    {
                        continue;
                    }
                }
                Key_t keyComponent;
                // keyState = KEY_LIST_NOACTION;
                FileHandler::StringPairMapIt_t   pairIt;
                FileHandler::StringPairMapIt_t   userPairCurrentIt;
                FileHandler::StringSingleMapIt_t arrayIt;

                FileHandler::StringSingleMapIt_t singleTt;
                FileHandler::StringSingleMapIt_t singleCurrentIt;
                switch (serverEntry->type)
                {

                case TYPE_PUBLIC:
                    for (auto& singleKey : *serverEntry->singleMap)
                    {
                        keyComponent = findKey(publicKeyMap, singleKey.first);
                        if (KEY_LIST_EMPTY == keyComponent.action)
                        {
                            break;
                        }
                        if (KEY_LIST_MARKED == keyComponent.action)
                        {
                            userEntryFromUUID.singleEntry[singleKey.first] = singleKey.second;
                        }
                    }
                    for (auto& pairMapEntry : *serverEntry->pairMap)
                    {
                        std::string arrayKey = pairMapEntry.first;
                        if (0 == arrayKey.compare("public"))
                        {
                            continue; // Dont add array of public keys to the output data
                        }
                        // Check if public key is available for whole array
                        keyComponent = findKey(publicKeyMap, arrayKey);
                        bool completeArrayIsPublic = (0 == keyComponent.keyPair.compare("true"));
                        if (KEY_LIST_EMPTY == keyComponent.action)
                        {
                            break;
                        }
                        if (KEY_LIST_MARKED == keyComponent.action && completeArrayIsPublic)
                        {
                            userEntryFromUUID.arrayEntry[pairMapEntry.first] = pairMapEntry.second;
                            continue; // Added full array to output data so further key checking is not required
                        }
                        for (auto& pairMapValues : pairMapEntry.second.values)
                        {
                            if (completeArrayIsPublic) // error can occur here because completeArrayIsPublic wont change state anymore
                            {
                                keyComponent = findKey(publicKeyMap, pairMapValues.first);
                            }

                            if (KEY_LIST_EMPTY == keyComponent.action)
                            {
                                break;
                            }
                            if (KEY_LIST_MARKED == keyComponent.action)
                            {
                                userEntryFromUUID.singleEntry[pairMapValues.first] = pairMapValues.second;
                            }
                        }
                        if (KEY_LIST_EMPTY == keyComponent.action)
                        {
                            break;
                        }
                    }
                    break;
                case TYPE_PRIVATE:
                    break;
                case TYPE_ALL:
                    //FileHandler::StringSingleMap_t & singlemap = serverEntry->singleMap;
                    singleTt = serverEntry->singleMap->begin();
                    //FileHandler::StringSingleMapIt_t singleCurrentIt;
                    while (singleTt != serverEntry->singleMap->cend())
                    {
                        keyComponent = findKey(deleteKeyMap, singleTt->first);
                        singleCurrentIt = singleTt++;
                        if (KEY_LIST_EMPTY == keyComponent.action)
                        {
                            break;
                        }
                        if (KEY_LIST_MARKED == keyComponent.action)
                        {
                            serverEntry->singleMap->erase(singleCurrentIt);
                        }
                    }
                    pairIt = serverEntry->pairMap->begin();
                    //FileHandler::StringPairMapIt_t userPairCurrentIt;
                    while (pairIt != serverEntry->pairMap->cend())
                    {
                        std::string arrayKey = pairIt->first;
                        if (0 == arrayKey.compare("deleteKeys")) // weird
                        {
                            continue; // Dont add array of deleteKeys keys to the output data
                        }
                        keyComponent = findKey(deleteKeyMap, arrayKey); // keyComponent contains arraypair value
                        bool deletecompleteArray = (0 == keyComponent.keyPair.compare("true"));
                        if (deletecompleteArray)
                        {
                            userPairCurrentIt = pairIt++;
                            std::string eraseKey = userPairCurrentIt->first;
                            //userEntryFromUUID.arrayEntry.erase( eraseKey );
                            serverEntry->pairMap->erase(eraseKey);
                            continue; // No more actions after deleting whole array
                        }
                        if (KEY_LIST_EMPTY == keyComponent.action)
                        {
                            break;
                        }
                        if (KEY_LIST_MARKED == keyComponent.action)
                        {
                            FileHandler::DataArray_t& dataArrayRef = pairIt->second;
                            FileHandler::StringSingleMap_t& dataArrayMapRef = dataArrayRef.values;
                            arrayIt = dataArrayMapRef.begin();
                            while (arrayIt != dataArrayMapRef.cend())
                            {
                                // keyState  = findKey( deleteKeyMap, arrayIt->first );
                                singleCurrentIt = arrayIt++;
                                if (KEY_LIST_EMPTY == keyComponent.action)
                                {
                                    break;
                                }
                                if (0 == keyComponent.keyPair.compare("true"))
                                {
                                    dataArrayRef.values.clear(); // Delete hole array
                                    break;
                                }
                                if (0 == keyComponent.keyPair.compare(singleCurrentIt->first))
                                {
                                    dataArrayMapRef.erase(singleCurrentIt);
                                }
                                else
                                {
                                    for (std::string& key : keyComponent.keyMulti)
                                    {
                                        if (0 == key.compare(singleCurrentIt->first))
                                        {
                                            dataArrayMapRef.erase(singleCurrentIt);
                                        }
                                    }
                                }
                            }
                        }

                        userPairCurrentIt = pairIt++; // could also be serverPairCurrentIt
                        if (userPairCurrentIt->second.values.empty())
                        {
                            serverEntry->pairMap->erase(userPairCurrentIt);
                        }
                    }
                    userEntryFromUUID.singleEntry = *serverEntry->singleMap; // bad copy instructions
                    userEntryFromUUID.arrayEntry = *serverEntry->pairMap;
                    break;
                default:
                    break;
                }
                std::string uuidString = serverEntry->key;
                switch (serverEntry->type)
                {
                default:
                    break;
                case TYPE_PUBLIC:
                case TYPE_PRIVATE:
                    uuidString = serverEntry->key.substr(0, 5);

                    break;
                case TYPE_ALL:
                    //serverEntry->data.singleEntry = serverEntry->singleMap; // Refactor
                    //serverEntry->data.arrayEntry = serverEntry->pairMap;   // Refactor
                   // m_ServerData[uuidString] = serverEntry->data;      // make single and pair map a ref to data field
                    break;
                }
                m_OutputData[uuidString] = userEntryFromUUID; // key = UUID(shorten), data = DataEntry_t



            }

        }
        break;
    }
    }
    if (rv)
    {
        if (m_OutputData.empty())
        {
            printf("NEVER DO THIS\n");
            m_File->writeJsonData("specificUser.json", m_UserData);
        }
        else {
            m_File->writeJsonData("specificUser.json", m_OutputData);
        }

        m_File->writeJsonData("server.json", m_ServerData);

        //m_File->writeJsonData("server.json", m_ServerData);
    }
    return rv;
}
#if 0

m_UserIt = m_ServerData.find(serverEntry.first); // Good to have
for (auto& serverArrays : serverData.arrayEntry)
{
    FileHandler::StringSingleMapIt_t it = serverArrays.second.values.find("public");
    if (serverArrays.second.values.end() == it)
    {
        for (auto& publicKeys : serverArrays.second.values)
        {
            if (0 == publicKeys.second.compare("true"))
            {
                break;
            }
            publicKeyList.push_back(publicKeys.first);
        }
    }
    for (auto& serverArrayKeys : serverArrays.second.values)
    {
        if (0 == serverArrayKeys.second.compare("true"))
        {
            break;
        }
        publicKeyList.push_back(serverArrayKeys.first);
    }
}

#endif