
#include <cmath>
#include <stdio.h>
#include <stdlib.h>

int gradient_vertical_pgm(unsigned char *Img, int x, int y, int width) {
  return Img[(x - 1) + y * width] - Img[(x + 1) + y * width];
}

int gradient_horizontal_pgm(unsigned char *Img, int x, int y, int width) {
  return Img[x + (y - 1) * width] - Img[x + (y + 1) * width];
}

int norme_gradient(unsigned char *Img, int x, int y, int width) {
  return sqrt(pow(gradient_vertical_pgm(Img, x, y, width), 2) +
              pow(gradient_horizontal_pgm(Img, x, y, width), 2));
}

int map_gradient(unsigned char *ImgIn, unsigned char *ImgOut, int width,
                 int height) {
  for (int i = 1; i < width - 1; i++) {
    for (int j = 1; j < height - 1; j++) {

      ImgOut[i + j * width] = norme_gradient(ImgIn, i, j, width);
    }
  }
  return 0;
}

int threshold_gradient(unsigned char *ImgIn, unsigned char *ImgOut, int width,
                       int height, int threshold) {
  for (int i = 1; i < width - 1; i++) {
    for (int j = 1; j < height - 1; j++) {

      ImgOut[i + j * width] =
          norme_gradient(ImgIn, i, j, width) < threshold ? 0 : 255;
    }
  }
  return 0;
}

int threshold_gradient_hys(unsigned char *ImgIn, unsigned char *ImgOut,
                           int width, int height, int threshold_high,
                           int threshold_down, int radius) {
  for (int i = 1; i < width - 1; i++) {
    for (int j = 1; j < height - 1; j++) {
      int norme = norme_gradient(ImgIn, i, j, width);
      if (norme > threshold_high) {
        ImgOut[i + j * width] = 255;
      }
      if (norme < threshold_down) {
        ImgOut[i + j * width] = 0;
      }
      if (norme >= threshold_down && norme <= threshold_high) {
        ImgOut[i + j * width] = 127;
      }
    }
  }

  unsigned char *temp = malloc(sizeof(unsigned char) * width * height);

  if (temp == NULL) {
    printf("error\n");
    exit(EXIT_FAILURE);
  }

  copy(ImgOut, temp, width * height);

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (temp[i * width + j] == 255 || temp[i * width + j] == 0) {
        continue;
      }
      int max_neighbor = 0;
      for (int x = -radius; x <= radius; x++) {
        for (int y = -radius; y <= radius; y++) {
          int target = (i + x) * width + j + y;
          if (target >= 0 && target < width * height) {
            max_neighbor = temp[target] == 255 ? 255 : 0;
          }
        }
      }
      ImgOut[i * width + j] = max_neighbor;
    }
  }

  /*for (int i = 0; i < width * height; i++) {
    if (ImgOut[i] != 255 && ImgOut[i] != 0) {
      printf("error pxl %d\n", ImgOut[i]);
    }
  }*/
  return 0;
}