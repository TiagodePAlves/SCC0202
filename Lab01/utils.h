#ifndef __UTILS_H__
/* Shared utility macros. */
#define __UTILS_H__

// See https://gcc.gnu.org/onlinedocs/gcc-4.8.5/cpp/Stringification.html
#define __STRINGIFY(token) #token
/* Turns a token or macro into literal string. */
#define STR(token) __STRINGIFY(token)

// Change behaviour of assert on non-debug builds to avoid unused errors
#ifdef NDEBUG
#define assert(expr) ((void) expr)
#endif

/* Compiler attributes declaration. */
#define attribute(...) __attribute__((__VA_ARGS__))

#endif/*__UTILS_H__*/
