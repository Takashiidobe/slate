#include <stdio.h>

__attribute__((target("default"))) static int gnu_target_default(int value) {
  return value + 1;
}

__attribute__((target("sse2"))) static int gnu_target_sse2(int value) {
  return value + 2;
}

__attribute__((target_clones("default", "arch=x86-64-v2"))) static int
gnu_target_clones(int value) {
  return value + 3;
}

int gnu_cpu_dispatch(int value) __attribute__((cpu_dispatch(generic, haswell)));

int __attribute__((cpu_specific(generic))) gnu_cpu_dispatch(int value) {
  return value + 4;
}

int __attribute__((cpu_specific(haswell))) gnu_cpu_dispatch(int value) {
  return value + 4;
}

int main(void) {
  printf("%d %d %d %d\n", gnu_target_default(10), gnu_target_sse2(10),
         gnu_target_clones(10), gnu_cpu_dispatch(10));
  return 0;
}
