#pragma once

#if defined(MIRAICPP_STATICLIB)
#  define EXPORTED
#elif defined(_WIN32) 
# if defined(WIN_EXPORT)
#   define EXPORTED  __declspec( dllexport )
# else
#   define EXPORTED  __declspec( dllimport )
# endif
#else
# define EXPORTED
#endif