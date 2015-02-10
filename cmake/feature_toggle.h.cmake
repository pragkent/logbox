#ifndef FEATURE_TOGGLE_H_
#define FEATURE_TOGGLE_H_

// Define to 1 if we have __builtin_expect function
#cmakedefine HAVE_BUILTIN_EXPECT 1

// Define to 1 if we have __attribute__((noinline))
#cmakedefine HAVE_ATTRIBUTE_NOINLINE 1

// Define to 1 if we have __attribute__((noreturn))
#cmakedefine HAVE_ATTRIBUTE_NORETURN 1

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define OS_WINDOWS
#elif defined(__CYGWIN__) || defined(__CYGWIN32__)
#define OS_CYGWIN
#elif defined(linux) || defined(__linux) || defined(__linux__)
#define OS_LINUX
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
#define OS_MACOSX
#elif defined(__FreeBSD__)
#define OS_FREEBSD
#elif defined(__NetBSD__)
#define OS_NETBSD
#elif defined(__OpenBSD__)
#define OS_OPENBSD
#else
#endif

#ifdef HAVE_ATTRIBUTE_NOINLINE
#define ATTRIBUTE_NOINLINE __attribute__((noinline))
#else
#define ATTRIBUTE_NOINLINE
#endif

#ifdef HAVE_ATTRIBUTE_NORETURN
#define ATTRIBUTE_NORETURN __attribute__((noreturn))
#else
#define ATTRIBUTE_NORETURN
#endif

// GCC can be told that a certain branch is not likely to be taken (for
// instance, a CHECK failure), and use that information in static analysis.
// Giving it this information can help it optimize for the common case in
// the absence of better information (ie. -fprofile-arcs).
#ifndef LOGBOX_PREDICT_BRANCH_NOT_TAKEN
#if defined(HAVE_BUILTIN_EXPECT)
#define LOGBOX_PREDICT_BRANCH_NOT_TAKEN(x) (__builtin_expect(x, 0))
#define LOGBOX_PREDICT_FALSE(x) (__builtin_expect(x, 0))
#define LOGBOX_PREDICT_TRUE(x) (__builtin_expect(!!(x), 1))
#else
#define LOGBOX_PREDICT_BRANCH_NOT_TAKEN(x) x
#define LOGBOX_PREDICT_FALSE(x) x
#define LOGBOX_PREDICT_TRUE(x) x
#endif
#endif

#endif  // FEATURE_TOGGLE_H_
