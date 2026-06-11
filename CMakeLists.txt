cmake_minimum_required(VERSION 3.22)

project(OceanEngine C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_VERBOSE_MAKEFILE ON)

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
    glew
    GIT_REPOSITORY https://github.com/Perlmint/glew-cmake
    GIT_TAG glew-cmake-2.3.1
)

FetchContent_MakeAvailable(glew)

# add_compile_options(-fsanitize=address -fno-omit-frame-pointer -g)
# add_link_options(-fsanitize=address -fno-omit-frame-pointer -g)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_definitions(_DEBUG)
endif()

macro(recursive_add_all)
    #include all source files into main list
    file(GLOB_RECURSE LOCAL_PROJECT_SOURCES CONFIGURE_DEPENDS *.h *.c)
    set(PROJECT_SOURCES ${PROJECT_SOURCES}${LOCAL_PROJECT_SOURCES}\; PARENT_SCOPE)

    #include all subdirectories
    file(GLOB_RECURSE FILE_LIST LIST_DIRECTORIES true CONFIGURE_DEPENDS *)
    foreach(DIR ${FILE_LIST})
        if(IS_DIRECTORY ${DIR})
            set(LOCAL_DIRECTORIES ${LOCAL_DIRECTORIES}${DIR}\;)
        endif()
    endforeach()

    #include this directory
    set(LOCAL_DIRECTORIES ${LOCAL_DIRECTORIES}${CMAKE_CURRENT_LIST_DIR}\;)

    set(PROJECT_DIRECTORIES ${PROJECT_DIRECTORIES}\;${LOCAL_DIRECTORIES} PARENT_SCOPE)
endmacro()

add_subdirectory(./source/BLoader)
add_subdirectory(./source/Constructure)
add_subdirectory(./source/MokyoMidi)
add_subdirectory(./source/Trifecta)
add_subdirectory(./source/Unknown)
add_subdirectory(./source/WindECS)
add_subdirectory(./source/ZMath)
add_subdirectory(./source/PGUtil)
add_subdirectory(./source/AAAgame)

# copy scripts
add_custom_target(script)
add_custom_command(TARGET script POST_BUILD COMMAND ${CMAKE_COMMAND}
        -E rm -rf ${CMAKE_BINARY_DIR}/res/script)
add_custom_command(TARGET script POST_BUILD COMMAND ${CMAKE_COMMAND}
        -E copy_directory
        ${CMAKE_SOURCE_DIR}/script/ ${CMAKE_BINARY_DIR}/res/script)

include_directories(${PROJECT_DIRECTORIES})
# include_directories(${CMAKE_SOURCE_DIR}/libraries/glfw/include)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

add_dependencies(${PROJECT_NAME} script)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)

if(WIN32)
    add_compile_definitions(GLEW_STATIC)
    set_target_properties(OceanEngine PROPERTIES COMPILE_FLAGS "/experimental:c11atomics")
    set(LIBS
        "winmm"
        "opengl32"
        glfw
        libglew_static
    )
    target_link_libraries(OceanEngine ${LIBS})
elseif(APPLE)
    add_compile_definitions(__unix__) # not sure why undefined
    # https://stackoverflow.com/questions/17070101/why-i-cannot-link-the-mac-framework-file-with-cmake
    set(LIBS 
        "-framework CoreServices"
        "-framework AudioUnit"
        "-framework AudioToolbox"
        "-framework IOKit"
        "-framework CoreGraphics"
        "-framework AppKit"
        "-framework OpenGL"
        glfw
    )
    target_link_libraries(OceanEngine ${LIBS})
    set_target_properties(OceanEngine PROPERTIES LINK_FLAGS "-Wl,-F/Library/Frameworks")
elseif(UNIX)
    add_compile_definitions(GLEW_STATIC _POSIX_C_SOURCE)
    set(LIBS
        "GL"
        # "glfw3"
        "pthread"
        "m"
        "asound"
        glfw
        glew
        # ${CMAKE_SOURCE_DIR}/libraries/libGLEW.a
    )
    target_link_libraries(OceanEngine ${LIBS})
endif()