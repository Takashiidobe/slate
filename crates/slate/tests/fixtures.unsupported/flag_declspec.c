/* { dg-options "-fdeclspec" } */
int printf(const char *, ...);

__declspec(noinline) static int triple(int x) { return 3 * x; }

__declspec(align(16)) static int aligned_value = 5;

int main(void) {
  printf("%d %zu %d\n", triple(aligned_value), _Alignof(aligned_value), (int)((unsigned long)&aligned_value % 16));
  return 0;
}
