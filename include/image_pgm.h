
#ifndef IMAGEPGM_H
#define IMAGEPGM_H

#include "image.h"
#include <string>

class ImagePGM : public Image {
private:
  std::string format = "P5";

  void _lire_image_pgm(std::string path);

public:
  ImagePGM(std::string filename);
  ImagePGM(int width, int height);

  void write(std::string filename);
  void threshold(int value);

  unsigned char getPixel(int x, int y) const;
  void setPixel(int x, int y, unsigned char value);

  void histogram(std::string filename);
  void writeProfil(std::string filename, int target, bool isLine);

  ImagePGM mapGradient() const;

  template <typename T> ImagePGM blur(T b) {
    auto res = ImagePGM(getWidth(), getHeight());
    auto sum = 0;
    int count = 0;
    for (int x = 0; x < getWidth(); x++) {
      for (int y = 0; y < getHeight(); y++) {
        for (int i = -b.getSizeX(); i < b.getSizeX(); i++) {
          for (int j = -b.getSizeY(); j < b.getSizeY(); j++) {
            if (b.isInScope(x, y, x + i, y + j, getWidth(), getHeight())) {
              sum += getPixel(x + i, y + j);
              count++;
            }
          }
        }
        if (count == 0) {
          res.setPixel(x, y, getPixel(x, y));

        } else {
          res.setPixel(x, y, sum / count);
        }

        sum = 0;
        count = 0;
      }
    }

    return res;
  };
};

#endif