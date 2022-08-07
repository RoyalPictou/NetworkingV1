#ifndef SIMPLEUUID__H_
#define SIMPLEUUID__H_
// https://opensource.apple.com/source/CF/CF-299.35/Base.subproj/uuid.c.auto.html
#include <stdint.h>
#include <xhash>
class SimpleUUID
{
public:
    typedef struct
    {
        uint64_t high;
        uint64_t low;
    } GUIDParts_t;

    typedef union
    {
        GUIDParts_t guid;
        uint8_t     array[ sizeof( GUIDParts_t ) ];
    } GUID_t;
    SimpleUUID();
    ~SimpleUUID();

    operator uint64_t() const;

private:
    uint64_t m_UUID;
};

namespace std
{
#if 0
template <> struct hash< SimpleUUID >
{
    std::size_t
    operator()( const SimpleUUID &uuid ) const
    {
        return hash< uint64_t >()( ( uint64_t ) uuid );
    }
};
#endif
} // namespace std
#endif
