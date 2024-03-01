#include "image_ppm.h"
#include "image.h"
#include "utils.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void ImagePPM::_lire_image_ppm(std::string path) {

  std::ifstream file(path, std::ios::binary);

  int max_grey_val;

  if (!file.is_open()) {
    std::cerr << "\nPas d'acces en lecture sur l'image " << path << std::endl;
    exit(EXIT_FAILURE);
  }

  file >> format;
  ignore_comments(file);
  file >> width >> height >> max_grey_val >> std::ws;
  int size = 3 * width * height;
  data.resize(size);
  file.read(data.data(), size);
  if (file.gcount() != size) {
    std::cerr << "\nErreur de lecture de l'image " << path << std::endl;
    exit(EXIT_FAILURE);
  }
  file.close();
}

ImagePPM::ImagePPM(std::string filename) : Image() {
  _lire_image_ppm(filename);
}

ImagePPM::ImagePPM(int w, int h) : Image(w, h) {
  format = "P6";
  data.resize(3 * width * height);
}

void ImagePPM::write(std::string filename) const {
  std::ofstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    exit_failure_file(filename);
  } else {
    file << format << "\r";
    file << width << " " << height << "\r255\r";
    file.write(data.data(), 3 * width * height);
    file.close();
  }
}

PixelRGB ImagePPM::getPixel(int x, int y) const {
  if (x < 0 || x >= width || y < 0 || y >= height) {
    return {255, 255, 255};
  }

  return {static_cast<unsigned char>(data.at(3 * (y * width + x))),
          static_cast<unsigned char>(data.at(3 * (y * width + x) + 1)),
          static_cast<unsigned char>(data.at(3 * (y * width + x) + 2))};
}
void ImagePPM::setPixel(int x, int y, PixelRGB value) {
  data.at(3 * (y * width + x)) = value.r;
  data.at(3 * (y * width + x) + 1) = value.g;
  data.at(3 * (y * width + x) + 2) = value.b;
}

void ImagePPM::writeProfil(std::string filename, int target,
                           bool isLine) const {

  std::ofstream file(filename, std::ios::out | std::ios::trunc);
  if (!file.is_open()) {
    exit_failure_file(filename);
  } else {
    PixelRGB px;
    if (isLine) {
      for (int i = 0; i < width; i++) {
        px = getPixel(i, target);
        file << i << ' ' << px.r << ' ' << px.g << ' ' << px.b << '\r';
      }
    } else {
      for (int i = 0; i < height; i++) {
        px = getPixel(target, i);
        file << i << ' ' << px.r << ' ' << px.g << ' ' << px.b << '\r';
      }
      file.close();
    }
  }
}

void ImagePPM::histogram(std::string filename) const {
  std::ofstream file(filename, std::ios::out | std::ios::trunc);
  if (!file.is_open()) {
    exit_failure_file(filename);
  }

  std::vector<int> histo(256 * 3, 0);
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      auto px = getPixel(i, j);
      histo.at(px.r)++;
      histo.at(px.g * 2)++;
      histo.at(px.b * 3)++;
    }
  }

  for (int i = 0; i < 256; i++) {
    file << i << ' ' << histo.at(i) << ' ' << histo.at(i + 256) << ' '
         << histo.at(i + 512) << '\r';
    file.close();
  }
}

ImagePPM ImagePPM::mapGradient() const {
  auto res = ImagePPM(getWidth(), getHeight());
  for (int x = 0; x < getWidth(); x++) {
    for (int y = 0; y < getHeight(); y++) {
      auto dx = addPxl(getPixel(x - 1, y), getPixel(x + 1, y));
      auto dy = addPxl(getPixel(x, y - 1), getPixel(x, y + 1));
      PixelRGB norm = sqrtPxl(addPxl(multPxl(dx, dx), multPxl(dy, dy)));
      res.setPixel(x, y, norm);
    }
  }
  return res;
}
