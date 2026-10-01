#include <stdio.h>

enum Color { Red, Green = 4, Blue };
enum Signed { Low = -3, Mid, High = 7 };
enum Small : unsigned char { Tiny = 1, Huge = 250 };
typedef enum { Off, On } Switch;

struct Lamp {
  Switch state;
  enum Color color;
  enum Small size;
};

static enum Color favorite = Blue;
static enum Signed levels[3] = {Low, Mid, High};

static enum Color next(enum Color color) {
  if (color == Red)
    return Green;
  if (color == Green)
    return Blue;
  return Red;
}

static Switch flip(Switch value) {
  if (value == On)
    return Off;
  return On;
}

int main(void) {
  struct Lamp lamp = {On, Green, Huge};
  enum Color color = Red;
  int sum = 0;
  for (int i = 0; i < 4; i++) {
    color = next(color);
    sum += color;
  }
  lamp.state = flip(lamp.state);
  lamp.color = next(lamp.color);
  enum Small size = lamp.size;
  size = (enum Small)(size + 3);
  printf("%d %d %d %d\n", sum, lamp.state, lamp.color, size);
  printf("%d %d %d %d\n", favorite, levels[0], levels[1], levels[2]);
  printf("%zu %zu %zu\n", sizeof(enum Color), sizeof(enum Small), sizeof(struct Lamp));
  return levels[0] + favorite;
}
