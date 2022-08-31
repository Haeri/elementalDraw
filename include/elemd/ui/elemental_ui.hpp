#ifndef ELEM_ELEMENTAL_UI_H
#define ELEM_ELEMENTAL_UI_H

#define ELEM_LIBRARY_NAME "Elemental UI"
#define ELEM_VERSION_MAJOR 0
#define ELEM_VERSION_MINOR 1
#define ELEM_VERSION_PATCH 1

#define TO_STR2(x) #x
#define TO_STR(x) TO_STR2(x)
#define ELEM_VERSION                                                                               \
    (ELEM_LIBRARY_NAME                                                                             \
     " " TO_STR(ELEM_VERSION_MAJOR) "." TO_STR(ELEM_VERSION_MINOR) "." TO_STR(ELEM_VERSION_PATCH))

#ifdef ELEM_BUILD_SHARED
#ifdef _MSC_VER
#ifdef elem_EXPORTS
#define ELEM_API __declspec(dllexport)
#else
#define ELEM_API __declspec(dllimport)
#endif
#else
#define ELEM_API __attribute__((visibility("default")))
#endif
#else
#define ELEM_API
#endif

#endif // ELEM_ELEMENTAL_UI_H