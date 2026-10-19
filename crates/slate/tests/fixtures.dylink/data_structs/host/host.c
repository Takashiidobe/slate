#include <stdio.h>
#include <string.h>
#include "dylink.h"
#include "dylink_path.h"

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

int main(void) {
  void *lib = dl_open(DEP_PATH);
  if (!lib) {
    printf("open failed\n");
    return 1;
  }
  int *counter = (int *)dl_sym(lib, "plug_counter");
  const char *banner = (const char *)dl_sym(lib, "plug_banner");
  struct vec3 *origin = (struct vec3 *)dl_sym(lib, "plug_origin");
  const char *(*name)(void) = (const char *(*)(void))dl_sym(lib, "plug_name");
  struct vec3 (*scale)(struct vec3, double) =
      (struct vec3(*)(struct vec3, double))dl_sym(lib, "plug_scale");
  struct small (*swap)(struct small) = (struct small(*)(struct small))dl_sym(lib, "plug_swap");
  struct mixed (*bump)(struct mixed) = (struct mixed(*)(struct mixed))dl_sym(lib, "plug_bump");
  void (*fill)(struct small *, int) = (void (*)(struct small *, int))dl_sym(lib, "plug_fill");
  long long (*sum)(const int *, int) = (long long (*)(const int *, int))dl_sym(lib, "plug_sum");
  void (*upper)(char *) = (void (*)(char *))dl_sym(lib, "plug_upper");
  int (*bump_counter)(void) = (int (*)(void))dl_sym(lib, "plug_bump_counter");
  if (!counter || !banner || !origin || !name || !scale || !swap || !bump || !fill || !sum ||
      !upper || !bump_counter) {
    printf("resolve failed\n");
    return 2;
  }

  printf("counter = %d\n", *counter);
  *counter = 40;
  int bumped_counter = bump_counter();
  printf("bump_counter = %d, counter = %d\n", bumped_counter, *counter);
  printf("banner = %s (%zu bytes)\n", banner, strlen(banner));
  printf("origin = %.1f %.1f %.1f\n", origin->x, origin->y, origin->z);
  origin->z = -1.0;
  struct vec3 scaled = scale(*origin, 2.5);
  printf("scaled = %.2f %.2f %.2f\n", scaled.x, scaled.y, scaled.z);
  printf("name = %s\n", name());

  struct small s = {3, 9};
  struct small swapped = swap(s);
  printf("swapped = %d %d\n", swapped.a, swapped.b);

  struct mixed m = {'a', 5000000000LL, 1.25f};
  struct mixed bumped = bump(m);
  printf("bumped = %c %lld %.2f\n", bumped.tag, bumped.big, (double)bumped.ratio);

  struct small table[4];
  fill(table, 4);
  for (int i = 0; i < 4; i++) printf("table[%d] = %d %d\n", i, table[i].a, table[i].b);

  int values[5] = {10, 20, 30, 40, -5};
  printf("sum = %lld\n", sum(values, 5));

  char text[] = "mixed Case text";
  upper(text);
  printf("upper = %s\n", text);

  dl_close(lib);
  return 0;
}
