//
// Created by adolin on 20. 01. 21.
//

#include "glad/glad.h"


#ifdef ARC_PLATFORM_SDL
#include "SDL.h"
#include "imgui_impl_sdl.h"
#elif ARC_PLATFORM_GLFW
#include "glfw3.h"
#include "imgui_impl_glfw.h"
#endif

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_layer.h"
#include "engine.h"


namespace arc{
    void ImGuiLayer::Init()
    {
        bool err = gladLoadGL() == 0;
        if (err)
        {
            logce( "Failed to initialize OpenGL loader!\n");
        }
        else{
            logci("dela :(");
        }
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_IsTouchScreen;

        //io.Fonts->AddFontFromFileTTF("/home/adolin/CLionProjects/arc_engine/test_core/assets/roboto_font/Roboto-Medium.ttf", 18.0f);
        //io.FontDefault = io.Fonts->AddFontFromFileTTF("/home/adolin/CLionProjects/arc_engine/test_core/assets/roboto_font/Roboto-Medium.ttf", 18.0f);

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();

        //SetDarkThemeColors();

    #ifdef ARC_PLATFORM_SDL
        SDL_Window* window = static_cast<SDL_Window*>(Engine::window().native_window());
        ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
        ImGui_ImplOpenGL3_Init("#version 300 es");
    #elif ARC_PLATFORM_GLFW
        GLFWwindow* window = static_cast<GLFWwindow*>(Engine::window().native_window());
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    #endif


        // Setup Platform/Renderer bindings



    }

    void ImGuiLayer::Dispose()
    {
        ImGui_ImplOpenGL3_Shutdown();

    #ifdef ARC_PLATFORM_SDL
        ImGui_ImplSDL2_Shutdown();
    #elif ARC_PLATFORM_GLFW
        ImGui_ImplGlfw_Shutdown();
    #endif

        ImGui::DestroyContext();
    }

//    void ImGuiLayer::OnEvent(Event& e)
//    {
//        if (m_BlockEvents)
//        {
//            ImGuiIO& io = ImGui::GetIO();
//            e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
//            e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
//        }
//    }

    void ImGuiLayer::Begin()
    {
        ImGui_ImplOpenGL3_NewFrame();
        #ifdef ARC_PLATFORM_SDL
            ImGui_ImplSDL2_NewFrame((SDL_Window *) Engine::window().native_window());
        #elif ARC_PLATFORM_GLFW
            ImGui_ImplGlfw_NewFrame();
        #endif
        ImGui::NewFrame();
    }

    void ImGuiLayer::End()
    {
        ImGuiIO& io = ImGui::GetIO();
        const Window& window = Engine::window();
        io.DisplaySize = ImVec2((float)window.width(), (float)window.height());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImGuiLayer::SetDarkThemeColors()
    {
        auto& colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

        // Headers
        colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Buttons
        colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Frame BG
        colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Tabs
        colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
        colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
        colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

        // Title
        colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    }
}