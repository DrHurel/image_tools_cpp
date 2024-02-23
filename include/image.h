
#ifndef IMAGE_H
#define IMAGE_H

#include "brush.h"
#include <fstream>

#include <string>
#include <vector>

using PixelRGB = struct {
  unsigned int r;
  unsigned int g;
  unsigned int b;
};

class Image {

protected:
  int width;
  int height;
  std::vector<char> data;
  void ignore_comments(std::ifstream &file);

public:
  Image();
  Image(int w, int h) : width(w), height(h) { data.resize(w * h); }

  int getWidth() const { return width; }
  int getHeight() const { return height; }
};

class ImagePPM : public Image {
private:
  std::string format = "P6";

  void _lire_image_ppm(std::string path);

public:
  ImagePPM(std::string filename);
  ImagePPM(int width, int height);
  void write(std::string filename);

  PixelRGB getPixel(int x, int y) const;
  void setPixel(int x, int y, PixelRGB value);
  void writeProfil(std::string filename, int target, bool isLine);
  template <typename T> ImagePPM blur(T b) {
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

class ImagePGM : public Image {
private:
  std::string format = "P5";

  void _lire_image_pgm(std::string path);

public:
  ImagePGM(std::string filename);
  ImagePGM(int width, int height);
  void write(std::string filename);

  unsigned char getPixel(int x, int y) const;
  void setPixel(int x, int y, unsigned char value);

  void writeProfil(std::string filename, int target, bool isLine);
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