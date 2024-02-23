#include "brush.h"

bool Brush::isInScope(int cx, int cy, int x, int y, int width, int height) {

  return x >= 0 && y >= 0 && x < width && y < height && x == cx && y == cy;
}

int Brush::getSizeX() const { return 0; };

int Brush::getSizeY() const { return 0; };
