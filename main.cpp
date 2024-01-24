#include <cstdint>
#include <cstdlib>
#include <format>
#include <sys/mman.h>

#include <SDL3/SDL.h>

#include <cellmap.hpp>
#include <render.hpp>


constexpr int window_width = 1920;
constexpr int window_height = 1080;
constexpr int bytes_per_pixel = 4;


int main(int, char **) {
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;

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

  SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, window_width, window_height);
  unsigned size = window_width * window_height * bytes_per_pixel;
  Buffer buffer{
          .pixels = (uint32_t *) mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, 0, 0),
          .size = size,
          .width = window_width,
          .height = window_height,
  };

  bool done = false;
  SDL_Event event;
  Uint64 start_time = SDL_GetPerformanceCounter();
  Uint64 end_time{};
  double delta_time{};
  auto timer_frequecy = SDL_GetPerformanceFrequency();

  setup();


  CellMap current_map(cellmap_height, cellmap_width);
  current_map.init();
  int generation{};

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

    generation++;
    current_map.next_generation(&buffer);

    SDL_UpdateTexture(texture, nullptr, buffer.pixels, window_width * bytes_per_pixel);
    SDL_RenderTexture(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);

    auto title = std::format("Gen: {} - Frame time: {:06.2f}μs - FPS: {:06.2f}", generation, delta_time * 1000, (60.0 - delta_time));
    SDL_SetWindowTitle(window, title.c_str());
    SDL_Delay(1000.0 / (60.0 - delta_time));
  }

  munmap(buffer.pixels, buffer.size);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
