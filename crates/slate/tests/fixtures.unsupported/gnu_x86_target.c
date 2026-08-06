#define _GNU_SOURCE
#include <cpuid.h>
#include <stdint.h>
#include <stdio.h>

static int gnu_cpu_queries(void) {
  __builtin_cpu_init();
  return !!__builtin_cpu_supports("cmov") +
         2 * !!__builtin_cpu_supports("sse2") +
         4 * !!__builtin_cpu_supports("avx") +
         8 * !!__builtin_cpu_supports("avx2") +
         16 * !!__builtin_cpu_supports("bmi2") +
         32 * !!__builtin_cpu_is("intel") + 64 * !!__builtin_cpu_is("amd");
}

static int gnu_cpuid_query(void) {
  unsigned int eax;
  unsigned int ebx;
  unsigned int ecx;
  unsigned int edx;
  unsigned int maximum = __get_cpuid_max(0, NULL);
  int          total   = maximum > 0;

  if (__get_cpuid(0, &eax, &ebx, &ecx, &edx)) {
    total += eax == maximum;
    total += ebx != 0;
    total += ecx != 0;
    total += edx != 0;
  }
  return total;
}

static int gnu_instruction_builtins(void) {
  unsigned int       auxiliary = 0;
  unsigned long long first;
  unsigned long long second;
  int                value = 0;
  int                total = 0;

  __builtin_ia32_pause();
  __builtin_ia32_lfence();
  first = __builtin_ia32_rdtsc();
  __builtin_ia32_mfence();
  second = __builtin_ia32_rdtscp(&auxiliary);
  __builtin_ia32_sfence();
  __builtin_nontemporal_store(27, &value);
  total += __builtin_nontemporal_load(&value) == 27;
  total += first != 0;
  total += second >= first;
  total += __builtin_readcyclecounter() != 0;
  return total;
}

static int gnu_segment_types(void) {
  int __seg_fs *fs_pointer = 0;
  int __seg_gs *gs_pointer = 0;
  return fs_pointer == 0 && gs_pointer == 0 &&
         sizeof(fs_pointer) == sizeof(void *) &&
         sizeof(gs_pointer) == sizeof(void *);
}

static int gnu_register_variable(void) {
  register uintptr_t value __asm__("r12") = 19;
  __asm__ volatile("" : "+r"(value));
  return (int)value;
}

int main(void) {
  printf("%d %d %d %d %d\n", gnu_cpu_queries(), gnu_cpuid_query(),
         gnu_instruction_builtins(), gnu_segment_types(),
         gnu_register_variable());
  return 0;
}
