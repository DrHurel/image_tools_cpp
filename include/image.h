
#ifndef IMAGE_H
#define IMAGE_H

#include <fstream>
#include <vector>

typedef struct {
  unsigned int r;
  unsigned int g;
  unsigned int b;
} PixelRGB;

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

  void negate() {
    for (char &c : data) {
      c = 255 - c;
    }
  }
};

#endif