#ifndef UTILS_H
#define UTILS_H
#include "image.h"
#include <string>
void exit_failure_file(std::string filename);
PixelRGB addPxl(PixelRGB a, PixelRGB b);

PixelRGB multPxl(PixelRGB a, PixelRGB b);
PixelRGB sqrtPxl(PixelRGB a);
#endif // UTILS_H
