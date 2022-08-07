#ifndef JSONPARSER__H_
#define JSONPARSER__H_
#include "tiny-json.h"
#include <list>
#include <string>
class jsonParser : jsonPool_t
{

    std::list< json_t > m_list{};
    std::string         m_str{};
    json_t const *      m_root{};
    static json_t *     alloc_fn( jsonPool_t *pool );

public:
    jsonParser();
    jsonParser( const char *str );
    jsonParser( const jsonParser & ) = delete;
    jsonParser( jsonParser && )      = delete;
    jsonParser &operator=( const jsonParser & ) = delete;
    jsonParser &operator=( jsonParser && ) = delete;

    void
    parse( const char *str );

    json_t const *getRoot() const;
};

#endif
