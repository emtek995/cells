#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <print>
#include <render.hpp>
#include <utility>

static inline auto draw_pixel(const Buffer *buffer, const uint32_t color, const Vertex &point) -> void {
  if (point.x >= buffer->width || point.y >= buffer->height) return;
  buffer->pixels[point.y * buffer->width + point.x] = color;
}

auto draw_pixel(const Buffer *buffer, const int scale, const uint32_t color, const int x, const int y) -> void {
  uint32_t *pixel = buffer->pixels + (y * scale * buffer->width + x * scale);
  for (int i = 0; i < scale; ++i) {
    for (int j = 0; j < scale; ++j) {
      *(pixel + j) = color;
    }
    pixel += buffer->width;
  }
}

static auto draw_line(const Buffer *buffer, const uint32_t color, const Vertex &point1, const Vertex &point2) -> void {
  int x1 = point1.x;
  int y1 = point1.y;
  int x2 = point2.x;
  int y2 = point2.y;
  bool steep = false;
  if (std::abs(x1 - x2) < std::abs(y1 - y2)) {
    std::swap(x1, y1);
    std::swap(x2, y2);
    steep = true;
  }
  if (x1 > x2) {
    std::swap(x1, x2);
    std::swap(y1, y2);
  }
  int dx = x2 - x1;
  int dy = y2 - y1;
  int derror2 = std::abs(dy) * 2;
  int error2 = 0;
  uint32_t y = y1;
  for (uint32_t x = x1; x <= x2; ++x) {
    if (steep) {
      draw_pixel(buffer, color, Vertex{.x = y, .y = x});
    } else {
      draw_pixel(buffer, color, Vertex{.x = x, .y = y});
    }
    error2 += derror2;
    if (error2 > dx) {
      y += (y2 > y1 ? 1 : -1);
      error2 -= dx * 2;
    }
  }
}

static auto draw_rect(const Buffer *buffer, const uint32_t color, const Vertex &point, const Size &size) -> void {
  if (point.x >= buffer->width || point.y >= buffer->height) return;
  for (auto row = point.x; row < point.x + size.width; ++row) {
    for (auto col = point.y; col < point.y + size.height; ++col) {
      if (point.x >= buffer->width || point.y >= buffer->height) return;
      buffer->pixels[col * buffer->width + row] = color;
    }
  }
}

static inline auto clear_buffer(const Buffer *buffer) -> void {
  std::memset(buffer->pixels, 0, buffer->size);
}

auto game_render(Buffer *buffer) -> void {
  static uint32_t x = 0;

  Vertex point1{.x = 100, .y = 100};
  Vertex point2{.x = x, .y = 200};

  ++x;
  clear_buffer(buffer);
  draw_line(buffer, 0xFFFFFFFF, point1, point2);
}
