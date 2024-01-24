#include <render.hpp>

class CellMap {
  private:
  unsigned char *cells;
  unsigned char *temp_cells;
  unsigned int width;
  unsigned int height;
  unsigned int length_in_bytes;

  public:
  CellMap(unsigned int h, unsigned int v);
  ~CellMap(void);
  void set_cell(unsigned int x, unsigned int y);
  void clear_cell(unsigned int x, unsigned int y);
  int cell_state(int x, int y);
  void next_generation(Buffer *buffer);
  void init(void);
};

constexpr unsigned int cellmap_width = 480;
constexpr unsigned int cellmap_height = 270;
constexpr unsigned int scale = 4;
