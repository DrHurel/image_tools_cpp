#include "brush.h"

#include "image_pgm.h"
#include "image_ppm.h"
int main() {
  ImagePGM img("./assets/man.pgm");
  img.write("./output/man2.pgm");

  ImagePPM img2("./assets/baboon.ppm");
  img2.write("./output/baboon2.ppm");

  img.writeProfil("./output/histo.txt", 30, true);
  img2.writeProfil("./output/histo2.txt", 30, true);

  img.writeProfil("./output/histo_column.txt", 30, false);
  img2.writeProfil("./output/histo2_column.txt", 30, false);

  auto b = CircleBrush(20);

  img.blur(b).write("./output/blurred.pgm");
  img2.blur(b).write("./output/blurred2.ppm");

  img.negate();
  img.write("./output/negated.pgm");

  img2.negate();
  img2.write("./output/negated2.ppm");

  img.mapGradient().blur(CircleBrush(1)).write("./output/gradient.pgm");

  return 0;
}