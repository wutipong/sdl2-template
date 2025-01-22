#include <SDL2/SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <memory>

constexpr int WINDOW_MINIMUM_WIDTH = 1280;
constexpr int WINDOW_MINIMUM_HEIGHT = 720;
constexpr char WINDOW_TITLE[] = "SDL2 template";
constexpr auto WINDOW_FLAGS = SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    std::unique_ptr<SDL_Window, decltype([](SDL_Window *w) { SDL_DestroyWindow(w); })> window{
        SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_MINIMUM_WIDTH,
                         WINDOW_MINIMUM_HEIGHT, WINDOW_FLAGS)};

    SDL_SetWindowMinimumSize(window.get(), WINDOW_MINIMUM_WIDTH, WINDOW_MINIMUM_HEIGHT);

    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

    std::unique_ptr<SDL_Renderer, decltype([](SDL_Renderer *r) { SDL_DestroyRenderer(r); })> renderer{
        SDL_CreateRenderer(window.get(), -1, 0)};

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

        ImGui_ImplSDL2_NewFrame();
        ImGui_ImplSDLRenderer2_NewFrame();

        ImGui::NewFrame();
        {
            if (ImGui::Begin("Hello"))
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

        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer.get());
        SDL_RenderPresent(renderer.get());
        SDL_Delay(1);
    }

    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    renderer.reset();
    window.reset();
    SDL_Quit();

    return EXIT_SUCCESS;
}
