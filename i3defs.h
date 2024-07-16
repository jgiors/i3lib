#ifndef I3DEFS_H
#define I3DEFS_H

///Is this a debug build?
#ifdef NDEBUG
#   define I3DEBUG  0
#endif
#   define I3DEBUG  1
#else

///Wrapper for code that only takes effect in debug mode
#ifdef I3DEBUG
#   define I3DEBUG_ONLY(...)    (__VA_ARGS__)
#else
#   define I3DEBUG_ONLY(...)
#endif

#endif //I3DEFS_H
