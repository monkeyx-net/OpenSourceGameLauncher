// Dear ImGui: standalone example application for SDL2 + SDL_Renderer
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

// Important to understand: SDL_Renderer is an _optional_ component of SDL2.
// For a multi-platform app consider using e.g. SDL+DirectX on Windows and SDL+OpenGL on Linux/OSX.

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <SDL.h>
#include <fstream>
#include <sstream>
#include <iostream>


#if !SDL_VERSION_ATLEAST(2,0,17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif


std::string read_file(std::ifstream in_text )
{
    //std::ifstream in_text (in_text);
    std::string line;
    std::string all_lines;
    if (in_text.is_open())
    {
        while ( getline (in_text,line) )
        {
            all_lines += line + "\n";
        }
    in_text.close();
  }
  return all_lines;
}

// Main code
int main(int, char**)
{
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

    // Create window with SDL_Renderer graphics context
    //SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_FULLSCREEN | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("Set IP Address for game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1280, window_flags);
    if (window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return -1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        SDL_Log("Error creating SDL_Renderer!");
        return 0;
    }
    //SDL_RendererInfo info;
    //SDL_GetRendererInfo(renderer, &info);
    //SDL_Log("Current SDL_Renderer: %s", info.name);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    //Using open source Roboto Medium. Remark the line below to use defaul 13 point terminal font.
    io.Fonts->AddFontFromFileTTF("Roboto-Medium.ttf", 18.0f);
    
    // Our state
   
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


    //Read ip and instructions files before the starting the loop
    std::string ip_load=read_file(std::ifstream("ip.txt"));
    std::string instructions=read_file(std::ifstream("instructions.txt"));

    // Vector for IP Address Input  ImGui::DragInt4
    static int vec4i[] = { 127, 0, 0, 1 }; 

    // Main loop
    bool done = false;
    bool main_window = true;
    bool show_ap_address_window = false;
    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }
        
       
        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

        if (main_window)
        {
            bool p_open = true;
            // Flags for some reason having empty flags removes the collapse button, which is why I look at them!
            //ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
            ImGui::Begin("Portmaster Test",&p_open, window_flags);
            //static char str0[128] = "Edit Text Test";
            //ImGui::InputText("##Input", str0, IM_ARRAYSIZE(str0));
       
            // Creates space
            ImGui::Dummy(ImVec2(0.0f, 20.0f));
            ImGui::TextWrapped("%s",instructions.c_str());
            // Creates space
            ImGui::Dummy(ImVec2(0.0f, 20.0f));
            if (ImGui::Button("Press L1",ImVec2(200,50)) || (ImGui::IsKeyPressed(ImGuiKey_F1)))
            {
                done = true;
                return 0;

            }
            ImGui::SameLine();
             if (ImGui::Button("Press R1",ImVec2(200,50))|| (ImGui::IsKeyPressed(ImGuiKey_F2)))
            {
                //done = true;
                //return 121;   
                main_window = false;
                show_ap_address_window = true;

            }
           
            ImGui::End();
        }


         if (show_ap_address_window)
        {
            ImGui::Begin("Change IP Address", &show_ap_address_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
             ImGui::Dummy(ImVec2(0.0f, 20.0f));
            ImGui::Text("Currently set IP Address: %s",ip_load.c_str());
            ImGui::Text("Set Server IP Adress:-");
            ImGui::DragInt4("", vec4i, 1, 1, 255);
            //ImGui::SetCursorPos(ImVec2(0,350));   // Place Button
            if (ImGui::Button("Save IP Address Press R2",ImVec2(347,50))|| (ImGui::IsKeyPressed(ImGuiKey_F3)))
            {
                std::ostringstream stream;
                for (size_t i = 0; i < sizeof(vec4i)/sizeof(vec4i[0]); ++i)
                {
                 if (i) stream << '.';
                    stream << vec4i[i] ;
                }
                std::ofstream file("ip.txt");
                std::string ip_string = stream.str();
			    file << ip_string;
                file.close();
                ip_load = ip_string;
                //printf("%d.%d.%d.%d", vec4i[0],vec4i[1],vec4i[2],vec4i[3]);
            }
            ImGui::Dummy(ImVec2(0.0f, 20.0f));
            if (ImGui::Button("Start Game - Press L1",ImVec2(347,50))|| (ImGui::IsKeyPressed(ImGuiKey_F1)))
            {
                //show_ap_address_window = false;
                done = true;    
                return 121;   
             }
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}