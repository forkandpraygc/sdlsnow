cmake_minimum_required(VERSION 3.5)

project(lostsession LANGUAGES CXX)

include(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lostsession main.cpp
    ltexture.h ltexture.cpp
    snowflake.h )

target_link_libraries(lostsession ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
include(GNUInstallDirs)
install(TARGETS lostsession
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
