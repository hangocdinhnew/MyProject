#define MyProject_VERSION_MAJOR 1
#define MyProject_VERSION_MINOR 9
#define MyProject_VERSION_PATCH 4

#include <iostream>
#include <vector>
#include <cstdio>
#include <glad/glad.h>
#include <gl/gl3w.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

/* #undef INCLUDE_OPENCV */

#ifdef INCLUDE_OPENCV
    #include <opencv2/opencv.hpp>
#endif

#include "../Sound/SoundDevice.h"
#include "../Sound/SoundEffectsPlayer.h"
#include "../Sound/SoundEffectsLibrary.h"

#include <ft2build.h>
#include <freetype/freetype.h>

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui_freetype.h>
#include "../Lib/ImGuiFileDialog/ImGuiFileDialog.h"

#include "../Lib/TextEditor/TextEditor.h"
#define IMGUI_ENABLE_FREETYPE

#include <lua.h>
#include <lauxlib.h>

#include "../Lib/imguilua/lib.hpp"
#include "../Lib/imguilua/helper.hpp"
#include "../Lib/imguilua/bind.hpp"
