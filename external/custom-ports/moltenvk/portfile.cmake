vcpkg_fail_port_install(ON_TARGET "WINDOWS" "LINUX" "UWP")

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO KhronosGroup/MoltenVK
    REF ce85a96d8041b208e6f32898912b217957019b5a
    SHA512 fa77a807a6e17fa7359b13546d46e5c5ebb4b7180df0a6fe378ff407c71c40670417ce135195db452df0d2fd1eaa51e39dda6743a1bbf19a6a68417d5e18e360
    HEAD_REF master
	PATCHES
    	added_configure.patch
)

vcpkg_extract_source_archive_ex(
    OUT_SOURCE_PATH SOURCE_PATH
    ARCHIVE ${ARCHIVE}
)

vcpkg_execute_required_process(
    COMMAND fetchDependencies --macos --ios
    WORKING_DIRECTORY ${SOURCE_PATH}
)

vcpkg_configure_make(
    SOURCE_PATH ${SOURCE_PATH}
 	#SKIP_CONFIGURE this doesn't work atm
#    PREFER_NINJA
)

vcpkg_install_make(BUILD_TARGET "macos")