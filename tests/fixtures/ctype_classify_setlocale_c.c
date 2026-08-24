#include <ctype.h>
#include <locale.h>
#include <stdio.h>

int main(void) {
  setlocale(LC_ALL, "C");
  char c = 'A';
  if (isalpha(c)) {
    printf("yes\n");
  } else {
    printf("no\n");
  }
  return 0;
}
// REWRITES-LABEL: {{^}}fn main() {
// REWRITES-DAG: .is_ascii_alphabetic()
// REWRITES: {{^}}}
