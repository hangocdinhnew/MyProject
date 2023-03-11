/* Including the header file MPConfig.h. */
#include "Config/MPConfig.h"

// Create framebuffer size
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

// Create Input Key
void processInput(GLFWwindow *window);

/* Defining the width and height of the screen. */
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

/* The above code is a vertex shader. It is a program that runs on the GPU. It is written in GLSL. */
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";
/* The above code is creating a fragment shader. */
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\n\0";

/**
 * The main function.
 *
 * @param argc the number of arguments passed to the program
 * @param argv the array of strings that are the command line arguments
 */
int main(int argc, char *argv[])
{
    /* Printing the version of the project. */
    std::cout << "Version: " << MyProject_VERSION_MAJOR << "." << MyProject_VERSION_MINOR << "." << MyProject_VERSION_PATCH;

    /* Creating a new lua state, opening the standard libraries, and then loading the file test.lua. */
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    luaL_dofile(L, "../Lua/test.lua");

    // Initializing Chaiscript
    chaiscript::ChaiScript chai;
    chai.eval(R"(
        puts("Chaiscript initialized.\n");
        puts("\n");
    )");

    /* Initializing the GLFW library. */
    glfwInit();

    /* Sound */
    SoundDevice *sd = LISTENER->Get();
    int Music = SE_LOAD("../Resources/sound1.mp3.mpeg");
    int Music1 = SE_LOAD("../Resources/music1.mp3");
    int Egg1 = SE_LOAD("../Resources/egg1.mp3");
    SoundEffectsPlayer sound_effects_player_forMusic;

    // Lua
    luaL_dofile(L, "../Lua/main.lua");

/* Setting the version of OpenGL to use. */
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char *glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char *glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    /* Creating a window with the title "MyApp" and the dimensions of SCR_WIDTH and SCR_HEIGHT. */
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MyApp", NULL, NULL);
    /* Creating a window and checking if it was created successfully. */
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    /* *|MARKER_CURSOR|* */
    glfwMakeContextCurrent(window);

    /* Setting the swap interval to 1. This means that the buffer swap will be synchronized with the
    monitor's vertical refresh. */

    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /* Loading the OpenGL function pointers. */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    /* GL3W */
    for (int i = 0; i < 100; ++i)
    {
        if (gl3wInit())
            return 3;
    }
    if (!gl3wIsSupported(3, 3))
        return 4;

    /* *|MARKER_CURSOR|* */
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    /* The above code is checking to see if the vertex shader compiled successfully. If it did not, it
    prints out the error message. */
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
    // fragmentShader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    /* *|MARKER_CURSOR|* */
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    /* The above code is checking to see if the fragment shader compiled successfully. If it did not, it
    prints out the error message. */
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
    /* Creating a shader program and link shaders. */
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left
        0.5f, -0.5f, 0.0f,  // right
        0.0f, 0.5f, 0.0f    // top
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // io.Fonts->AddFontDefault();
    // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    ImFont *font = io.Fonts->AddFontFromFileTTF("../Resources/font.ttf", 18.0f, NULL,
                                                io.Fonts->GetGlyphRangesJapanese());
    IM_ASSERT(font != NULL);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    bool show_text_editor = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // SD
    sound_effects_player_forMusic.SetLooping(false);

    // Prints out the OpenGL version
    std::cout << "" << std::endl;
    std::cout << ""
              << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << ""
              << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << ""
              << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << ""
              << "OpenGL Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "" << std::endl;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        if (show_text_editor)
        {
            static TextEditor editor;
            static const char *fileToEdit = "helloworld.cpp";
            editor.SetLanguageDefinition(TextEditor::LanguageDefinition::CPlusPlus());
            auto cpos = editor.GetCursorPosition();
            ImGui::Begin("Text Editor", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
            ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("Save"))
                    {
                        auto textToSave = editor.GetText();
                        /// save text....
                    }
                    if (ImGui::MenuItem("Quit", "Alt-F4"))
                        break;
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Edit"))
                {
                    bool ro = editor.IsReadOnly();
                    if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
                        editor.SetReadOnly(ro);
                    ImGui::Separator();

                    if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && editor.CanUndo()))
                        editor.Undo();
                    if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && editor.CanRedo()))
                        editor.Redo();

                    ImGui::Separator();

                    if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, editor.HasSelection()))
                        editor.Copy();
                    if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && editor.HasSelection()))
                        editor.Cut();
                    if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && editor.HasSelection()))
                        editor.Delete();
                    if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
                        editor.Paste();

                    ImGui::Separator();

                    if (ImGui::MenuItem("Select all", nullptr, nullptr))
                        editor.SetSelection(TextEditor::Coordinates(),
                                            TextEditor::Coordinates(editor.GetTotalLines(), 0));

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("View"))
                {
                    if (ImGui::MenuItem("Dark palette"))
                        editor.SetPalette(TextEditor::GetDarkPalette());
                    if (ImGui::MenuItem("Light palette"))
                        editor.SetPalette(TextEditor::GetLightPalette());
                    if (ImGui::MenuItem("Retro blue palette"))
                        editor.SetPalette(TextEditor::GetRetroBluePalette());
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
            ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1,
                        editor.GetTotalLines(),
                        editor.IsOverwrite() ? "Ovr" : "Ins",
                        editor.CanUndo() ? "*" : " ",
                        editor.GetLanguageDefinition().mName.c_str(), fileToEdit);

            editor.Render("TextEditor");
        }

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");          // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);
            ImGui::Checkbox("File Editor", &show_text_editor);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);             // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float *)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button(
                    "Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                        ImGui::GetIO().Framerate);
            ImGui::End();

            ImGui::BeginMainMenuBar();
            // File Manager
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::BeginMenu("Open"))
                {
                    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".cpp,.h,.hpp", ".", 1,
                                                            nullptr, ImGuiFileDialogFlags_Modal);
                    ImGui::EndMenu();
                }

                // display
                if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
                {
                    // action if OK
                    if (ImGuiFileDialog::Instance()->IsOk())
                    {
                        std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                        std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
                        // action
                    }

                    // close
                    ImGuiFileDialog::Instance()->Close();
                }

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Music"))
            {
                if (ImGui::BeginMenu("Music Manager"))
                {
                    // music manager
                    ImGui::BeginPopupModal("Music Manager");
                    ImGui::BulletText("Sound Looping");
                    ImGui::Text("Use this for more good experience of easter eggs sounds.");
                    if (ImGui::Button("Yes"))
                        sound_effects_player_forMusic.SetLooping(true);
                    if (ImGui::Button("No"))
                        sound_effects_player_forMusic.SetLooping(false);
                    ImGui::BulletText("Music List");
                    if (ImGui::Button("Play music 1"))
                        sound_effects_player_forMusic.Play(Music);
                    if (ImGui::Button("Play music 2"))
                        sound_effects_player_forMusic.Play(Music1);
                    ImGui::EndPopup();
                }
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window",
                         &show_another_window); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;

            if (ImGui::Button("SUSSY BUTTON"))
                sound_effects_player_forMusic.Play(Egg1);

            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w,
                     clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    lua_close(L);
    return 0;
}

/**
 * It tells OpenGL the size of the rendering window so OpenGL knows how we want to display the data and
 * coordinates with respect to the window
 *
 * @param window The window whose size, position, or video mode was changed.
 */

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

/**
 * Whenever the window is resized, the viewport is set to match the new window dimensions
 *
 * @param window The window that received the event.
 * @param width the width of the framebuffer in pixels
 * @param height the height of the framebuffer in pixels
 */
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
