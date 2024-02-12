// Dear ImGui: standalone example application for SDL2 + SDL_Renderer
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

// Important to understand: SDL_Renderer is an _optional_ component of SDL2.
// For a multi-platform app consider using e.g. SDL+DirectX on Windows and SDL+OpenGL on Linux/OSX.
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <SDL.h>
#include <SDL_image.h>
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

// Run system command and capture output

std::string exec(const char* cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}




// Load texture for IMGUI
bool LoadTextureFromFile(const char* filename, SDL_Texture** texture_ptr, int& width, int& height, SDL_Renderer* renderer) 
{
    int channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);

    if (data == nullptr) {
        fprintf(stderr, "Failed to load image: %s\n", stbi_failure_reason());
        return false;
    }

    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom((void*)data, width, height, channels * 8, channels * width,
                                                    0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

    if (surface == nullptr) {
        fprintf(stderr, "Failed to create SDL surface: %s\n", SDL_GetError());
        return false;
    }

    *texture_ptr = SDL_CreateTextureFromSurface(renderer, surface);

    if ((*texture_ptr) == nullptr) {
        fprintf(stderr, "Failed to create SDL texture: %s\n", SDL_GetError());
    }

    SDL_FreeSurface(surface);
    stbi_image_free(data);

    return true;
}

// Main code
int main(int, char**)
{

    SDL_Texture *texture = NULL;
    //SDL_Renderer *renderer = NULL;
    bool done = false;
    bool main_window = true;
    bool show_ap_address_window = false;
    bool ret = false;
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }


SDL_Texture* my_texture;
int my_image_width, my_image_height;


    // From 2.0.18: Enable native IME.
    #ifdef SDL_HINT_IME_SHOW_UI
      SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
    #endif

    // Create window with SDL_Renderer graphics context
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    //SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_FULLSCREEN | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("Set IP Address for game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1280, window_flags);
    if (window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return -1;
    }
    // add error checking for img ang fft files etc
   

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        SDL_Log("Error creating SDL_Renderer!");
        return 0;
    }


    if (ret == LoadTextureFromFile("Assets/screenshot.png", &my_texture, my_image_width, my_image_height, renderer))
    {
        printf("Error: SDL_CreateWindow():");
        return false;
    }

    

    //setup image
    IMG_Init(IMG_INIT_PNG);
    texture = IMG_LoadTexture(renderer, "Assets/battlezone.png");
    
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
    // Add error checking and default to system font.
    io.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto-Medium.ttf", 18.0f);
    
    // Colour state
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


    //Read ip and instructions files before the starting the loop
    std::string ip_load=read_file(std::ifstream("ip.txt"));
    std::string instructions=read_file(std::ifstream("instructions.txt"));

    // Vector for IP Address Input  ImGui::DragInt4
    static int vec4i[] = { 127, 0, 0, 1 }; 

    // Main loop
    
    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;

        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
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
 
        //Full Screen IMGUI window
        //ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        //ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

        if (main_window)
        {
            bool p_open = true;
            // Flags for some reason having empty flags removes the collapse button, which is why I look at them!
            //ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
            ImGui::Begin("Portmaster GUI Test",&p_open, window_flags);
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

// Demonstrate create a window with multiple child windows.
{
     bool p_open = true;
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Example: Simple layout", &p_open, ImGuiWindowFlags_MenuBar))
    {
//        IMGUI_DEMO_MARKER("Examples/Simple layout");
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Close", "Ctrl+W")) { p_open = false; }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Left
        static int selected = 0;
        {
           ImGui::BeginChild("left pane", ImVec2(150, 0), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX);
           std::string texts[3] = {"Game Instructions", "View Read Me", "Visit Author"};

//            for (int i : texts)
           for (int i = 0; i < 3; i++)
            {
                // FIXME: Good candidate to use ImGuiSelectableFlags_SelectOnNav
                char label[128];
               sprintf(label, texts[i].c_str());
                //sprintf(label, "TEst");
                if (ImGui::Selectable(label, selected == i))
                    selected = i;
            }
            ImGui::EndChild();
        }
        ImGui::SameLine();

        // Right
        {
            ImGui::BeginGroup();
            ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
            ImGui::Text("MyObject: %d", selected);
            ImGui::Separator();
            if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
            {
                if (ImGui::BeginTabItem("Description"))
                {
                    ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Details"))
                {
                    ImGui::Text("ID: 0123456789");

                   

                    ImGui::Text("pointer = %p", my_texture);
                    ImGui::Text("size = %d x %d", my_image_width, my_image_height);
                    ImGui::Image((void*) my_texture, ImVec2(my_image_width, my_image_height));
                    

                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            ImGui::EndChild();
            if (ImGui::Button("Revert")) {}
            ImGui::SameLine();
            if (ImGui::Button("Save")) {}
            ImGui::EndGroup();
        }
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

     SDL_DestroyTexture(texture);
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}