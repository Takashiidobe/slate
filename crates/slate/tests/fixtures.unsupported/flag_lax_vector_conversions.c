/* { dg-options "-flax-vector-conversions=all" } */
int printf(const char *, ...);

typedef int v4si __attribute__((vector_size(16)));
typedef float v4sf __attribute__((vector_size(16)));
typedef unsigned char v16qi __attribute__((vector_size(16)));

static v4si as_ints(v4si value) { return value; }

int main(void) {
  v4sf floats = {1.0f, -2.0f, 0.5f, 0.0f};
  v4si ints = as_ints(floats);
  v16qi bytes = ints;
  printf("%x %x %x %x %u\n", ints[0], ints[1], ints[2], ints[3], bytes[3]);
  return 0;
}
