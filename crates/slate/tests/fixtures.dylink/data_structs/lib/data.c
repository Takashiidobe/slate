#include <string.h>
#include "dylink.h"

struct vec3 {
  double x, y, z;
};

struct small {
  int a, b;
};

struct mixed {
  char tag;
  long long big;
  float ratio;
};

EXPORT int plug_counter = 5;

EXPORT const char plug_banner[] = "dylink data fixture";

EXPORT struct vec3 plug_origin = {1.0, 2.0, 3.0};

EXPORT const char *plug_name(void) { return "data-lib"; }

EXPORT struct vec3 plug_scale(struct vec3 v, double k) {
  struct vec3 out = {v.x * k, v.y * k, v.z * k};
  return out;
}

EXPORT struct small plug_swap(struct small s) {
  struct small out = {s.b, s.a};
  return out;
}

EXPORT struct mixed plug_bump(struct mixed m) {
  m.tag = (char)(m.tag + 1);
  m.big += 10;
  m.ratio *= 2.0f;
  return m;
}

EXPORT void plug_fill(struct small *out, int n) {
  for (int i = 0; i < n; i++) {
    out[i].a = i;
    out[i].b = i * i;
  }
}

EXPORT long long plug_sum(const int *values, int n) {
  long long total = 0;
  for (int i = 0; i < n; i++) total += values[i];
  return total;
}

EXPORT void plug_upper(char *text) {
  for (; *text; text++)
    if (*text >= 'a' && *text <= 'z') *text = (char)(*text - 32);
}

EXPORT int plug_bump_counter(void) { return ++plug_counter; }
