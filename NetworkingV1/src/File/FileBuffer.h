

#ifndef FILEBUFFER__H_ 
#define FILEBUFFER__H_
#include <string>
class FileBuffer
{
public:
    enum
    {
        RET_SUCCESS         = 0,
        RET_ERR_OPEN_FAILED = 1,
        RET_ERR_NO_MEM      = 2,
        RET_ERR_READ_ERR    = 3,
    };

public:
    FileBuffer( std::string filename );
    ~FileBuffer();
    void *
    GetFileContent();
    size_t
    GetFileSize();
    std::string &
    GetFileContentString();

    void SetFileContentString(std::string &data);

private:
    bool
    Open();
    void
    Close();
    void
    GetSize();
    void
    Load();

private:
    std::string m_Filename;
    std::string m_Content;
    FILE *      m_FileHandle;
    void *      m_Buffer;
    uint64_t    m_Size;
    int         m_Error;
};
#endif