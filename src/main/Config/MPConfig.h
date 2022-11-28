#define MyProject_VERSION_MAJOR 2
#define MyProject_VERSION_MINOR 0

#include <iostream>
#include <vector>
#include <stdio.h>
#include <glad/glad.h>
#include <gl/gl3w.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

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
