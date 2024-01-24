#include <cstdlib>
#include <cstring>

#include <SDL3/SDL.h>

#include <cellmap.hpp>

CellMap::CellMap(unsigned int h, unsigned int w) {
  width = w;
  height = h;
  length_in_bytes = w * h;
  cells = new unsigned char[length_in_bytes];
  temp_cells = new unsigned char[length_in_bytes];
  memset(cells, 0, length_in_bytes);
}

CellMap::~CellMap(void) {
  delete[] temp_cells;
  delete[] cells;
}

void CellMap::set_cell(unsigned int x, unsigned int y) {
  int xoleft, xoright, yoabove, yobelow;
  unsigned char *cell_ptr = cells + (y * width) + x;
  if (x == 0)
    xoleft = width - 1;
  else
    xoleft = -1;
  if (y == 0)
    yoabove = length_in_bytes - width;
  else
    yoabove = -width;
  if (x == (width - 1))
    xoright = -(width - 1);
  else
    xoright = 1;
  if (y == (height - 1))
    yobelow = -(length_in_bytes - width);
  else
    yobelow = width;
  *(cell_ptr) |= 0x01;
  *(cell_ptr + yoabove + xoleft) += 2;
  *(cell_ptr + yoabove) += 2;
  *(cell_ptr + yoabove + xoright) += 2;
  *(cell_ptr + xoleft) += 2;
  *(cell_ptr + xoright) += 2;
  *(cell_ptr + yobelow + xoleft) += 2;
  *(cell_ptr + yobelow) += 2;
  *(cell_ptr + yobelow + xoright) += 2;
}

void CellMap::clear_cell(unsigned int x, unsigned int y) {
  int xoleft, xoright, yoabove, yobelow;
  unsigned char *cell_ptr = cells + (y * width) + x;
  if (x == 0)
    xoleft = width - 1;
  else
    xoleft = -1;
  if (y == 0)
    yoabove = length_in_bytes - width;
  else
    yoabove = -width;
  if (x == (width - 1))
    xoright = -(width - 1);
  else
    xoright = 1;
  if (y == (height - 1))
    yobelow = -(length_in_bytes - width);
  else
    yobelow = width;
  *(cell_ptr) &= ~0x01;
  *(cell_ptr + yoabove + xoleft) -= 2;
  *(cell_ptr + yoabove) -= 2;
  *(cell_ptr + yoabove + xoright) -= 2;
  *(cell_ptr + xoleft) -= 2;
  *(cell_ptr + xoright) -= 2;
  *(cell_ptr + yobelow + xoleft) -= 2;
  *(cell_ptr + yobelow) -= 2;
  *(cell_ptr + yobelow + xoright) -= 2;
}

int CellMap::cell_state(int x, int y) {
  unsigned char *cell_ptr = cells + (y * width) + x;
  return *cell_ptr & 0x01;
}

void CellMap::init(void) {
  int seed = SDL_GetPerformanceCounter();
  srand(seed);
  int init_length = (cellmap_height * cellmap_width) / 2;
  do {
    int x = std::rand() % cellmap_width;
    int y = std::rand() % cellmap_height;
    if (cell_state(x, y) == 0) {
      set_cell(x, y);
    }
  } while (--init_length);
}

void CellMap::next_generation(Buffer *buffer) {
  memcpy(temp_cells, cells, length_in_bytes);
  unsigned char *cell_ptr = temp_cells;
  for (int y = 0; y < height; ++y) {
    int x = 0;
    do {
      while (*cell_ptr == 0) {
        cell_ptr++;
        if (++x >= width) goto RowDone;
      }
      int count = *cell_ptr >> 1;
      if (*cell_ptr & 0x01) {
        if ((count != 2) && (count != 3)) {
          clear_cell(x, y);
          draw_pixel(buffer, scale, 0x00000000, x, y);
        }
      } else {
        if (count == 3) {
          set_cell(x, y);
          draw_pixel(buffer, scale, 0xFFFFFFFF, x, y);
        }
      }
      cell_ptr++;
    } while (++x < width);
  RowDone:
  }
}
