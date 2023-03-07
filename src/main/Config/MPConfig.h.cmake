# define MyProject_VERSION_MAJOR @MyProject_VERSION_MAJOR@
# define MyProject_VERSION_MINOR @MyProject_VERSION_MINOR@
# define MyProject_VERSION_PATCH @MyProject_VERSION_PATCH@

# include <iostream>
# include <vector>
# include <cstdio>
# include <glad/glad.h>
# include <gl/gl3w.h>
# include <GLFW/glfw3.h>
# include <GLFW/glfw3native.h>
# include <imgui.h>

# cmakedefine INCLUDE_OPENCV

# ifdef INCLUDE_OPENCV
# include <opencv2/opencv.hpp>
# endif

# cmakedefine JSON_USE_IMPLICIT_CONVERSIONS

# ifdef JSON_USE_IMPLICIT_CONVERSIONS
# define JSON_USE_IMPLICIT_CONVERSIONS 1
# endif

# include "../Sound/SoundDevice.h"
# include "../Sound/SoundEffectsPlayer.h"
# include "../Sound/SoundEffectsLibrary.h"

# include <ft2build.h>
# include <freetype/freetype.h>

# include <imgui_impl_opengl3.h>
# include <imgui_impl_glfw.h>
# include <imgui_freetype.h>
# include "../Lib/ImGuiFileDialog/ImGuiFileDialog.h"

# include "../Lib/TextEditor/TextEditor.h"
# define IMGUI_ENABLE_FREETYPE
# include "../Lua/LuaInc.h"
# include <chaiscript/chaiscript.hpp>