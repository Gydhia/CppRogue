#pragma once

// Add a signal that can be intercepted by the debugger before closing the program
#if not defined NDEBUG

#include <cstdio> // printf

#if defined __MINGW32__ || __GNUC__
// Windows + MinGW || Linux
#include <csignal>
#define CPPROGUE_DEBUGBREAK() raise(SIGTRAP)
#elif defined _WIN32 || defined __CYGWIN__
// Windows only
#define CPPROGUE_DEBUGBREAK() __debugbreak()
#else
// Not yet supported
#define CPPROGUE_DEBUGBREAK() ((void)0)
#endif

#define ASSERT_DEBUG(condition, message)                                                           \
    do {                                                                                           \
        if (!(condition))                                                                          \
        {                                                                                          \
            printf((message));                                                                     \
            CPPROGUE_DEBUGBREAK();                                                                 \
        }                                                                                          \
    } while (false)
#else
#define ASSERT_DEBUG(...) ((void)0)
#endif
