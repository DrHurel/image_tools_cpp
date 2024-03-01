#include "brush.h"

bool CircleBrush::isInScope(int cx, int cy, int x, int y, int width,
                            int height) const {

  return (x - cx) * (x - cx) + (y - cy) * (y - cy) <= radius * radius &&
         x >= 0 && y >= 0 && x < width && y < height;
}