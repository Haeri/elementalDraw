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

vcpkg_execute_required_process(
    COMMAND ./fetchDependencies --macos
    WORKING_DIRECTORY ${SOURCE_PATH}
)

vcpkg_configure_make(
    SOURCE_PATH ${SOURCE_PATH}
 	#SKIP_CONFIGURE this doesn't work atm
    COPY_SOURCE
    PREFER_NINJA
)

vcpkg_build_make(BUILD_TARGET macos)

# copy copyright
file(INSTALL ${SOURCE_PATH}/LICENSE DESTINATION ${CURRENT_INSTALLED_DIR}/share/moltenvk RENAME copyright)
file(INSTALL ${SOURCE_PATH}/LICENSE DESTINATION ${CURRENT_PACKAGES_DIR}/share/moltenvk RENAME copyright)

# copy MoltenVK include
file(INSTALL ${SOURCE_PATH}/x64-osx-rel/Package/Latest/MoltenVK/include DESTINATION ${CURRENT_INSTALLED_DIR}/include)
file(INSTALL ${SOURCE_PATH}/x64-osx-rel/Package/Latest/MoltenVK/include DESTINATION ${CURRENT_PACKAGES_DIR}/include)

# copy MoltenVKShaderConverter include
file(INSTALL ${SOURCE_PATH}/x64-osx-rel/Package/Latest/MoltenVKShaderConverter/include ${CURRENT_INSTALLED_DIR}/include)
file(INSTALL ${SOURCE_PATH}/x64-osx-rel/Package/Latest/MoltenVKShaderConverter/include ${CURRENT_PACKAGES_DIR}/include)

# copy tools
file(INSTALL ${SOURCE_PATH}/x64-osx-rel/Package/Latest/MoltenVKShaderConverter/Tools/MoltenVKShaderConverter ${CURRENT_INSTALLED_DIR}/tools/moltenvk)
file(INSTALL ${SOURCE_PATH}/x64-osx-rel/Package/Latest/MoltenVKShaderConverter/Tools/MoltenVKShaderConverter ${CURRENT_PACKAGES_DIR}/tools/moltenvk)


# TODO copy debug static
# TODO copy release static
# TODO copy debug dynamic
# TODO copy release dynamic