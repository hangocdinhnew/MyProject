enable_testing()

set(GRAPHICS_TESTING_NAME "Graphics_Test")

include_directories(
    ${GRAPHICS_TESTING_NAME}
    Graphics/
)

add_executable(
    ${GRAPHICS_TESTING_NAME}
    Graphics/Graphics.cpp
)

find_package(GTest CONFIG REQUIRED)
target_link_libraries(${GRAPHICS_TESTING_NAME} PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${GRAPHICS_TESTING_NAME} PRIVATE glfw)

add_test(NAME test1 COMMAND ${GRAPHICS_TESTING_NAME} arg1 WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})