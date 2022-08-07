
#include "Game/GameHandler.h"
int
main( int argc, char **argv )
{
    GameHandler *gh = new GameHandler();
    for (;;)
    {

        gh->handleStartScreen();
    }
    return 0;
}