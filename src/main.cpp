#include "brush.h"

#include "image_pgm.h"
#include "image_ppm.h"
#include "ui.h"
#include <string>
int main(const int argc, const char *argv[]) {

  int i = 0;
  auto YCbCr = Image3("./assets/peppers.ppm").toYCbCr();
  for (auto img : YCbCr) {
    img.write(std::string("./output/") + std::to_string(i) +
              std::string("_peppers.pgm"));
    i++;
  }

  Image3(YCbCr[0], YCbCr[1], YCbCr[2], true)
      .write("./output/test_fix_peppers.ppm");
  Image3(YCbCr[0], YCbCr[1], YCbCr[2], false)
      .write("./output/test_fix_peppers_notrgb.ppm");

  auto b = CircleBrush(3);
  Image3("./assets/baboon.ppm").blurRef(b).write("./output/blur_bab_ref.ppm");

  auto temp = Image3("./assets/baboon.ppm").toHLS();
  temp[0].write("./output/hsl0.ppm");
  temp[1].write("./output/hsl1.ppm");
  temp[2].write("./output/hsl2.ppm");

  /*
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
  */
  return 0;
}