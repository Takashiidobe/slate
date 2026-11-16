// { dg-options "-O2" }
#include <stdio.h>

void __compiletime_assert_1(void);

#define BUILD_BUG_ON(cond)                                                     \
  do {                                                                         \
    if (cond)                                                                  \
      __compiletime_assert_1();                                                \
  } while (0)

struct list_head {
  struct list_head *next, *prev;
};

struct display_mode {
  struct list_head head;
  const char *name;
  int vtotal;
};

static const struct display_mode modes[] = {
    {{0, 0}, "640x480", 525},
    {{0, 0}, "720x576", 625},
    {{0, 0}, "1440x288", 312},
};

static const struct display_mode *mode_for(int index) { return &modes[index]; }

static const unsigned char non_inheritance[3] = {255, 1, 56};

int check_mode(void) {
  BUILD_BUG_ON(mode_for(2)->vtotal != 312);
  return mode_for(0)->vtotal;
}

int check_elems(void) {
  struct {
    unsigned char id, len, ext;
  } x;
  BUILD_BUG_ON(sizeof(x) != non_inheritance[1] + 2);
  return non_inheritance[2];
}

const void *volatile escaped_mode;
const void *volatile escaped_elems;

int main(void) {
  escaped_mode = mode_for(1);
  escaped_elems = non_inheritance;
  printf("%d %d %s\n", check_mode(), check_elems(), mode_for(1)->name);
  return 0;
}
