#include "image.h"
#include "utils.h"
#include <cmath>
PixelRGB addPxl(PixelRGB a, PixelRGB b) {
  return {(a.r + b.r), (a.g + b.g), (a.b + b.b)};
}

PixelRGB multPxl(PixelRGB a, PixelRGB b) {
  return {(a.r * b.r), (a.g * b.g), (a.b * b.b)};
}

PixelRGB sqrtPxl(PixelRGB a) {
  return {static_cast<int>((std::sqrt(a.r))),
          static_cast<int>((std::sqrt(a.g))),
          static_cast<int>((std::sqrt(a.b)))};
}
