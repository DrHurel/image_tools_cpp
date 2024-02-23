#include "image.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void ImagePGM::_lire_image_pgm(std::string path) {

  std ::ifstream file(path, std::ios::binary);
  int max_grey_val;

  if (!file.is_open()) {
    std::cerr << "\nPas d'acces en lecture sur l'image " << path << std::endl;
    exit(EXIT_FAILURE);
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

ImagePGM::ImagePGM(std::string filename) : Image() {
  _lire_image_pgm(filename);
}

void ImagePGM::write(std::string filename) {
  std::ofstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "\nPas d'acces en ecriture sur l'image " << filename
              << std::endl;
    exit(EXIT_FAILURE);
  } else {
    file << format << "\r";
    file << width << " " << height << "\r255\r";
    file.write(data.data(), data.size());
    file.close();
  }
}

unsigned char ImagePGM::getPixel(int x, int y) const {
  return data.at(y * width + x);
}
void ImagePGM::setPixel(int x, int y, unsigned char value) {
  data.at(y * width + x) = value;
}

void ImagePGM::writeProfil(std::string filename, int target, bool isLine) {

  std::ofstream file(filename, std::ios::out | std::ios::trunc);
  if (!file.is_open()) {
    std::cerr << "\nPas d'acces en ecriture sur le fichier " << filename
              << std::endl;
    exit(EXIT_FAILURE);
  } else {
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
}
