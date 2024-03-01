#ifndef UI_H
#define UI_H
#include <string>
typedef struct {
  char *input_file;
  char *filter;
  char *output_file;

} Config;

enum ImageType { PPM, PGM, UNKNOWN };

ImageType parse_image_type(const std::string &filename);

void printUsage(const std::string name);
Config parseArgv(char **argv, int argc);
#endif // UI_H
