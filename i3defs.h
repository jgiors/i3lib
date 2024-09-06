#ifndef I3DEFS_H
#define I3DEFS_H
///@file

///Is this a debug build?
#ifdef NDEBUG
#   define I3DEBUG  0
#else
#   define I3DEBUG  1
#endif

///Wrapper for code that only takes effect in debug mode
#if I3DEBUG
#   define I3DEBUG_ONLY(...)    (__VA_ARGS__)
#else
#   define I3DEBUG_ONLY(...)
#endif

#define I3STRINGIZE(X)              _I3_STRINGIZE_PRIVATE(X)    ///<Stringize macro (will work with __LINE__).
#define _I3_STRINGIZE_PRIVATE(X)    #X                          ///<Private for I3STRINGIZE() only.

#define I3_LINE_NUMBER              I3STRINGIZE(__LINE__)       ///<Line number as literal string.

///Standard banner with build date/time for command-line app or unit test.
#define LIB_BANNER(APP_NAME)   APP_NAME " ~ built " __DATE__ " " __TIME__

#endif //I3DEFS_H
