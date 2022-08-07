#ifndef TRIMHELPER__H_
#define TRIMHELPER__H_

#include <string>

class TrimHelper
{
public:
    TrimHelper();
    TrimHelper( const char *chars );
    ~TrimHelper();
	// trim from end of string (right)
    inline void rtrim( std::string &s );

    // trim from beginning of string (left)
    inline void ltrim( std::string &s  );

    // trim from both ends of string (right then left)
    void trimLeftRight( std::string &s );
    void
    findAndReplaceAll( std::string &data, std::string toSearch, std::string replaceStr );//https://thispointer.com/find-and-replace-all-occurrences-of-a-sub-string-in-c/
    


	private:
    const char *m_Chars;

};


#endif