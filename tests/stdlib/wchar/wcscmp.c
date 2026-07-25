#include <stdio.h>
#include <wchar.h>
int main(void) {
  wchar_t a[] = L"abc";
  wchar_t b[] = L"abd";
  printf("%d %d\n", wcscmp(a, a) == 0, wcscmp(a, b) < 0);
  return 0;
}
