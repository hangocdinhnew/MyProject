#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <GLFW/glfw3.h>

using namespace testing;

TEST(GLFWTest, WindowCreation)
{
    // Initialize GLFW
    ASSERT_TRUE(glfwInit());

    // Create a GLFW window
    GLFWwindow *window = glfwCreateWindow(640, 480, "Test Window", NULL, NULL);

    // Verify that the window was created successfully
    ASSERT_NE(window, nullptr);

    // Clean up GLFW
    glfwTerminate();
}

// Main function to run all tests
int main(int argc, char **argv)
{
    // Initialize Google Test
    ::testing::InitGoogleTest(&argc, argv);
    // Run all tests
    return RUN_ALL_TESTS();
}