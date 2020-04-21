#pragma once

// Define EXPORTED for any platform
#if defined(_WIN32) && defined(SHARED_LIB)
# ifdef WIN_EXPORT
#   define EXPORTED  __declspec( dllexport )
# else
#   define EXPORTED  __declspec( dllimport )
# endif
#else
# define EXPORTED
#endif