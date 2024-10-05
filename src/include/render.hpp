#pragma once

#include <cstdint>

struct Buffer {
  uint32_t *pixels;
  unsigned size;
  unsigned width;
  unsigned height;
};

struct Vertex {
  unsigned x;
  unsigned y;
};

struct Size {
  unsigned width;
  unsigned height;
};

auto game_render(Buffer *buffer) -> void;
auto draw_pixel(const Buffer *buffer, const int scale, const uint32_t color, const int x, const int y) -> void;
