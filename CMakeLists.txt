cmake_minimum_required(VERSION 3.5)

project(smb
    DESCRIPTION "pntr_app port of SMB"
    HOMEPAGE_URL "https://github.com/konsumer/pntr_smb"
    VERSION 0.0.1
    LANGUAGES C
)

option(PNTR_APP_BUILD_TEST "Build Unit-tests" TRUE)
option(PNTR_APP_BUILD_DEMOS "Build Examples" TRUE)

# find cmake stuff in current dir/cmake
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# general settings that make things build better

# https://cmake.org/cmake/help/latest/policy/CMP0135.html
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
  set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)
endif()

# https://cmake.org/cmake/help/latest/policy/CMP0077.html
if(POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
  set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
endif()

# enable colors for ninja
set(CMAKE_COLOR_DIAGNOSTICS ON)

# switch these for debugging
set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE Release)

find_package(pntr REQUIRED)
find_package(pntr_app REQUIRED)
find_package(pntr_tiled REQUIRED)

add_executable("${PROJECT_NAME}" src/main.c)
target_link_libraries("${PROJECT_NAME}" PUBLIC pntr pntr_app pntr_tiled)


if (EMSCRIPTEN)
  set_target_properties("${PROJECT_NAME}" PROPERTIES
    SUFFIX ".mjs"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/docs"
    LINK_FLAGS "-s ASSERTIONS=1 -sSINGLE_FILE --preload-file ../assets@assets"
  )
  target_compile_definitions("${PROJECT_NAME}" PUBLIC PNTR_APP_WEB)
else()
  find_package(raylib REQUIRED)
  find_package(retro REQUIRED)
  
  target_compile_definitions("${PROJECT_NAME}" PUBLIC PNTR_APP_RAYLIB)
  target_link_libraries("${PROJECT_NAME}" PUBLIC raylib)
  set_target_properties("${PROJECT_NAME}" PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}"
  )

  add_library("${PROJECT_NAME}-libretro" SHARED src/main.c)
  target_sources("${PROJECT_NAME}-libretro" PRIVATE
    "${retro_SOURCE_DIR}/audio/audio_mixer.c"
    "${retro_SOURCE_DIR}/audio/conversion/float_to_s16.c"
    "${retro_SOURCE_DIR}/memmap/memalign.c"
  )
  target_compile_definitions("${PROJECT_NAME}-libretro" PUBLIC PNTR_APP_LIBRETRO)
  target_link_libraries("${PROJECT_NAME}-libretro" PUBLIC pntr pntr_app)
  set_target_properties("${PROJECT_NAME}-libretro" PROPERTIES
    PREFIX ""
    POSITION_INDEPENDENT_CODE ON
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}"
  )
endif()


