#include "ui.h"

Config parseArgv(char **argv, int argc) {
  Config config;
  config.input_file = nullptr;
  config.filter = nullptr;
  config.output_file = nullptr;
  if (argc < 7) {
    printUsage(argv[0]);
  }

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      switch (argv[i][1]) {
      case 'i':
        config.input_file = argv[i + 1];
        break;
      case 'f':
        config.filter = argv[i + 1];
        break;
      case 'o':
        config.output_file = argv[i + 1];
        break;
      default:
        printUsage(argv[0]);
        break;
      }
    }
  }

  return config;
}