set(AudioThumbs_SRCS AudioThumbs.cpp)

add_library(AudioThumbs MODULE ${AudioThumbs_SRCS})

target_link_libraries(AudioThumbs PRIVATE KF5::KIOWidgets ${TAGLIB_LIBRARIES} ${FLAC_LIBRARIES})

install(TARGETS AudioThumbs DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES AudioThumbs.desktop DESTINATION ${SERVICES_INSTALL_DIR})
