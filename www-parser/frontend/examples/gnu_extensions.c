#define max(a, b) ({ __typeof__(a) _a = (a); __typeof__(b) _b = (b); _a > _b ? _a : _b; })

__attribute__((aligned(16))) struct vec4 { float x, y, z, w; };

static inline __attribute__((always_inline)) unsigned rotl(unsigned value, int shift) {
  return (value << shift) | (value >> (32 - shift));
}

int clamp_sum(int a, int b, int limit) {
  int total = __builtin_add_overflow(a, b, &total) ? limit : total;
  return max(total, limit) == total ? limit : (int)rotl((unsigned)total, 3);
}
