#include "FileBuffer.h"

FileBuffer::FileBuffer( std::string filename )
    : m_Filename( filename )
    , m_Size( 0ull )
    , m_Error( RET_SUCCESS )
{
    bool rv = Open();
    if ( rv )
    {
        GetSize();
        m_Buffer = malloc( ( size_t ) m_Size );
        if ( m_Buffer )
        {
            Load();
        }
        else
        {
            m_Error = RET_ERR_NO_MEM;
        }
    }
    else
    {
        m_Error = RET_ERR_OPEN_FAILED;
    }
}
FileBuffer::~FileBuffer()
{
    if ( m_Buffer )
    {
        free( m_Buffer );
        m_Buffer = NULL;
    }
    Close();
}
void *
FileBuffer::GetFileContent()
{
    return m_Buffer;
}
size_t
FileBuffer::GetFileSize()
{
    return m_Size;
}
std::string &
FileBuffer::GetFileContentString()
{
    if ( m_Buffer )
    {
        std::string myContent( reinterpret_cast< char * >( m_Buffer ), m_Size );
        m_Content = std::move( myContent );
    }
    return m_Content;
}

void
FileBuffer::SetFileContentString( std::string &data )
{
    m_FileHandle = fopen( m_Filename.c_str(), "w+" );
    fwrite( data.c_str(), 1, data.length(), m_FileHandle );
    Close();
}

bool
FileBuffer::Open()
{
    m_FileHandle = fopen( m_Filename.c_str(), "rb" );
    return ( NULL != m_FileHandle );
}
void
FileBuffer::Close()
{
    if ( m_FileHandle )
    {
        fclose( m_FileHandle );
    }
}
void
FileBuffer::GetSize()
{
    m_Size = 0ull;
    if ( m_FileHandle )
    {
        int res = fseek( m_FileHandle, 0, SEEK_END );
        if ( 0 == res )
        {
            m_Size = ( uint64_t ) ftell( m_FileHandle );
            // reset file-pointer
            fseek( m_FileHandle, 0, SEEK_SET );
        }
    }
}
void
FileBuffer::Load()
{
    if ( m_FileHandle && m_Buffer )
    {
        size_t n = fread( m_Buffer, m_Size, 1, m_FileHandle );
        if ( 1 != n )
        {
            m_Error = RET_ERR_READ_ERR;
        }
    }
}
