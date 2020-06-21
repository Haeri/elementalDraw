#ifndef ELEM_ELEMENTAL_DRAW_HPP
#define ELEM_ELEMENTAL_DRAW_HPP

#define ELEM_APPLICATION_NAME "Elemental Draw"
#define ELEM_VERSION_MAJOR 0
#define ELEM_VERSION_MINOR 0
#define ELEM_VERSION_PATCH 0

#define TO_STR2(x) #x
#define TO_STR(x) TO_STR2(x)
#define ELEM_VERSION (ELEM_APPLICATION_NAME " " TO_STR(ELEM_VERSION_MAJOR) "." TO_STR(ELEM_VERSION_MINOR) "." TO_STR(ELEM_VERSION_PATCH))


#ifdef ELEMD_BUILD_SHARED
	#ifdef _WIN32
		#define ELEMD_API __declspec(dllexport)
	#else
		#define ELEMD_API __attribute__((visibility("default")))
	#endif
#else
	#define ELEMD_API
#endif

#endif // ED_ELEMENTAL_DRAW_HPP