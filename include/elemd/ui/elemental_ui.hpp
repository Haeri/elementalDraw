#ifndef ELEMD_ELEMENTAL_UI_H
#define ELEMD_ELEMENTAL_UI_H

#define ELEMD_LIBRARY_NAME "Elemental UI"
#define ELEMD_VERSION_MAJOR 0
#define ELEMD_VERSION_MINOR 1
#define ELEMD_VERSION_PATCH 1

#define TO_STR2(x) #x
#define TO_STR(x) TO_STR2(x)
#define ELEMD_VERSION                                                                              \
    (ELEMD_LIBRARY_NAME " " TO_STR(ELEMD_VERSION_MAJOR) "." TO_STR(                                \
        ELEMD_VERSION_MINOR) "." TO_STR(ELEMD_VERSION_PATCH))

#ifdef ELEMD_BUILD_SHARED
#ifdef _MSC_VER
#ifdef ELEMD_EXPORTS
#define ELEMD_API __declspec(dllexport)
#else
#define ELEMD_API __declspec(dllimport)
#endif
#else
#define ELEMD_API __attribute__((visibility("default")))
#endif
#else
#define ELEMD_API
#endif

#endif // ELEMD_ELEMENTAL_UI_H