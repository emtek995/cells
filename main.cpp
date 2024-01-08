#include <cstdint>
#include <format>
#include <sys/mman.h>

#include <SDL3/SDL.h>

#include <render.hpp>

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
SDL_Texture *texture = nullptr;
uint32_t *buffer = nullptr;

constexpr int window_width = 800;
constexpr int window_height = 600;
constexpr int bytes_per_pixel = 4;
constexpr int buffer_size = window_width * window_height * bytes_per_pixel;

int main(int, char **) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
    SDL_Log("SDL_Init failed (%s)", SDL_GetError());
    return 1;
  }

  if (SDL_CreateWindowAndRenderer(window_width, window_height, 0, &window, &renderer) < 0) {
    SDL_Log("SDL_CreateWindowAndRenderer failed (%s)", SDL_GetError());
    SDL_Quit();
    return 1;
  }
  SDL_SetWindowTitle(window, "derp");

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, window_width, window_height);
  buffer = (uint32_t *) mmap(nullptr, buffer_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, 0, 0);

  bool done = false;
  SDL_Event event;
  Uint64 start_time = SDL_GetPerformanceCounter();
  Uint64 end_time{};
  double delta_time{};
  auto timer_frequecy = SDL_GetPerformanceFrequency();

  while (!done) {
    end_time = start_time;
    start_time = SDL_GetPerformanceCounter();
    delta_time = double(start_time - end_time) / timer_frequecy;

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        done = true;
      }
      if (event.key.keysym.sym == SDL_KeyCode::SDLK_ESCAPE) {
        done = true;
      }
    }
    game_render(buffer, window_width, window_height);

    SDL_UpdateTexture(texture, nullptr, buffer, window_width * bytes_per_pixel);
    SDL_RenderTexture(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);

    auto title = std::format("Frame time: {:03.4f}ms FPS: {:03.4f}", delta_time, (60.0 - delta_time));
    SDL_SetWindowTitle(window, title.c_str());
    SDL_Delay(1000.0 / (60.0 - delta_time));
  }

  munmap(buffer, buffer_size);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
