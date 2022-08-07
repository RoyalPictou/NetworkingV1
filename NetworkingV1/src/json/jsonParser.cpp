#include "jsonParser.h"

json_t *
jsonParser::alloc_fn( jsonPool_t *pool )
{
    const auto list_pool = ( jsonParser * ) pool;
    list_pool->m_list.emplace_back();
    return &list_pool->m_list.back();
}



jsonParser::jsonParser()
    : jsonPool_t{ &alloc_fn, &alloc_fn }
{
}
jsonParser::jsonParser( const char *str )
    : jsonPool_t{ &alloc_fn, &alloc_fn }
    , m_str{ str }
{
    m_root = json_createWithPool( ( char * ) m_str.data(), this );
}

void
jsonParser::parse( const char *str )
{
    m_str = str;
    m_list.clear();
    m_root = json_createWithPool( ( char * ) m_str.data(), this );
}

json_t const *
jsonParser::getRoot() const
{
    return m_root;
}
