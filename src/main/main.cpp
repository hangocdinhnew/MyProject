/* Including the header file MPConfig.h. */
#include "Config/MPConfig.h"

/**
 * The main function.
 *
 */
int main() {
  /* Printing the version of the project. */
  std::cout << "Version: " << MyProject_VERSION_MAJOR << "."
            << MyProject_VERSION_MINOR << "." << MyProject_VERSION_PATCH;

  /* Creating a new lua state, opening the standard libraries, and then loading
   * the file test.lua. */
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);

  /* Initializing the GLFW library. */
  glfwInit();

  /* Sound */
  SoundDevice *sd = LISTENER->Get();
  int Sound1 = SE_LOAD("../Resources/sound1.wav");
  int Sound2 = SE_LOAD("../Resources/sound2.wav");
  int Egg1 = SE_LOAD("../Resources/egg1.wav");
  SoundEffectsPlayer sound_effects_player_forSound;

  // Lua
  luaL_dofile(L, "../Lua/main.lua");

  // Set GL version 3.2 and GLSL version 150
  const char *glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on Mac

  /* Creating a window with the title "MyApp" and the dimensions of SCR_WIDTH
   * and SCR_HEIGHT. */
  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MyApp", NULL, NULL);
  /* Creating a window and checking if it was created successfully. */
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  /* *|MARKER_CURSOR|* */
  glfwMakeContextCurrent(window);

  /* Setting the swap interval to 1. This means that the buffer swap will be
  synchronized with the monitor's vertical refresh. */

  glfwSwapInterval(1);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  /* Loading the OpenGL function pointers. */
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  /* GL3W */
  for (int i = 0; i < 100; ++i) {
    if (gl3wInit())
      return 3;
  }
  if (!gl3wIsSupported(3, 3))
    return 4;

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable
  // Keyboard Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; //
  // Enable Gamepad Controls

  // Default Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  cv::VideoCapture cap;
  while (!cap.isOpened()) {
    cap.open(0, cv::CAP_ANY); // try to open the camera with any available API
  }

  // Load Fonts
  // - If no fonts are loaded, dear imgui will use the default font. You can
  // also load multiple fonts and use ImGui::PushFont()/PopFont() to select
  // them.
  // - AddFontFromFileTTF() will return the ImFont* so you can store it if you
  // need to select the font among multiple.
  // - If the file cannot be loaded, the function will return NULL. Please
  // handle those errors in your application (e.g. use an assertion, or display
  // an error and quit).
  // - The fonts will be rasterized at a given size (w/ oversampling) and stored
  // into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which
  // ImGui_ImplXXXX_NewFrame below will call.
  // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype
  // for higher quality font rendering.
  // - Read 'docs/FONTS.md' for more instructions and details.
  // - Remember that in C/C++ if you want to include a backslash \ in a string
  // literal you need to write a double backslash \\ !
  // io.Fonts->AddFontDefault();
  // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
  ImFont *font = io.Fonts->AddFontFromFileTTF(
      "../Resources/font.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
  IM_ASSERT(font != NULL);

  // Our state
  bool show_demo_window = true;
  bool show_another_window = false;
  bool show_text_editor = false;
  bool show_camera_window = true;
  bool enable_camera = true;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  // SD
  sound_effects_player_forSound.SetLooping(false);

  // Prints out the OpenGL version
  std::cout << "" << std::endl;
  std::cout << ""
            << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
  std::cout << ""
            << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << ""
            << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << ""
            << "OpenGL Shading Language Version: "
            << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  std::cout << "" << std::endl;

  // Loop to check camera access status
  bool camera_access_granted = false;
  while (!camera_access_granted) {
    // Check camera access status
    camera_access_granted = cap.grab();

    // Poll events and swap buffers
    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  // CascadeClassifer
  cv::CascadeClassifier face_cascade;
  try {
    face_cascade.load("../Resources/cascade-opencv.xml");
  } catch (cv::Exception &e) {
    std::cerr << "Exception caught: " << e.what() << std::endl;
  }

  // render loop
  // -----------
  while (!glfwWindowShouldClose(window)) {
    // input
    // -----
    processInput(window);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved
    // etc.)
    // -------------------------------------------------------------------------------
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    cv::Mat frame;
    cap >> frame;

    std::vector<cv::Rect> faces;
    face_cascade.detectMultiScale(frame, faces);

    // Draw rectangles around the detected faces
    for (const auto &face : faces) {
      rectangle(frame, face, cv::Scalar(255, 0, 0), 2);
    }

    // Convert the OpenCV Mat to an OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_BGR,
                 GL_UNSIGNED_BYTE, frame.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (show_text_editor) {
      static TextEditor editor;
      static const char *fileToEdit = "helloworld.cpp";
      editor.SetLanguageDefinition(TextEditor::LanguageDefinition::CPlusPlus());
      auto cpos = editor.GetCursorPosition();
      ImGui::Begin("Text Editor", nullptr,
                   ImGuiWindowFlags_HorizontalScrollbar |
                       ImGuiWindowFlags_MenuBar);
      ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
      if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
          if (ImGui::MenuItem("Save")) {
            auto textToSave = editor.GetText();
            /// save text....
          }
          if (ImGui::MenuItem("Quit", "Alt-F4"))
            break;
          ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
          bool ro = editor.IsReadOnly();
          if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
            editor.SetReadOnly(ro);
          ImGui::Separator();

          if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr,
                              !ro && editor.CanUndo()))
            editor.Undo();
          if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr,
                              !ro && editor.CanRedo()))
            editor.Redo();

          ImGui::Separator();

          if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, editor.HasSelection()))
            editor.Copy();
          if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr,
                              !ro && editor.HasSelection()))
            editor.Cut();
          if (ImGui::MenuItem("Delete", "Del", nullptr,
                              !ro && editor.HasSelection()))
            editor.Delete();
          if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr,
                              !ro && ImGui::GetClipboardText() != nullptr))
            editor.Paste();

          ImGui::Separator();

          if (ImGui::MenuItem("Select all", nullptr, nullptr))
            editor.SetSelection(
                TextEditor::Coordinates(),
                TextEditor::Coordinates(editor.GetTotalLines(), 0));

          ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View")) {
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
      ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1,
                  cpos.mColumn + 1, editor.GetTotalLines(),
                  editor.IsOverwrite() ? "Ovr" : "Ins",
                  editor.CanUndo() ? "*" : " ",
                  editor.GetLanguageDefinition().mName.c_str(), fileToEdit);

      editor.Render("TextEditor");
    }

    // 1. Show the big demo window (Most of the sample code is in
    // ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear
    // ImGui!).
    if (show_demo_window)
      ImGui::ShowDemoWindow(&show_demo_window);

    if (show_camera_window) {
      // Display the texture in an ImGui window
      ImGui::Begin("OpenCV Camera");
      ImGui::Image((void *)(intptr_t)textureID, ImVec2(frame.cols, frame.rows));
      ImGui::End();
    }
    // 2. Show a simple window that we create ourselves. We use a Begin/End pair
    // to create a named window.
    {
      static float f = 0.0f;
      static int counter = 0;

      ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!"
                                     // and append into it.

      ImGui::Text("This is some useful text."); // Display some text (you can
                                                // use a format strings too)
      ImGui::Checkbox(
          "Demo Window",
          &show_demo_window); // Edit bools storing our window open/close state
      ImGui::Checkbox("Another Window", &show_another_window);
      ImGui::Checkbox("File Editor", &show_text_editor);
      ImGui::Checkbox("Camera window", &show_camera_window);

      if (ImGui::Checkbox("Enable Camera", &enable_camera)) {
        if (enable_camera) {
          if (!cap.isOpened()) {
            cap.open(0);
            if (!cap.isOpened()) {
              printf("Failed to open camera.\n");
              enable_camera = false;
            }
          }
        } else {
          cap.release();
        }
      }

      ImGui::SliderFloat("float", &f, 0.0f,
                         1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
      ImGui::ColorEdit3(
          "clear color",
          (float *)&clear_color); // Edit 3 floats representing a color

      if (ImGui::Button("Button")) // Buttons return true when clicked (most
                                   // widgets return true when edited/activated)
        counter++;
      ImGui::SameLine();
      ImGui::Text("counter = %d", counter);
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                  1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      ImGui::End();

      ImGui::BeginMainMenuBar();
      // File Manager
      if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Open Music")) {
          nfdchar_t *outPath;
          nfdresult_t result = NFD_OpenDialog(NULL, NULL, &outPath);
          if (result == NFD_OKAY) {
            int ConvertedSP = SE_LOAD(outPath);
            sound_effects_player_forSound.Play(ConvertedSP);
          } else if (result == NFD_CANCEL) {
            puts("NFD has being cancelled!");
          } else {
            printf("Error: %s\n", NFD_GetError());
          }
        }

        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("Sound")) {
        if (ImGui::BeginMenu("Sound Manager")) {
          // music manager
          ImGui::BeginPopupModal("Sound Manager");
          ImGui::BulletText("Sound Looping");
          if (ImGui::Button("Yes"))
            sound_effects_player_forSound.SetLooping(true);
          if (ImGui::Button("No"))
            sound_effects_player_forSound.SetLooping(false);
          ImGui::BulletText("Music List");
          if (ImGui::Button("Play sound 1"))
            sound_effects_player_forSound.Play(Sound1);
          if (ImGui::Button("Play sound 2"))
            sound_effects_player_forSound.Play(Sound2);
          ImGui::EndPopup();
        }
        ImGui::EndMenu();
      }

      ImGui::EndMainMenuBar();
    }

    // 3. Show another simple window.
    if (show_another_window) {
      ImGui::Begin(
          "Another Window",
          &show_another_window); // Pass a pointer to our bool variable (the
                                 // window will have a closing button that will
                                 // clear the bool when clicked)
      ImGui::Text("Hello from another window!");
      if (ImGui::Button("Close Me"))
        show_another_window = false;

      if (ImGui::Button("SUSSY BUTTON"))
        sound_effects_player_forSound.Play(Egg1);

      ImGui::End();
    }

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glDeleteTextures(1, &textureID);

    glfwSwapBuffers(window);
  }

  // Shutdown Imgui implementation and destroy imgui context.
  // ------------------------------------------------------------------
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  // Release opencv video capture.
  // ------------------------------------------------------------------
  cap.release();

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  lua_close(L);
  return 0;
}
