cmake_minimum_required(VERSION 3.1)

set(PRJNAME VGSynth)

project(${PRJNAME})


find_library(PTHREAD_LIBRARY pthread)
find_package(SDL2 REQUIRED)
# find_package(SDL2TFT REQUIRED)


include_directories(${SDL2_INCLUDE_DIRS})


set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE source_list_root  src/*.cpp src/*.hpp )
file(GLOB_RECURSE source_list_drv   src/driver/*.cpp src/driver/*.hpp )
file(GLOB_RECURSE source_list_ins   src/instruments/*.cpp src/instruments/*.hpp )
file(GLOB_RECURSE source_list_midi  src/midi/*.cpp src/midi/*.hpp )
file(GLOB_RECURSE srclist_runtime   src/runtime/*.cpp src/runtime/*.hpp )
file(GLOB_RECURSE srclist_bridges   src/bridges/*.cpp src/bridges/*.hpp )
file(GLOB_RECURSE srclist_math      src/math/*.cpp src/math/*.hpp )
file(GLOB_RECURSE srclist_fsys      src/fsys/*.cpp src/fsys/*.hpp )
file(GLOB_RECURSE srclist_wnd       src/wnd/*.cpp src/wnd/*.hpp src/wnd/*.h )
file(GLOB_RECURSE srclist_osc       src/osciloscope/*.cpp src/osciloscope/*.hpp )
file(GLOB_RECURSE srclist_abuffer   src/abuffer/*.cpp src/abuffer/*.hpp )
file(GLOB_RECURSE srclist_channel   src/channel/*.cpp src/channel/*.hpp )


add_executable(${PRJNAME}

    ${source_list_root}
    ${source_list_drv}
    ${source_list_ins}
    ${source_list_midi}
    ${srclist_runtime}
    ${srclist_math}
    ${srclist_fsys}
    ${srclist_wnd}
    ${srclist_osc}
    ${srclist_abuffer}
    ${srclist_channel}

)

# -lopenal openal

target_link_libraries(${PRJNAME} ${SDL2_LIBRARIES} ${PTHREAD_LIBRARY} openal asound SDL2_ttf)

target_include_directories(${PRJNAME}
        PRIVATE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/abuffer>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/channel>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/driver>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/instruments>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/patterns>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/osciloscope>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/wnd>
)
