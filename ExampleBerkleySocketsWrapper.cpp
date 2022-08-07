#ifdef _WIN32
#    define WS_WINDOWS
#else
#    define WS_LINUX
#    define WS_XILINX_FREERTOS
#endif

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// windows - windows - windows - windows - windows - windows
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#ifdef WS_WINDOWS
#    undef UNICODE

#    define WIN32_LEAN_AND_MEAN
#    include <windows.h>
#    include <winsock2.h>
#    include <ws2tcpip.h>

#    include <process.h>

// Need to link with Ws2_32.lib
#    pragma comment( lib, "Ws2_32.lib" )
//#    pragma comment( lib, "Mswsock.lib" )
#endif

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// linux - linux - linux - linux - linux - linux - linux - l
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#ifdef WS_LINUX
//???? what is the correct define for XILINX SDK ????
//#ifdef INC_FREERTOS_H

// Xilinx SDK ------------- start
#    include <stdio.h> // only for "size_t" in Xilinx SDK ... :-(

#    include <lwip/sockets.h>
#    include <netif/xadapter.h>
#    include <lwipopts.h>

#    include <FreeRTOS.h>
#    include <task.h>
#    define WS_FREEE_RTOS_THREAD_STACKSIZE ( 1024 ) // this is words - probably 32-bit * 1024 ==> 4096 bytes ... but haven't checked so far

#endif

// standard includes
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// windows - windows - windows - windows - windows - windows
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#ifdef WS_WINDOWS
#    undef UNICODE

#    define WIN32_LEAN_AND_MEAN
#    include <windows.h>
#    include <winsock2.h>
#    include <ws2tcpip.h>

#    include <process.h>

// Need to link with Ws2_32.lib
#    pragma comment( lib, "Ws2_32.lib" )
//#    pragma comment( lib, "Mswsock.lib" )
#endif

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// linux - linux - linux - linux - linux - linux - linux - l
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#ifdef WS_LINUX

#endif

// section: sockets start =================================

#define SAL_SOCK_AF_INET ( AF_INET )
#define SAL_SOCK_AF_UNSPEC ( AF_UNSPEC )
#define SAL_SOCK_SOCK_STREAM ( SOCK_STREAM )
#define SAL_SOCK_SOCK_DGRAM ( SOCK_DGRAM )
#define SAL_SOCK_IPPROTO_TCP ( IPPROTO_TCP )
#define SAL_SOCK_AI_PASSIVE ( AI_PASSIVE )

#define SAL_SOCK_PF_INET ( PF_INET )

#define SAL_SOCK_INVALID_SOCKET ( INVALID_SOCKET )
#define SAL_SOCK_SOCKET_ERROR ( SOCKET_ERROR )

#ifdef WS_WINDOWS
typedef SOCKET SAL_SockSocket_t;
typedef SOCKADDR_IN SAL_SockSockAddr_t;
typedef int SAL_SockLen;

typedef struct
{
    WSADATA m_Info;
    SAL_SockSocket_t m_SocketListen;
    SAL_SockSocket_t m_SocketAccept;
} SAL_SockConfig_t;

// socket functions
bool
SAL_SockInit( SAL_SockConfig_t *sockConfig )
{
    bool rv = ( 0 == WSAStartup( MAKEWORD( 2, 0 ), &( sockConfig->m_Info ) ) ) ? true : false;
    return rv;
}

void
SAL_SockDeInit()
{
    WSACleanup();
}

SAL_SockSocket_t
SAL_SockSocket( int af, int type, int protocol )
{
    SAL_SockSocket_t s = socket( af, type, protocol );
    return s;
}

int
SAL_SockCloseSocket( SAL_SockSocket_t s )
{
    int rv = closesocket( s );
    return rv;
}

int
SAL_SockBind( SAL_SockSocket_t s, SAL_SockSockAddr_t *name, int namelen )
{
    int rv = bind( ( SOCKET ) s, ( const struct sockaddr * ) name, namelen );
    return rv;
}

int
SAL_SockListen( SAL_SockSocket_t s, int backlog )
{
    int rv = listen( s, backlog );
    return rv;
}

SAL_SockSocket_t
SAL_SockAccept( SAL_SockSocket_t s, SAL_SockSockAddr_t *addr, int *addrlen )
{
    SAL_SockSocket_t rv = ( SAL_SockSocket_t ) accept( s, ( struct sockaddr * ) addr, addrlen );
    return rv;
}

int
SAL_SockRecv( SAL_SockSocket_t s, char *buf, int len, int flags )
{
    int rv = recv( ( SOCKET ) s, buf, len, flags );
    return rv;
}

int
SAL_SockSend( SAL_SockSocket_t s, char *buf, int len, int flags )
{
    int rv = send( ( SOCKET ) s, buf, len, flags );
    return rv;
}

#endif

#ifdef WS_LINUX
typedef void *WSADATA;
#    ifdef WS_XILINX_FREERTOS
typedef int SAL_SockSocket_t;
typedef struct sockaddr SAL_SockSockAddr_t;
typedef socklen_t SAL_SockLen;
// lwip_socket
#    endif
typedef struct
{
    WSADATA m_Info;
    SAL_SockSocket_t m_SocketListen;
    SAL_SockSocket_t m_SocketAccept;
} SAL_SockConfig_t;

// socket functions
bool
SAL_SockInit( SAL_SockConfig_t *sockConfig )
{
    bool rv = true;
    return rv;
}

void
SAL_SockDeInit()
{
}

SAL_SockSocket_t
SAL_SockSocket( int af, int type, int protocol )
{
    SAL_SockSocket_t s = lwip_socket( af, type, protocol );
    return s;
}

int
SAL_SockCloseSocket( SAL_SockSocket_t s )
{
    int rv = closesocket( s );
    return rv;
}

int
SAL_SockBind( SAL_SockSocket_t s, SAL_SockSockAddr_t *name, int namelen )
{
    int rv = lwip_bind( ( SAL_SockSocket_t ) s, ( const struct sockaddr * ) name, namelen );
    return rv;
}

int
SAL_SockListen( SAL_SockSocket_t s, int backlog )
{
    int rv = lwip_listen( s, backlog );
    return rv;
}

SAL_SockSocket_t
SAL_SockAccept( SAL_SockSocket_t s, SAL_SockSockAddr_t *addr, SAL_SockLen *addrlen )
{
    SAL_SockSocket_t rv = ( SAL_SockSocket_t ) lwip_accept( s, ( struct sockaddr * ) addr, addrlen );
    return rv;
}

int
SAL_SockRecv( SAL_SockSocket_t s, char *buf, int len, int flags )
{
    int rv = read( ( SAL_SockSocket_t ) s, buf, len );
    return rv;
}

int
SAL_SockSend( SAL_SockSocket_t s, char *buf, int len, int flags )
{
    int rv = write( ( SAL_SockSocket_t ) s, buf, len );
    return rv;
}

#endif
