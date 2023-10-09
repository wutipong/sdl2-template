#include <SDL2/SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

#include <memory>

constexpr int WINDOW_MINIMUM_WIDTH = 800;
constexpr int WINDOW_MINIMUM_HEIGHT = 600;
constexpr char WINDOW_TITLE[] = "SDL2 template project.";
constexpr auto WINDOW_FLAGS = SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    {
        auto windowDeleter = [](SDL_Window *w) { SDL_DestroyWindow(w); };
        std::unique_ptr<SDL_Window, decltype(windowDeleter)> window{
            SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_MINIMUM_WIDTH,
                             WINDOW_MINIMUM_HEIGHT, WINDOW_FLAGS),
            windowDeleter};

        SDL_SetWindowMinimumSize(window.get(), WINDOW_MINIMUM_WIDTH, WINDOW_MINIMUM_HEIGHT);
        auto rendererDeleter = [](SDL_Renderer *r) { SDL_DestroyRenderer(r); };

        std::unique_ptr<SDL_Renderer, decltype(rendererDeleter)> renderer{SDL_CreateRenderer(window.get(), -1, 0),
                                                                          rendererDeleter};

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGui_ImplSDL2_InitForSDLRenderer(window.get(), renderer.get());
        ImGui_ImplSDLRenderer2_Init(renderer.get());

        bool is_running{true};
        while (is_running)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                ImGui_ImplSDL2_ProcessEvent(&event);
                if (event.type == SDL_QUIT)
                {
                    is_running = false;
                    break;
                }
            }

            SDL_RenderClear(renderer.get());

            ImGui_ImplSDL2_NewFrame(window.get());
            ImGui_ImplSDLRenderer2_NewFrame();

            ImGui::NewFrame();
            {
                ImGui::Begin("Hello");
                {
                    if (ImGui::Button("Hello World"))
                    {
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Hello", "Hello World", window.get());
                    }
                }
                ImGui::End();
            }

            ImGui::EndFrame();
            ImGui::Render();

            ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
            SDL_RenderPresent(renderer.get());
            SDL_Delay(1);
        }

        ImGui_ImplSDLRenderer2_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }
    SDL_Quit();

    return EXIT_SUCCESS;
}
