#include "image_ppm.h"
#include "image.h"
#include "image_pgm.h"
#include "utils.h"
#include <array>
#include <cmath>
#include <exception>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

void Image3::_lire_image_ppm(std::string path) {

  std::ifstream file(path, std::ios::binary);

  int max_grey_val;

  if (!file.is_open()) {
    std::cerr << "\nPas d'acces en lecture sur l'image " << path << std::endl;
    exit(EXIT_FAILURE);
  }

  file >> format;

  ignore_comments(file);
  file >> width >> height >> max_grey_val >> std::ws;
  std::cout << format << " " << width << " " << height << std::endl;
  int size = 3 * width * height;
  data.resize(size);
  file.read(data.data(), size);
  if (file.gcount() != size) {
    std::cerr << "\nErreur de lecture de l'image " << path << std::endl;
    exit(EXIT_FAILURE);
  }
  file.close();
}

Image3::Image3(std::string filename) : Image() { _lire_image_ppm(filename); }

Image3::Image3(int w, int h) : Image(w, h) {
  format = "P6";
  data.resize(3 * width * height);
}

Image3::Image3(ImagePGM Y, ImagePGM Cb, ImagePGM Cr, bool isRGB)
    : Image(Y.getWidth(), Y.getHeight()) {
  format = "P6";
  data.resize(3 * width * height);
  for (int x = 0; x < getWidth(); x++) {
    for (int y = 0; y < getHeight(); y++) {
      auto Ypx = Y.getPixel(x, y);
      auto Cbpx = Cb.getPixel(x, y);
      auto Crpx = Cr.getPixel(x, y);
      int R = Ypx + 1.402 * (Crpx - 128);
      int G = Ypx - 0.34414 * (Cbpx - 128) - 0.714414 * (Crpx - 128);
      int B = Ypx + 1.772 * (Cbpx - 128);

      if (isRGB) {
        setPixel(x, y,
                 {(R > 0 ? (R < 256 ? R : 255) : 0),
                  (G > 0 ? (G < 256 ? G : 255) : 0),
                  (B > 0 ? (B < 256 ? B : 255) : 0)});
      } else {
        setPixel(x, y,
                 {(R > 0 ? (R < 256 ? R : 255) : 0),
                  (B > 0 ? (B < 256 ? B : 255) : 0),
                  (G > 0 ? (G < 256 ? G : 255) : 0)});
      }
    }
  }
}

Image3::Image3(ImagePGM Y, ImagePGM Cb, ImagePGM Cr, short k)
    : Image(Y.getWidth(), Y.getHeight()) {
  format = "P6";
  data.resize(3 * width * height);
  if (k < -128 || k > 128) {
    throw std::exception();
  }

  for (int x = 0; x < getWidth(); x++) {
    for (int y = 0; y < getHeight(); y++) {
      int n = (Y.getPixel(x, y) + k);
      auto Ypx = n < 0 ? 0 : n > 255 ? 255 : n;
      auto Cbpx = Cb.getPixel(x, y);
      auto Crpx = Cr.getPixel(x, y);
      int R = Ypx + 1.402 * (Crpx - 128);
      int G = Ypx - 0.34414 * (Cbpx - 128) - 0.714414 * (Crpx - 128);
      int B = Ypx + 1.772 * (Cbpx - 128);

      setPixel(x, y,
               {(R > 0 ? (R < 256 ? R : 255) : 0),
                (G > 0 ? (G < 256 ? G : 255) : 0),
                (B > 0 ? (B < 256 ? B : 255) : 0)});
    }
  }
}

