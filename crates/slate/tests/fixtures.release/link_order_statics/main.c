#include <stdio.h>
#include "entry.h"

extern const struct slate_entry __start_slate_static_set[];
extern const struct slate_entry __stop_slate_static_set[];

unsigned long work_u1(unsigned long), work_u2(unsigned long), work_u3(unsigned long),
    work_u4(unsigned long), work_u5(unsigned long), work_u6(unsigned long),
    work_u7(unsigned long), work_u8(unsigned long);

static int value_main(void) { return 0; }

__attribute__((used, section("slate_static_set"))) static const struct slate_entry entry_main = {
    0, &entry_main, value_main};

int main(void) {
  for (const struct slate_entry *entry = __start_slate_static_set; entry < __stop_slate_static_set;
       entry++)
    printf("%d:%d:%d ", entry->id, entry->self == entry, entry->value());
  unsigned long mix = work_u1(1) ^ work_u2(2) ^ work_u3(3) ^ work_u4(4) ^ work_u5(5) ^
                      work_u6(6) ^ work_u7(7) ^ work_u8(8);
  printf("\n%lu\n", mix);
  return 0;
}
