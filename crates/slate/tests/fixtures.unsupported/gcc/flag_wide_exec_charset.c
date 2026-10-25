/* { dg-options "-fwide-exec-charset=UTF-32BE" } */
#include <stdio.h>
#include <wchar.h>

int main(void) {
  const wchar_t text[] = L"Azé";
  for (unsigned i = 0; i < sizeof text / sizeof text[0]; i++)
    printf("%08x ", (unsigned)text[i]);
  printf("%08x\n", (unsigned)L'Z');
  return 0;
}
