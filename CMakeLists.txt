cmake_minimum_required(VERSION 3.20.0)
# Set Generator and build type
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_GENERATOR "Ninja")
# Set Project Name and Version 
project(
    MyProject
    VERSION 1.0.0
    DESCRIPTION "The Greatest App Ever"
)

# Build type and options
#ADD SOURCES
set(MyProject_DIR_ROOT ${CMAKE_CURRENT_LIST_DIR})
set(MyProject_DIR_SRC "${MyProject_DIR_ROOT}/src")
set(MyProject_DIR_RES "${MyProject_DIR_ROOT}/res")
set(MyProject_BundleApp_ResourcesDir "${CMAKE_CURRENT_BINARY_DIR}/MyProject.app/Contents/Resources/")
set(MyProject_SRC_CFG_DIR ${MyProject_DIR_SRC}/Config/)
set(MyProject_CFG_APPEND 
    ${MyProject_SRC_CFG_DIR}/MPConfig.h.in
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
MyProject_SOURCES_APPEND(${MyProject_DIR_SRC})

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

#Add STB
set(STB_INCLUDE_DIRS "./src/Lib/stb/")
target_include_directories(${PROJECT_NAME} PRIVATE ${STB_INCLUDE_DIRS})

#Add GTest
#ind_package(GTest CONFIG REQUIRED)
#target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
# If you want to