#include "image.h"
int main() {
  ImagePGM img("./assets/man.pgm");
  img.write("./output/man2.pgm");

  ImagePPM img2("./assets/baboon.ppm");
  img2.write("./output/baboon2.ppm");

  img.writeProfil("./output/histo.txt", 30, true);
  img2.writeProfil("./output/histo2.txt", 30, true);

  img.writeProfil("./output/histo_column.txt", 30, false);
  img2.writeProfil("./output/histo2_column.txt", 30, false);

  return 0;
}