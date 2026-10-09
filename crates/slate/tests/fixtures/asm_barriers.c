#include <stdint.h>
#include <stdio.h>

static int typecheck_probe(void) { __asm__(""); return 1; }

static void empty_volatile(void) { __asm__ __volatile__(""); }

static int memory_barrier(int *p) {
  *p = 5;
  __asm__ __volatile__("" ::: "memory");
  return *p;
}

static uint32_t value_barrier(uint32_t x) {
  __asm__ __volatile__("" : "+r"(x));
  return x * 3;
}

static uint64_t value_barrier_memory(uint64_t x) {
  __asm__ __volatile__("" : "+r"(x) : : "memory");
  return x + 1;
}

static int input_barrier(const unsigned char *p, int n) {
  int sum = 0;
  for (int i = 0; i < n; i++)
    sum += p[i];
  __asm__ __volatile__("" : : "r"(p) : "memory");
  return sum;
}

#define CURLWARNING(id, message)                                               \
  static void __attribute__((__warning__(message)))                            \
  __attribute__((__unused__)) __attribute__((__noinline__))                    \
  id(void) { __asm__(""); }

CURLWARNING(curl_wrong_type, "wrong type")

static int verify_volatile(const unsigned char *x, const unsigned char *y, int n) {
  volatile uint16_t d = 0U;
  for (int i = 0; i < n; i++)
    d |= x[i] ^ y[i];
  __asm__ __volatile__("" : "+r"(d) :);
  return (1 & ((d - 1) >> 8)) - 1;
}

static int nonvolatile_guard(const unsigned char *x, int n) {
  unsigned char d = 0;
  uint16_t e;
  for (int i = 0; i < n; i++)
    d |= x[i];
  e = d;
  __asm__("" : "+r"(e) :);
  return (1 & ((e - 1) >> 8)) - 1;
}

static int no_unroll(const float *p, int n) {
  float sum = 0;
  for (int i = 0; i < n; i++) {
    __asm__(""::"r"(p));
    sum += p[i];
  }
  return (int)sum;
}

static void spin_pause(void) { __asm__ __volatile__("pause"); }

static void spin_pause_memory(void) { __asm__ __volatile__("pause" ::: "memory"); }

static void spin_rep_nop(void) { __asm__ __volatile__("rep; nop" ::: "memory"); }

int main(void) {
  int cell = 0;
  unsigned char bytes[4] = {1, 2, 3, 4};
  empty_volatile();
  spin_pause();
  spin_rep_nop();
  printf("%d %d\n", typecheck_probe(), memory_barrier(&cell));
  printf("%u %llu\n", value_barrier(7), (unsigned long long)value_barrier_memory(41));
  printf("%d\n", input_barrier(bytes, 4));
  unsigned char zeros[4] = {0};
  float floats[3] = {1.5f, 2.5f, 3.0f};
  spin_pause_memory();
  printf("%d %d\n", verify_volatile(bytes, bytes, 4), verify_volatile(bytes, zeros, 4));
  printf("%d %d\n", nonvolatile_guard(zeros, 4), nonvolatile_guard(bytes, 4));
  printf("%d\n", no_unroll(floats, 3));
  return 0;
}
