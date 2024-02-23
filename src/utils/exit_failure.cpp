#include "utils.h"

#include <iostream>
#include <ostream>

void exit_failure_file(std::string filename) {
  std::cerr << "\nPas d'acces en ecriture sur le fichier " << filename
            << std::endl;
  exit(EXIT_FAILURE);
}