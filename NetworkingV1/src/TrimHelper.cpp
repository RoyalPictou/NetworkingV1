#include "TrimHelper.h"

const char *ws = " \t\n\r\f\v";
TrimHelper::TrimHelper()
    : m_Chars(ws)
{

    

}

TrimHelper::TrimHelper( const char *chars ) 
{
    ws = chars;
}

TrimHelper::~TrimHelper() {}

// trim from end of string (right)
inline void
TrimHelper::rtrim( std::string &s )
{
    s =s.erase( s.find_last_not_of( m_Chars ) + 1 );
    
}

// trim from beginning of string (left)
inline void
TrimHelper::ltrim( std::string &s )
{
    s = s.erase( 0, s.find_first_not_of( m_Chars ) );
    
}

// trim from both ends of string (right then left)
 void 
TrimHelper::trimLeftRight( std::string &s )
{
    rtrim( s );
    ltrim( s ); 
}

void
TrimHelper::findAndReplaceAll( std::string &data, std::string toSearch, std::string replaceStr )
{
    // Get the first occurrence
    size_t pos = data.find( toSearch );
    // Repeat till end is reached
    while ( pos != std::string::npos )
    {
        // Replace this occurrence of Sub String
        data.replace( pos, toSearch.size(), replaceStr );
        // Get the next occurrence from the current position
        pos = data.find( toSearch, pos + replaceStr.size() );
    }
}
