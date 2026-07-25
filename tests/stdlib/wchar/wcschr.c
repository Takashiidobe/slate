#include <stdio.h>
#include <wchar.h>
int main(void) {
  wchar_t s[] = L"abcdef";
  wchar_t *p = wcschr(s, L'd');
  printf("%ld %d\n", (long)(p - s), wcschr(s, L'z') == NULL);
  return 0;
}
