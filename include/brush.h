#ifndef BRUSH_H
#define BRUSH_H

struct Brush {

  virtual bool isInScope(int cx, int cy, int x, int y, int width,
                         int height) const = 0;
  virtual int getSizeX() const = 0;
  virtual int getSizeY() const = 0;
};

class CircleBrush : public Brush {
  int radius;

public:
  CircleBrush(int r) : Brush(), radius(r) {}

  bool isInScope(int cx, int cy, int x, int y, int width,
                 int height) const override;
  int getSizeX() const override { return radius; }
  int getSizeY() const override { return radius; }
};

#endif
