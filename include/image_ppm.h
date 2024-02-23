
#ifndef IMAGEPPM_H
#define IMAGEPPM_H

#include "image.h"
#include <string>

class ImagePPM : public Image {
private:
  std::string format = "P6";

  void _lire_image_ppm(std::string path);

public:
  ImagePPM(std::string filename);
  ImagePPM(int width, int height);
  void write(std::string filename) const;
  void histogram(std::string filename) const;

  PixelRGB getPixel(int x, int y) const;
  void setPixel(int x, int y, PixelRGB value);
  void writeProfil(std::string filename, int target, bool isLine) const;

  template <typename T> ImagePPM blur(T b) const {
    auto res = ImagePPM(getWidth(), getHeight());
    auto sum = PixelRGB{0, 0, 0};
    int count = 0;
    for (int x = 0; x < getWidth(); x++) {
      for (int y = 0; y < getHeight(); y++) {
        for (int i = -b.getSizeX(); i < b.getSizeX(); i++) {
          for (int j = -b.getSizeY(); j < b.getSizeY(); j++) {
            if (b.isInScope(x, y, x + i, y + j, getWidth(), getHeight())) {
              auto px = getPixel(x + i, y + j);
              sum = {(sum.r + px.r), (sum.g + px.g), (sum.b + px.b)};
              count++;
            }
          }
        }
        res.setPixel(x, y,
                     {static_cast<unsigned char>(sum.r / count),
                      static_cast<unsigned char>(sum.g / count),
                      static_cast<unsigned char>(sum.b / count)});
        sum = {0, 0, 0};
        count = 0;
      }
    }

    return res;
  };
};

#endif