void Image3::write(std::string filename) const {
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

PixelRGB Image3::getPixel(int x, int y) const {
  if (x < 0 || x >= width || y < 0 || y >= height) {
    return {255, 255, 255};
  }

  return {static_cast<unsigned char>(data.at(3 * (y * width + x))),
          static_cast<unsigned char>(data.at(3 * (y * width + x) + 1)),
          static_cast<unsigned char>(data.at(3 * (y * width + x) + 2))};
}
void Image3::setPixel(int x, int y, PixelRGB value) {
  data.at(3 * (y * width + x)) = value.r;
  data.at(3 * (y * width + x) + 1) = value.g;
  data.at(3 * (y * width + x) + 2) = value.b;
}

void Image3::writeProfil(std::string filename, int target, bool isLine) const {

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

void Image3::histogram(std::string filename) const {
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

Image3 Image3::mapGradient() const {
  auto res = Image3(getWidth(), getHeight());
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

ImagePGM Image3::toPGM() const {
  ImagePGM res = ImagePGM(getWidth(), getHeight());

  for (int x = 0; x < getWidth(); x++) {
    for (int y = 0; y < getHeight(); y++) {
      PixelRGB px = getPixel(x, y);
      int l = 0.299 * px.r + 0.587 * px.g + 0.114 * px.b;
      res.setPixel(x, y, l);
    }
  }
  return res;
}

std::array<ImagePGM, 3> Image3::toYCbCr() const {

  std::array<ImagePGM, 3> res = {ImagePGM(getWidth(), getHeight()),
                                 ImagePGM(getWidth(), getHeight()),
                                 ImagePGM(getWidth(), getHeight())};
  for (int x = 0; x < getWidth(); x++) {
    for (int y = 0; y < getHeight(); y++) {
      PixelRGB px = getPixel(x, y);

      int Y = 0.299 * px.r + 0.587 * px.g + 0.114 * px.b;
      int Cb = -0.1687 * px.r - 0.3313 * px.g + 0.5 * px.b + 128;
      int Cr = 0.5 * px.r - 0.4187 * px.g - 0.0813 * px.b + 128;
      res[0].setPixel(x, y, Y);
      res[1].setPixel(x, y, Cb);
      res[2].setPixel(x, y, Cr);
    }
  }
  return res;
}

Image3 Image3::blurRef(Brush &b) const {
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

std::array<ImagePGM, 3> Image3::toHLS() const {
  std::array<ImagePGM, 3> res = {ImagePGM(getWidth(), getHeight()),
                                 ImagePGM(getWidth(), getHeight()),
                                 ImagePGM(getWidth(), getHeight())};

  for (int x = 0; x < getWidth(); x++) {
    for (int y = 0; y < getHeight(); y++) {
      PixelRGB px = getPixel(x, y);
      float r = px.r / 255.;
      float g = px.g / 255.;
      float b = px.b / 255.;
      float maxVal = std::max(std::max(r, g), b);
      float minVal = std::min(std::min(r, g), b);
      float delta = maxVal - minVal;
      PixelRGB hls;
      if (delta > 0) {
        int H = ((60 * std::fmod(((g - b) / delta), 6)));
        int L = (60 * ((b - r) / delta + 2));
        int S = (60 * ((r - g) / delta + 4));

        H = std::fmax(0.0, std::fmin(360.0, H));
        L = std::fmax(0.0, std::fmin(1.0, L));
        S = std::fmax(0.0, std::fmin(1.0, S));

        hls = {H * 255, L, S};
      } else {
        hls = {0, 0, 0};
      }
      res[0].setPixel(x, y, hls.r);
      res[0].setPixel(x, y, hls.g);
      res[0].setPixel(x, y, hls.b);
    }
  }

  return res;
}

Image3::Image3(ImagePGM h, ImagePGM l, ImagePGM s) {
  format = "P6";
  data.resize(3 * width * height);
  for (int x = 0; x < getWidth(); x++) {
    for (int y = 0; y < getHeight(); y++) {
      PixelRGB px = {h.getPixel(x, y), l.getPixel(x, y), s.getPixel(x, y)};
      float C = (1 - std::abs(2 * px.b - 1)) * px.g;
      float X = C * (1 - std::abs(std::fmod(px.r / 60, 2) - 1));
      float m = px.g - C / 2;
      float R, G, B;
      if (px.r < 60) {
        R = C;
        G = X;
        B = 0;
      } else if (px.r < 120) {
        R = X;
        G = C;
        B = 0;
      } else if (px.r < 180) {
        R = 0;
        G = C;
        B = X;
      } else if (px.r < 240) {
        R = 0;
        G = X;
        B = C;
      } else if (px.r < 300) {
        R = X;
        G = 0;
        B = C;
      } else {
        R = C;
        G = 0;
        B = X;
      }
      setPixel(x, y,
               {static_cast<unsigned char>((R + m) * 255),
                static_cast<unsigned char>((G + m) * 255),
                static_cast<unsigned char>((B + m) * 255)});
    }
  }
}