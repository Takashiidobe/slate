typedef unsigned __int64 u64;

__declspec(align(16)) struct vec4 { float x, y, z, w; };

static __forceinline u64 rotl(u64 value, int shift) {
  return (value << shift) | (value >> (64 - shift));
}

u64 mix(u64 seed) {
  struct vec4 v = { 1.0f, 2.0f, 3.0f, 4.0f };
  return rotl(seed, 13) ^ (u64)(v.x + v.w) ^ __alignof(struct vec4);
}
