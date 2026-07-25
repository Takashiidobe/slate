#include <stdio.h>
#include <wchar.h>
int main(void) {
  wchar_t *s = L"hello";
  printf("%zu\n", wcslen(s));
  return 0;
}
