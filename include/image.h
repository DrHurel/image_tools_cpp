
#ifndef IMAGE_H
#define IMAGE_H

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
};

class ImagePPM : public Image {
private:
  std::string format = "P6";

  void _lire_image_ppm(std::string path);

public:
  ImagePPM(std::string filename);
  void write(std::string filename);

  int getWidth() const { return width; }
  int getHeight() const { return height; }

  PixelRGB getPixel(int x, int y) const;
  void setPixel(int x, int y, char value);
  void writeProfil(std::string filename, int target, bool isLine);
};

class ImagePGM : public Image {
private:
  std::string format = "P5";

  void _lire_image_pgm(std::string path);

public:
  ImagePGM(std::string filename);
  void write(std::string filename);

  int getWidth() const { return width; }
  int getHeight() const { return height; }

  unsigned char getPixel(int x, int y) const;
  void setPixel(int x, int y, unsigned char value);

  void writeProfil(std::string filename, int target, bool isLine);
};

#endif