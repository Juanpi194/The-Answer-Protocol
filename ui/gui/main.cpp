#define SDL_MAIN_HANDLED // avoid depending on SDL2main / WinMain on Windows
#include <SDL.h>
#include <SDL_opengl.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include <string>
#include <vector>
#include <cstdio>

#include "ui/CLI.hpp"

enum class AppState
{
    CONNECT,
    GAME
};

static void logMsg(std::vector<std::string>& log, const std::string& msg)
{
    log.push_back(msg);
    if (log.size() > 200)
        log.erase(log.begin());
}

// ---------------------------------------------------------------------------
// MODIFIED: ya no hay formulario de usuario/contraseña ni selector
// Local/Remote (el modo Local ha desaparecido, y no vamos a gestionar
// cuentas). En su lugar, un unico campo de texto: el jugador escribe algo
// como "nc 127.0.0.1 8080" -- igual que abrir una conexion con netcat de
// verdad -- y eso es lo unico que se interpreta en el propio cliente
// (CLI::parseNcCommand). Todo lo que venga despues (CONNECT <nombre>,
// MOVE, etc.) son comandos normales que ya interpreta el servidor, no algo
// que gestionemos aqui.
// ---------------------------------------------------------------------------
static void drawConnectScreen(CLI& client, AppState& state, std::string& errorMsg)
{
    static char inputBuf[128] = "";

    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f),
                             ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(420, 140), ImGuiCond_FirstUseEver);
    ImGui::Begin("TAP", nullptr, ImGuiWindowFlags_NoCollapse);

    ImGui::TextWrapped("Type a connection command to start, e.g.:");
    ImGui::TextColored(ImVec4(0.6f, 0.9f, 1.0f, 1.0f), "  nc 127.0.0.1 8080");

    ImGui::Separator();
    bool submitted = ImGui::InputText("##ConnectInput", inputBuf, IM_ARRAYSIZE(inputBuf),
                                       ImGuiInputTextFlags_EnterReturnsTrue);
    ImGui::SameLine();
    bool pressed = ImGui::Button("Go");

    if (submitted || pressed)
    {
        errorMsg.clear();
        std::string host;
        int port = 0;

        if (!CLI::parseNcCommand(inputBuf, host, port))
        {
            errorMsg = "Not a valid connection command. Try: nc 127.0.0.1 8080";
        }
        else if (client.connect(host, port))
        {
            state = AppState::GAME;
        }
        else
        {
            errorMsg = "Could not connect to " + host + ":" + std::to_string(port);
        }
    }

    if (!errorMsg.empty())
    {
        ImGui::Spacing();
        ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "%s", errorMsg.c_str());
    }

    ImGui::End();
}

// ---------------------------------------------------------------------------
// No Map/Room/Stats windows -- those read from a local Player that no
// longer exists (no Local mode). Only the Menu, with Actions (buttons +
// chat) and Inventory/Stats as placeholders until the server sends back
// structured state (STATUS, INVENTORY...).
// ---------------------------------------------------------------------------
static void drawMenuWindow(CLI& client, std::vector<std::string>& log)
{
    ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(700, 500), ImGuiCond_FirstUseEver);
    ImGui::Begin("Menu");

    if (ImGui::BeginTabBar("MenuTabs"))
    {
        if (ImGui::BeginTabItem("Actions"))
        {
            ImGui::Text("Move:");
            const char* dirs[] = { "NORTH", "SOUTH", "EAST", "WEST" };
            for (const char* dir : dirs)
            {
                if (ImGui::Button(dir))
                {
                    std::string cmd = std::string("MOVE ") + dir;
                    client.sendCommand(cmd);
                    logMsg(log, "> " + cmd);
                }
                ImGui::SameLine();
            }
            ImGui::NewLine();

            ImGui::Separator();
            if (ImGui::Button("Look"))
            {
                client.sendCommand("LOOK");
                logMsg(log, "> LOOK");
            }
            ImGui::NewLine();

            ImGui::Separator();
            ImGui::Text("Log:");
            ImGui::BeginChild("LogScroll", ImVec2(0, 250), true);
            for (const std::string& line : log)
                ImGui::TextUnformatted(line.c_str());
            ImGui::SetScrollHereY(1.0f);
            ImGui::EndChild();

            // Free-text command input -- CONNECT <name> and anything else
            // goes here, same idea as typing after `nc host port`.
            static char commandBuf[256] = "";
            ImGui::PushItemWidth(-70);
            bool submitted = ImGui::InputText("##CommandInput", commandBuf, IM_ARRAYSIZE(commandBuf),
                                               ImGuiInputTextFlags_EnterReturnsTrue);
            ImGui::PopItemWidth();
            ImGui::SameLine();
            bool sendPressed = ImGui::Button("Send");
            if ((submitted || sendPressed) && commandBuf[0] != '\0')
            {
                client.sendCommand(commandBuf);
                logMsg(log, std::string("> ") + commandBuf);
                commandBuf[0] = '\0';
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Inventory"))
        {
            ImGui::TextDisabled("Not available yet -- the server does not send "
                                 "structured inventory state. Try \"INVENTORY\" "
                                 "in the chat and watch the log.");
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Stats"))
        {
            ImGui::TextDisabled("Not available yet -- the server does not send "
                                 "structured stats state. Try \"STATUS\" in the "
                                 "chat and watch the log.");
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Settings"))
        {
            // Intentionally empty for now.
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

int main(int, char**)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        std::fprintf(stderr, "Error initializing SDL2: %s\n", SDL_GetError());
        return 1;
    }

    const char* glslVersion = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow(
        "TAP - GUI client",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1200, 700, windowFlags);
    if (!window)
    {
        std::fprintf(stderr, "Error creating the SDL2 window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glContext);
    SDL_GL_SetSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui::GetIO().FontGlobalScale = 1.5f;
    ImGui::GetStyle().ScaleAllSizes(1.5f);

    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init(glslVersion);

    CLI                         client;
    AppState                    state = AppState::CONNECT;
    std::string                 connectError;
    std::vector<std::string>    log;
    bool                        running = true;

    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                running = false;
            if (event.type == SDL_WINDOWEVENT
                && event.window.event == SDL_WINDOWEVENT_CLOSE
                && event.window.windowID == SDL_GetWindowID(window))
                running = false;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        if (state == AppState::CONNECT)
        {
            drawConnectScreen(client, state, connectError);
        }
        else
        {
            for (const std::string& msg : client.pollMessages())
                logMsg(log, msg);

            drawMenuWindow(client, log);
        }

        ImGui::Render();
        int w, h;
        SDL_GetWindowSize(window, &w, &h);
        glViewport(0, 0, w, h);
        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}