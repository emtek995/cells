#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

int SDL_AppInit(int, char **) {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    SDL_Log("SDL_Init failed (%s)", SDL_GetError());
    return 1;
  }

  if (SDL_CreateWindowAndRenderer(800, 600, 0, &window, &renderer) < 0) {
    SDL_Log("SDL_CreateWindowAndRenderer failed (%s)", SDL_GetError());
    SDL_Quit();
    return 1;
  }
  SDL_SetWindowTitle(window, "derp");

  return 0;
}

int SDL_AppIterate() {
  SDL_SetRenderDrawColor(renderer, 80, 80, 80, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);

  return 0;
}

int SDL_AppEvent(const SDL_Event *event) {
  if (event->type == SDL_EVENT_QUIT) {
    return 1;
  }
  if (event->key.keysym.sym == SDL_KeyCode::SDLK_q) {
    return 1;
  }

  return 0;
}

void SDL_AppQuit() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
