#include "image.h"
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

void ImagePPM::write(std::string filename) {
  std::ofstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "\nPas d'acces en ecriture sur l'image " << filename
              << std::endl;
    exit(EXIT_FAILURE);
  } else {
    file << format << "\r";
    file << width << " " << height << "\r255\r";
    file.write(data.data(), 3 * width * height);
    file.close();
  }
}

PixelRGB ImagePPM::getPixel(int x, int y) const {
  return {static_cast<unsigned char>(data.at(3 * (y * width + x))),
          static_cast<unsigned char>(data.at(3 * (y * width + x) + 1)),
          static_cast<unsigned char>(data.at(3 * (y * width + x) + 2))};
}
void ImagePPM::setPixel(int x, int y, char value) {
  data.at(3 * (y * width + x)) = value;
}

void ImagePPM::writeProfil(std::string filename, int target, bool isLine) {

  std::ofstream file(filename, std::ios::out | std::ios::trunc);
  if (!file.is_open()) {
    std::cerr << "\nPas d'acces en ecriture sur le fichier " << filename
              << std::endl;
    exit(EXIT_FAILURE);
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