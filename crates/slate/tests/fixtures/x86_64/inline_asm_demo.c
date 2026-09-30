#include <stdio.h>

typedef unsigned int U32;

static int add_via_asm(int x) {
  __asm__ volatile("addl $5, %0" : "+r"(x));
  return x;
}

static U32 cpuid_leaf7_feature(void) {
  U32 eax, ebx, ecx;
  __asm__("cpuid" : "=a"(eax), "=b"(ebx), "=c"(ecx) : "a"(7), "c"(0) : "edx");
  return ebx != 0 || eax != 0 || ecx != 0;
}

int main(void) {
  printf("%d %u\n", add_via_asm(37), cpuid_leaf7_feature());
  return 0;
}
