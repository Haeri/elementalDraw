vcpkg_fail_port_install(ON_TARGET "WINDOWS" "LINUX" "UWP")

vcpkg_download_distfile(
    ARCHIVE
    URLS https://developer.apple.com/metal/cpp/files/metal-cpp_macOS12_iOS15.zip
    FILENAME metal-cpp_macOS12_iOS15.zip
    SHA512 dabb4109c7bf283288b5b3bd392892a7a52ad13b4d53a72a117a852f54b0a82871bec3e55c8493d8048365839bd1be37d72f872041f43db314622bb4a983921f
)
vcpkg_extract_source_archive(SOURCE_PATH ARCHIVE "${ARCHIVE}" NO_REMOVE_ONE_LEVEL)

file(COPY ${SOURCE_PATH}/metal-cpp/Metal DESTINATION "${CURRENT_PACKAGES_DIR}/include")
file(COPY ${SOURCE_PATH}/metal-cpp/Foundation DESTINATION "${CURRENT_PACKAGES_DIR}/include")
file(COPY ${SOURCE_PATH}/metal-cpp/QuartzCore DESTINATION "${CURRENT_PACKAGES_DIR}/include")

file(INSTALL "${SOURCE_PATH}/metal-cpp/LICENSE.txt" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)