#include <render.hpp>

auto game_render(uint32_t *buffer, uint32_t buffer_width, uint32_t buffer_height) -> void {
  static auto i = 0;
  auto row = buffer;
  for (auto y = 0; y < buffer_height; ++y) {
    auto pixel = row;
    for (auto x = 0; x < buffer_width; ++x) {
      uint8_t blue = x + i;
      uint8_t red = y;

      // RGBA pixel format
      *pixel++ = (red << 24) | (blue << 8);
    }
    row += buffer_width;
  }
  i++;
}
