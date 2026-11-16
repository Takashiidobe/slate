#include <stdio.h>

extern const int __start_slate_order_set[];
extern const int __stop_slate_order_set[];

unsigned long work_u1(unsigned long), work_u2(unsigned long), work_u3(unsigned long),
    work_u4(unsigned long), work_u5(unsigned long), work_u6(unsigned long),
    work_u7(unsigned long), work_u8(unsigned long);

__attribute__((used)) static int registered_main(void) { return 0; }

__asm__(".pushsection slate_order_set,\"aR\"\n"
        ".balign 4\n"
        ".long registered_main - .\n"
        ".popsection");

int main(void) {
  for (const int *entry = __start_slate_order_set; entry < __stop_slate_order_set; entry++) {
    int (*registered)(void) = (int (*)(void))((const char *)entry + *entry);
    printf("%d ", registered());
  }
  unsigned long mix = work_u1(1) ^ work_u2(2) ^ work_u3(3) ^ work_u4(4) ^ work_u5(5) ^
                      work_u6(6) ^ work_u7(7) ^ work_u8(8);
  printf("\n%lu\n", mix);
  return 0;
}
