#include "brush.h"

#include "image_pgm.h"
#include "image_ppm.h"
#include "ui.h"
#include <string>
int main(const int argc, const char *argv[]) {

  if (argc != 7) {
    printUsage(argv[0]);
  }

  std::string in;
  std::string out;
  std::string fitlter = 0;

  for (int i = 1; i < argc; i += 2) {
    std::string p = argv[i];

    if (p[0] == '-' && p.length() == 2) {
      switch (p[1]) {
      case 'o':
        out = argv[i + 1];
        break;
      case 'i':
        in = argv[i + 1];
        break;
      case 'f':
        fitlter = argv[i + 1];
        break;
      default:
        printUsage(argv[0]);
      }
    } else {
      printUsage(argv[0]);
    }
  }

  return 0;
}