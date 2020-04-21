#pragma once

// Define EXPORTED for any platform
#if defined(_WIN32) 
# if defined(WIN_EXPORT) && defined(SHARED_LIB)
#   define EXPORTED  __declspec( dllexport )
# else
#   define EXPORTED  __declspec( dllimport )
# endif
#else
# define EXPORTED
#endif