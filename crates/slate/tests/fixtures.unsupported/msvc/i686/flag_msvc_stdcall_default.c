/* { dg-options "/Zc:preprocessor /Gz /std:c11" } */
void __cdecl abort(void);

static int add(int a, int b) { return a + b; }

int main(void) {
  if (_Generic(&add, int(__stdcall *)(int, int): 1, default: 0) != 1)
    abort();
  if (add(2, 3) != 5)
    abort();
  return 0;
}
