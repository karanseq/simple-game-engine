// library includes
#ifdef BUILD_DEBUG
#include <crtdbg.h>
#endif // BUILD_DEBUG

// engine includes
#include "Assert\Assert.h"
#include "Common\Engine.h"
#include "GLib.h"
#include "Logger\Logger.h"

// game includes
#include "Game\Game.h"

#ifdef ENABLE_TESTS
void RunTests();
#endif // ENABLE_TESTS

int WINAPI wWinMain( HINSTANCE i_h_instance, HINSTANCE i_h_prev_instance, LPWSTR i_lp_cmd_line, int i_n_cmd_show )
{
    // init engine
    if (engine::StartUp(i_h_instance, i_n_cmd_show, "Game", game::Game::SCREEN_WIDTH, game::Game::SCREEN_HEIGHT))
    {
#ifdef ENABLE_TESTS
        RunTests();
        LOG("\n");
#endif // ENABLE_TESTS

        // init game
        if (game::StartUp())
        {
            engine::Run();
        }

        // cleanup game
        game::Shutdown();
    }

    // cleanup
    engine::Shutdown();

    #if defined(_DEBUG)
    _CrtDumpMemoryLeaks();
    #endif // _DEBUG

    return 0;
}

