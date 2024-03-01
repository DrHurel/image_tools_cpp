
#include "ui.h"
#include <iostream>

void printUsage(const std::string name) {

  std::cout << "Usage: " << name
            << " -i <input_file> -f <filter> -o <output_file>" << std::endl;
}
