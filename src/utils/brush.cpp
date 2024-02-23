#include "brush.h"

bool Brush::isInScope(int cx, int cy, int x, int y, int width, int height) {

  return true;
}

int Brush::getSizeX() const { return 0; };

int Brush::getSizeY() const { return 0; };

bool CircleBrush::isInScope(int cx, int cy, int x, int y, int width,
                            int height) {

  return (x - cx) * (x - cx) + (y - cy) * (y - cy) <= radius * radius &&
         x >= 0 && y >= 0 && x < width && y < height;
}