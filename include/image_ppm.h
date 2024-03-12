
#ifndef IMAGEPPM_H
#define IMAGEPPM_H

#include "brush.h"
#include "image.h"
#include "image_pgm.h"
#include <array>
#include <string>

class Image3 : public Image {
private:
  std::string format = "P6";

  void _lire_image_ppm(std::string path);

public:
  Image3(std::string filename);
  Image3(int width, int height);
  Image3(ImagePGM y, ImagePGM Cb, ImagePGM Cr, bool isRGB);
  Image3(ImagePGM y, ImagePGM Cb, ImagePGM Cr, short k);

  void write(std::string filename) const;
  void histogram(std::string filename) const;

  PixelRGB getPixel(int x, int y) const;
  void setPixel(int x, int y, PixelRGB value);
  void writeProfil(std::string filename, int target, bool isLine) const;

  Image3 mapGradient() const;
  ImagePGM toPGM() const;
  std::array<ImagePGM, 3> toYCbCr() const;

  template <typename T> Image3 blur(T b) const {
    auto res = Image3(getWidth(), getHeight());
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

  Image3 blurRef(Brush &b) const;
  std::array<ImagePGM, 3> toHLS() const;
};

#endif