#include <ctype.h>
#include <stdio.h>

int main(void) {
  char alpha = 'A';
  char digit = '5';
  char space = ' ';
  char vtab  = '\v';
  char punct = '!';

  if (isalpha(alpha)) {
    printf("alpha-yes\n");
  } else {
    printf("alpha-no\n");
  }
  if (!isalpha(digit)) {
    printf("not-alpha-yes\n");
  } else {
    printf("not-alpha-no\n");
  }
  if (isdigit(digit)) {
    printf("digit-yes\n");
  } else {
    printf("digit-no\n");
  }
  if (isupper(alpha)) {
    printf("upper-yes\n");
  } else {
    printf("upper-no\n");
  }
  if (islower(alpha)) {
    printf("lower-yes\n");
  } else {
    printf("lower-no\n");
  }
  if (isalnum(punct)) {
    printf("alnum-yes\n");
  } else {
    printf("alnum-no\n");
  }
  if (isxdigit(alpha)) {
    printf("xdigit-yes\n");
  } else {
    printf("xdigit-no\n");
  }
  if (ispunct(punct)) {
    printf("punct-yes\n");
  } else {
    printf("punct-no\n");
  }
  if (iscntrl(vtab)) {
    printf("cntrl-yes\n");
  } else {
    printf("cntrl-no\n");
  }
  if (isgraph(punct)) {
    printf("graph-yes\n");
  } else {
    printf("graph-no\n");
  }
  if (isprint(space)) {
    printf("print-yes\n");
  } else {
    printf("print-no\n");
  }
  if (isspace(space)) {
    printf("space-yes\n");
  } else {
    printf("space-no\n");
  }
  if (isspace(vtab)) {
    printf("vtab-space-yes\n");
  } else {
    printf("vtab-space-no\n");
  }

  printf("%d\n", isalpha(alpha));

  return 0;
}
// REWRITES-NOT: fn isdigit(
// REWRITES-NOT: fn isupper(
// REWRITES-NOT: fn islower(
// REWRITES-NOT: fn isalnum(
// REWRITES-NOT: fn isxdigit(
// REWRITES-NOT: fn ispunct(
// REWRITES-NOT: fn iscntrl(
// REWRITES-NOT: fn isgraph(
// REWRITES-NOT: fn isprint(
// REWRITES-NOT: fn isspace(
// REWRITES-LABEL: {{^}}fn main() {
// REWRITES-DAG: .is_ascii_alphabetic()
// REWRITES-DAG: .is_ascii_digit()
// REWRITES-DAG: .is_ascii_uppercase()
// REWRITES-DAG: .is_ascii_lowercase()
// REWRITES-DAG: .is_ascii_alphanumeric()
// REWRITES-DAG: .is_ascii_hexdigit()
// REWRITES-DAG: .is_ascii_punctuation()
// REWRITES-DAG: .is_ascii_control()
// REWRITES-DAG: .is_ascii_graphic()
// REWRITES-DAG: .is_ascii_whitespace()
// REWRITES-DAG: unsafe { isalpha(
// REWRITES: {{^}}}
