#include "EventListener.h"
#include "../../include/SDL2/SDL.h"
#include <stdio.h>
EventListener::EventListener()
{
    SDL_Init(0);
    m_KeyState = SDL_GetKeyboardState( NULL );
}
EventListener::~EventListener() {}
void
EventListener::setScreenSize( uint32_t x, uint32_t y )
{
    m_ScreenSize = { x, y };
    // m_ScreenSize.x = x;
    // m_ScreenSize.y = y;
    // printf("EventListener__: setscreenSize: x %d y %d\n", m_ScreenSize.x, m_ScreenSize.y);
}
IntPoint2D_t
EventListener::getScreenSize()
{
    printf( "EventListener__: getscreenSize: x %d y %d\n", m_ScreenSize.x, m_ScreenSize.y );
    return m_ScreenSize;
}
void
EventListener::processInput()
{

#if 0
    // continuous-response keys
    if ( m_Key[ SDLK_LEFT ] )
    {
        printf( "SDLK_LEFT\n" );
    }
    if ( m_Key[ SDLK_RIGHT ] )
    {
        printf( "SDLK_RIGHT\n" );
    }
    if ( m_Key[ SDLK_UP ] )
    {
        printf( "SDLK_UP\n" );
    }
    if ( m_Key[ SDLK_DOWN ] )
    {
        printf( "SDLK_DOWN\n" );
    }
#endif
}
EventListener::EventType_t
EventListener::checkEvents()
{
    EventListener::EventType_t rv = { 0, 0 };
    SDL_WaitEvent( &m_Event );
    rv.environmentType = m_Event.type;
    
    //if ( m_Event.key.keysym.scancode != SDL_GetScancodeFromKey( m_Event.key.keysym.sym ) )
    //    printf( "Physical %s key acting as %s key", SDL_GetScancodeName( m_Event.key.keysym.scancode ), SDL_GetKeyName( m_Event.key.keysym.sym ) );
    if ( m_Event.type == SDL_WINDOWEVENT )
    {
        SDL_WindowEvent we = m_Event.window;
        rv.windowType      = we.event;
        switch ( we.event )
        {
            case SDL_WINDOWEVENT_SHOWN:
                SDL_Log( "Window %d active", we.windowID );
                rv.window = WINDOW_ACTIVE;
                break;
            case SDL_WINDOWEVENT_HIDDEN:
                SDL_Log( "Window %d inactive", we.windowID );
                rv.window = WINDOW_INACTIVE;
                break;
            case SDL_WINDOWEVENT_EXPOSED:
                SDL_Log( "Window %d exposed", we.windowID );
                break;
            case SDL_WINDOWEVENT_MOVED:
                SDL_Log( "Window %d moved to %d,%d", we.windowID, we.data1, we.data2 );
                break;
            case SDL_WINDOWEVENT_RESIZED:
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                SDL_Log( "Window %d resized to %dx%d", we.windowID, we.data1, we.data2 );
                rv.window = WINDOW_REDRAW;
                setScreenSize( we.data1, we.data2 );
                break;
            case SDL_WINDOWEVENT_MINIMIZED:
                SDL_Log( "Window %d minimized", we.windowID );
                break;
            case SDL_WINDOWEVENT_MAXIMIZED:
                SDL_Log( "Window %d maximized", we.windowID );
                rv.window = WINDOW_MAXIMIZED;
                break;
            case SDL_WINDOWEVENT_RESTORED:
                SDL_Log( "Window %d restored", we.windowID );
                break;
            case SDL_WINDOWEVENT_ENTER:
                SDL_Log( "Mouse entered window %d", we.windowID );
                break;
            case SDL_WINDOWEVENT_LEAVE:
                SDL_Log( "Mouse left window %d", we.windowID );
                break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                SDL_Log( "Window %d gained keyboard focus", we.windowID );
                break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                SDL_Log( "Window %d lost keyboard focus", we.windowID );
                break;
            case SDL_WINDOWEVENT_CLOSE:
                SDL_Log( "Window %d closed", we.windowID );
                break;
#if SDL_VERSION_ATLEAST( 2, 0, 5 )
            case SDL_WINDOWEVENT_TAKE_FOCUS:
                SDL_Log( "Window %d is offered a focus", we.windowID );
                break;
            case SDL_WINDOWEVENT_HIT_TEST:
                SDL_Log( "Window %d has a special hit test", we.windowID );
                break;
#endif
            default:
                SDL_Log( "Window %d got unknown m_Event %d ", we.windowID, we.event );

                break;
        }
    }
    if ( m_Event.type == SDL_KEYDOWN )
    {
        
        SDL_Keycode key = m_Event.key.keysym.sym;
        // processInput();
        // printf( "Key pressed\n" );
        // handleKeyPress( &event.key.keysym );
        rv.window = WINDOW_ACTION;
        switch ( key )
        {
            case SDLK_LEFT:
                printf( "SDLK_LEFT\n" );
                rv.environment = ANGLE_TURN_LEFT;
                break;
            case SDLK_RIGHT:
                rv.environment = ANGLE_TURN_RIGHT;
                printf( "SDLK_RIGHT\n" );
                break;
            case SDLK_UP:
                printf( "SDLK_UP\n" );
                rv.environment = ACCELERATE_LEFT;
                break;
            case SDLK_DOWN:
                printf( "SDLK_DOWN\n" );
                rv.environment = ACCELERATE_RIGHT;
                break;
            case SDLK_SPACE:
                rv.environment = SHOOT_MISSILE;
                break;
            default:
                rv.window = WINDOW_UNKOWN;
                break;
        }
    }
    if ( m_Event.type == SDL_QUIT )
    {
        rv.window = WINDOW_QUIT;
    }
    return rv;
}