typedef unsigned _BitInt(17) u17;
typedef unsigned v4u __attribute__((vector_size(16)));

static int check17(u17 high, u17 low, u17 count) {
  u17 left = __builtin_elementwise_fshl(high, low, count);
  u17 right = __builtin_elementwise_fshr(high, low, count);
  return left != 2 || right != 0x10001;
}

#if __SIZEOF_POINTER__ == 8
typedef unsigned _BitInt(65) u65;

static int check65(u65 high, u65 low, u65 count) {
  u65 left = __builtin_elementwise_fshl(high, low, count);
  u65 right = __builtin_elementwise_fshr(high, low, count);
  return left != ((u65)1 << 16) || right != ((u65)1 << 49);
}
#endif

static int check_lanes(v4u high, v4u low, v4u count) {
  v4u left = __builtin_elementwise_fshl(high, low, count);
  v4u right = __builtin_elementwise_fshr(high, low, count);
  return left[0] != 1 || left[1] != 2 || left[2] != 1 || left[3] != 2 ||
         right[0] != 2 || right[1] != 0x80000001U || right[2] != 2 ||
         right[3] != 0x80000001U;
}

int main(void) {
  int failures = check17(1, 2, 18);
#if __SIZEOF_POINTER__ == 8
  failures += 2 * check65(1, 0, (u65)1 << 64);
#endif
  return failures + 4 * check_lanes((v4u){1, 1, 1, 1}, (v4u){2, 2, 2, 2},
                                    (v4u){0, 1, 32, 33});
}
