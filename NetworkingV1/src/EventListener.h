#ifndef EVENTLISTENER__H_
#define EVENTLISTENER__H_
#include "math/myTypes.h" // changed path
#include "../../include/SDL2/SDL.h"
#include <cstdint>


class EventListener
{
public:
    enum WindowStates
    {
        WINDOW_UNKOWN    = 0,
        WINDOW_QUIT      = 1,
        WINDOW_INACTIVE  = 2,
        WINDOW_ACTIVE    = 3,
        WINDOW_REDRAW    = 4,
        WINDOW_MAXIMIZED = 5,
        WINDOW_ACTION,

    };
    enum EnvironmentStates
    {
        IDLE,
        ACCELERATE_LEFT,
        ACCELERATE_RIGHT,
        SHOOT_MISSILE,
        ANGLE_TURN_RIGHT,
        ANGLE_TURN_LEFT,

    };
    typedef struct
{
        uint32_t windowType;
    uint32_t environmentType;
    uint8_t window;
    uint8_t environment;
    

 }EventType_t;
    EventListener();
    ~EventListener();
    void      setScreenSize( uint32_t x, uint32_t y );
    IntPoint2D_t getScreenSize();
    void      processInput();
    EventType_t checkEvents();

private:
    SDL_Event    m_Event;
    IntPoint2D_t m_ScreenSize;
    const Uint8 *m_KeyState;
    int          m_Key;
};
#endif
