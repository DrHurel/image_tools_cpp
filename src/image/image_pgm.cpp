
#include "image_pgm.h"
#include "brush.h"
#include "utils.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void ImagePGM::_lire_image_pgm(std::string path) {

  std ::ifstream file(path, std::ios::binary);
  int max_grey_val;

  if (!file.is_open()) {
    exit_failure_file(path);
  }

  file >> format;
  ignore_comments(file);
  file >> width >> height >> max_grey_val >> std::ws;
  int size = width * height;
  data.resize(size);
  file.read(data.data(), size);
  if (file.gcount() != size) {
    std::cerr << "\nErreur de lecture de l'image " << path << std::endl;
    exit(EXIT_FAILURE);
  }

  file.close();
}

ImagePGM::ImagePGM(int w, int h) : Image(w, h) { format = "P5"; }

ImagePGM::ImagePGM(std::string filename) : Image() {
  _lire_image_pgm(filename);
}

void ImagePGM::write(std::string filename) {
  std::ofstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    exit_failure_file(filename);
  }
  file << format << "\r";
  file << width << " " << height << "\r255\r";
  file.write(data.data(), data.size());
  file.close();
}

unsigned char ImagePGM::getPixel(int x, int y) const {
  if (x < 0 || x >= width || y < 0 || y >= height) {
    return 255;
  }
  return data.at(y * width + x);
}
void ImagePGM::setPixel(int x, int y, unsigned char value) {
  data.at(y * width + x) = value;
}

void ImagePGM::writeProfil(std::string filename, int target, bool isLine) {

  std::ofstream file(filename, std::ios::out | std::ios::trunc);
  if (!file.is_open()) {
    exit_failure_file(filename);
  }

  unsigned char px;
  if (isLine) {
    for (int i = 0; i < width; i++) {
      px = getPixel(i, target);
      file << i << ' ' << (int)px << '\r';
    }
  } else {
    for (int i = 0; i < height; i++) {
      px = getPixel(target, i);
      file << i << ' ' << (int)px << '\r';
    }
    file.close();
  }
}

void ImagePGM::histogram(std::string filename) {
  std::ofstream file(filename, std::ios::out | std::ios::trunc);
  if (!file.is_open()) {
    exit_failure_file(filename);
  }
  std::vector<int> histo(256, 0);
  for (int i = 0; i < width * height; i++) {
    histo.at(data.at(i))++;
  }
  for (int i = 0; i < 256; i++) {
    file << i << ' ' << histo.at(i) << '\r';
  }
  file.close();
}

void ImagePGM::threshold(int value) {
  for (unsigned char px : data) {
    if (px < value) {
      px = 0;
    } else {
      px = 255;
    }
  }
}

ImagePGM ImagePGM::mapGradient() const {

  ImagePGM res(width, height);
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {

      int dx = getPixel(x - 1, y) - getPixel(x + 1, y);
      int dy = getPixel(x, y - 1) - getPixel(x, y + 1);
      res.setPixel(x, y, std::sqrt(dx * dx + dy * dy));
    }
  }
  return res;
}

ImagePGM ImagePGM::threshold_gradient(int value) const {
  ImagePGM res(width, height);
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {

      int dx = getPixel(x - 1, y) - getPixel(x + 1, y);
      int dy = getPixel(x, y - 1) - getPixel(x, y + 1);
      res.setPixel(x, y, std::sqrt(dx * dx + dy * dy) > value ? 255 : 0);
    }
  }
  return res;
}

ImagePGM ImagePGM::threshold_gradient_hys(int threshold_down, int threshold_up,
                                          int radius) const {

  ImagePGM res(width, height);
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {

      int dx = getPixel(x - 1, y) - getPixel(x + 1, y);
      int dy = getPixel(x, y - 1) - getPixel(x, y + 1);
      int grad = std::sqrt(dx * dx + dy * dy);
      if (grad > threshold_up) {
        res.setPixel(x, y, 255);
      } else if (grad < threshold_down) {
        res.setPixel(x, y, 0);
      } else {
        bool isMax = true;
        for (int i = -radius; i <= radius; i++) {
          for (int j = -radius; j <= radius; j++) {
            if (getPixel(x + i, y + j) > grad) {
              isMax = false;
              break;
            }
          }
        }
        res.setPixel(x, y, isMax ? 255 : 0);
      }
    }
  }
  return res;
}

ImagePGM erode(ImagePGM in, const Brush &brush) {
  ImagePGM res(in.getWidth(), in.getHeight());
  for (int x = 0; x < in.getWidth(); x++) {
    for (int y = 0; y < in.getHeight(); y++) {
      unsigned char max = 0;
      for (int i = -brush.getSizeX(); i <= brush.getSizeX(); i++) {
        for (int j = -brush.getSizeY(); j <= brush.getSizeY(); j++) {
          if (brush.isInScope(x, y, x + i, y + j, in.getWidth(),
                              in.getHeight())) {
            max = std::max(max, in.getPixel(x + i, y + j));
          }
        }
      }
      res.setPixel(x, y, max);
    }
  }
  return res;
}

ImagePGM dilate(ImagePGM in, const Brush &brush) {
  ImagePGM res(in.getWidth(), in.getHeight());
  for (int x = 0; x < in.getWidth(); x++) {
    for (int y = 0; y < in.getHeight(); y++) {
      unsigned char min = 255;
      for (int i = -brush.getSizeX(); i <= brush.getSizeX(); i++) {
        for (int j = -brush.getSizeY(); j <= brush.getSizeY(); j++) {
          if (brush.isInScope(x, y, x + i, y + j, in.getWidth(),
                              in.getHeight())) {
            min = std::min(min, in.getPixel(x + i, y + j));
          }
        }
      }
      res.setPixel(x, y, min);
    }
  }
  return res;
}

ImagePGM ImagePGM::gaussien_filter() {
  ImagePGM res(getWidth(), getHeight());
  for (int x = 0; x < getWidth(); x++) {
    for (int y = 0; y < getHeight(); y++) {
      int sum = 0;
      int count = 0;
      for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
          if (x + i >= 0 && x + i < getWidth() && y + j >= 0 &&
              y + j < getHeight()) {
            if (i == 0 && j == 0) {
              sum += getPixel(x + i, y + j) * 4;
              count += 4;
            } else if (i == 0 || j == 0) {
              sum += getPixel(x + i, y + j) * 2;
              count += 2;
            } else {
              sum += getPixel(x + i, y + j);
              count++;
            }
          }
        }
      }
      res.setPixel(x, y, sum / count);
    }
  }
  return res;
}