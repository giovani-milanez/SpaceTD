cmake_minimum_required (VERSION 2.8)
project (SpaceTD)

include_directories(include)

if (NOT DEFINED SDL2_INCLUDE_DIR)
	message(WARNING "SDL2_INCLUDE_DIR is not set! Example: Add -DSDL2_INCLUDE_DIR=C:\\SDL2\\include")
else()
	message("SDL2_INCLUDE_DIR is set to ${SDL2_INCLUDE_DIR}")
	include_directories(${SDL2_INCLUDE_DIR})
endif()

if (NOT DEFINED SDL2_LIB_DIR)
	message(WARNING "SDL2_LIB_DIR is not set! Example: Add -DSDL2_LIB_DIR=C:\\SDL2\\include\\lib")
else()
	message("SDL2_LIB_DIR is set to ${SDL2_LIB_DIR}")
	SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${SDL2_LIB_DIR})	
endif()

IF("${CMAKE_SYSTEM}" MATCHES "Linux" OR "${CMAKE_SYSTEM}" MATCHES "FreeBSD")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++11 -z defs")
	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++11 -Wall -z defs")
ELSEIF("${CMAKE_SYSTEM}" MATCHES "Windows")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
ENDIF()

set(PROJECT_VERSION "1.0.0")

file(GLOB SRC_FILES "src/*.c" "src/*.cc" "src/*.cpp")
file(GLOB_RECURSE INCS_H "include/*.h" "include/*.hpp")

add_executable(${PROJECT_NAME} ${SRC_FILES} ${INCS_H})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)

macro(my_find_library)          
    find_library(${ARGV0} ${ARGV1})    
    if (NOT ${ARGV0})
        message(FATAL_ERROR "-- could not find library ${ARGV1}")
    endif(NOT ${ARGV0})
    message("-- Library ${ARGV1} found as ${${ARGV0}}")   
endmacro()

my_find_library(SDL SDL2)
my_find_library(SDL_MAIN SDL2main)
my_find_library(SDL_IMAGE SDL2_image)
my_find_library(SDL_TTF SDL2_ttf)
my_find_library(SDL_GFX SDL2_gfx)
my_find_library(SDL_mixer SDL2_mixer)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} 
${SDL}
${SDL_MAIN}
${SDL_IMAGE}
${SDL_TTF}
${SDL_GFX}
${SDL_mixer}
)
