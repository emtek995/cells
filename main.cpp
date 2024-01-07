#include <cstdint>
#include <cstdlib>
#include <sys/mman.h>

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
SDL_Texture *texture = nullptr;
uint32_t *buffer = nullptr;

constexpr int window_width = 800;
constexpr int window_height = 600;
constexpr int bytes_per_pixel = 4;
constexpr int buffer_size = window_width * window_height * bytes_per_pixel;

int SDL_AppInit(int, char **) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
    SDL_Log("SDL_Init failed (%s)", SDL_GetError());
    exit(1);
  }

  if (SDL_CreateWindowAndRenderer(window_width, window_height, 0, &window, &renderer) < 0) {
    SDL_Log("SDL_CreateWindowAndRenderer failed (%s)", SDL_GetError());
    SDL_Quit();
    exit(1);
  }
  SDL_SetWindowTitle(window, "derp");

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, window_width, window_height);
  buffer = (uint32_t *) mmap(nullptr, buffer_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, 0, 0);
  return 0;
}

int SDL_AppIterate() {


  SDL_UpdateTexture(texture, nullptr, buffer, window_width * bytes_per_pixel);
  SDL_RenderTexture(renderer, texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);

  return 0;
}

int SDL_AppEvent(const SDL_Event *event) {
  if (event->type == SDL_EVENT_QUIT) {
    return 1;
  }
  if (event->key.keysym.sym == SDL_KeyCode::SDLK_ESCAPE) {
    return 1;
  }

  return 0;
}

void SDL_AppQuit() {
  munmap(buffer, buffer_size);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
