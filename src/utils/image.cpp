#include "image.h"
#include <fstream>
#include <iostream>

#include <vector>

Image::Image() {
  width = 0;
  height = 0;
  data = std::vector<char>();
}

void Image::ignore_comments(std::ifstream &file) {
  unsigned char c;
  while ((c = file.get()) == '#')
    while ((c = file.get()) != '\n')
      ;
  file.seekg(-sizeof(unsigned char), std::ios::cur);
}
