

file (GLOB sources CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/lib/*.cpp")

file (GLOB headers CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/*.h")

add_library(emu STATIC ${sources} ${headers})

target_include_directories(emu PUBLIC ${PROJECT_SOURCE_DIR}/include )


if (WIN32)
  target_include_directories(emu PUBLIC "${PROJECT_SOURCE_DIR}/../src/include" )
  target_include_directories(emu PUBLIC ${PROJECT_SOURCE_DIR}/../src/sdl2_ttf/include )
else()
  target_include_directories(emu PUBLIC ${SDL2_INCLUDE_DIR})
  target_link_libraries(emu PUBLIC ${SDL2_LIBRARY}) 
  target_link_libraries(emu PUBLIC ${SDL2_TTF_LIBRARY}) 
endif()

include_directories("/usr/local/include")
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(emu ${SDL2_LIBRARIES})
target_link_libraries(emu ${SDL2_TTF_LIBRARIES})