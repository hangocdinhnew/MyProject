cmake_minimum_required(VERSION 3.20.0)
# Set Generator and build type
if (CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_CXX_FLAGS "-O3")
elseif (CMAKE_BUILD_TYPE MATCHES Trace)
    set(CMAKE_CXX_FLAGS "-O3")
    add_compile_definitions(ENABLE_TRACING)
elseif (CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS "-g")
endif ()
# Toolchain, CXX Standard Flags and Build Type
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(CMAKE_TOOLCHAIN_FILE "~/DEV/vcpkg/scripts/buildsystems/vcpkg.cmake")
# Get Submodules
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()
# Set Project Name and Version
project(
    MyProject
    LANGUAGES C CXX
    VERSION 1.9.1
    DESCRIPTION "The Greatest App Ever"
)

#ADD SOURCES
set(MyProject_DIR_ROOT ${CMAKE_CURRENT_LIST_DIR})
set(MyProject_DIR_SRC "${MyProject_DIR_ROOT}/src")
set(MyProject_DIR_RES "${MyProject_DIR_ROOT}/res")
set(MyProject_BundleApp_ResourcesDir "${CMAKE_CURRENT_BINARY_DIR}/MyProject.app/Contents/Resources/")
set(MyProject_SRC_CFG_DIR ${MyProject_DIR_SRC}/main/Config/)
set(MyProject_CFG_APPEND 
    ${MyProject_SRC_CFG_DIR}/MPConfig.h.cmake
    ${MyProject_SRC_CFG_DIR}/MPConfig.h 
)

#ADD VCPKG PREFIX PATH TO CMAKE
set(CMAKE_PREFIX_PATH "~/DEV/vcpkg/installed/arm64-osx/share")
# ONLY IF DIDN'T CLONE THE VCPKG

#ADD RESOURCES 
set(MACOSX_BUNDLE_ICON_FILE ico.icns)
set(MyProject_ICON "${MyProject_DIR_RES}/ico.icns")
set_source_files_properties(${MyProject_ICON}
    PROPERTIES
    MACOSX_PACKAGE_LOCATION "Resources"
)

# Set Resources
file(GLOB_RECURSE RESOURCES_APPEND CONFIGURE_DEPENDS
    "*.ttf"
    "*.mp3.mpeg"
)

list(APPEND MyProject_Resources_FILES ${RESOURCES_APPEND})
file(COPY ${MyProject_Resources_FILES} DESTINATION ${MyProject_BundleApp_ResourcesDir})

#ADD FILES_APPEND
macro(MyProject_FILES_APPEND)
    file(GLOB_RECURSE FILES_APPEND CONFIGURE_DEPENDS ${ARGV})
    list(APPEND MyProject_SOURCES ${FILES_APPEND})
endmacro()
#ADD SOURCES
macro(MyProject_SOURCES_APPEND)
    MyProject_FILES_APPEND(${ARGV0}/*.h)
    MyProject_FILES_APPEND(${ARGV0}/*.cpp)
endmacro()

# Set the project folder you can use GLOB to manually do it but it's not very automatically tho
MyProject_SOURCES_APPEND(${MyProject_DIR_SRC}/main)

add_executable(${PROJECT_NAME}
    MACOSX_BUNDLE
    ${MyProject_ICON}
    ${MyProject_SOURCES}
)
#Add configuration files
configure_file(
    ${MyProject_CFG_APPEND}
)

#Add some libraries
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR})
#Add Graphics Libraries
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw "-framework OpenGL")

find_package(imgui CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)

find_package(glad CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)

find_package(gl3w CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE unofficial::gl3w::gl3w)

#OpenCV
find_package(OpenCV CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE opencv_ml tegra_hal opencv_dnn opencv_core)
#OpenCV

#Add STB
find_path(STB_INCLUDE_DIRS "stb_c_lexer.h")
target_include_directories(${PROJECT_NAME} PRIVATE ${STB_INCLUDE_DIRS})

#Add Audio Libraries
find_package(OpenAL CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenAL::OpenAL "-framework OpenAL")
#Add SndFile
find_package(SndFile CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SndFile::sndfile)
#Add Freetype
find_package(freetype CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE freetype)

#Add Unit Testing Framework

find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)

#find_package(GTest CONFIG REQUIRED)
#target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)

#Options
option(USE_DOXYGEN "Use Build Documentation" ${USE_DOXYGEN})
option(USE_CPPCHECK "Use Static anylist application" TRUE)
option(INCLUDE_OPENCV "Include OpenCV" ${INCLUDE_OPENCV})

if(USE_DOXYGEN)
    if (CMAKE_BUILD_TYPE MATCHES "Release")
        find_package(Doxygen)
        if (DOXYGEN_FOUND)
            # set input and output files
            set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
            set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

            # request to configure the file
            configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
            message("Doxygen build started")

            # note the option ALL which allows to build the docs together with the application
            add_custom_target( doc_doxygen ALL
                COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "Generating API documentation with Doxygen"
                VERBATIM )
        else (DOXYGEN_FOUND)
        message("Doxygen need to be installed to generate the doxygen documentation")
        endif (DOXYGEN_FOUND)
    endif()
endif()

if(USE_CPPCHECK)
    if (CMAKE_BUILD_TYPE MATCHES "Release")  
        find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
        if (CMAKE_CXX_CPPCHECK)
            list(
                APPEND CMAKE_CXX_CPPCHECK 
                    "--enable=warning"
                    "--inconclusive"
                    "--force" 
                    "--inline-suppr"
            )
        endif()
    endif()
endif()