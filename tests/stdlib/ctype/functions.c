#include <ctype.h>
#include <stdio.h>

int main(void) {
  volatile int upper = 'A';
  volatile int lower = 'a';
  volatile int digit = '7';
  volatile int hex = 'F';
  volatile int nonhex = 'g';
  volatile int blank = ' ';
  volatile int tab = '\t';
  volatile int newline = '\n';
  volatile int punctuation = '!';
  volatile int nul = 0;
  volatile int del = 127;
  volatile int high = 255;
  volatile int eof = EOF;

  printf("isalnum %d %d %d\n", isalnum(upper) ? 1 : 0,
         isalnum(digit) ? 1 : 0, isalnum(punctuation) ? 1 : 0);
  printf("isalpha %d %d\n", isalpha(upper) ? 1 : 0,
         isalpha(digit) ? 1 : 0);
  printf("isblank %d %d %d\n", isblank(blank) ? 1 : 0,
         isblank(tab) ? 1 : 0, isblank(newline) ? 1 : 0);
  printf("iscntrl %d %d %d\n", iscntrl(newline) ? 1 : 0,
         iscntrl(nul) ? 1 : 0, iscntrl(upper) ? 1 : 0);
  printf("isdigit %d %d\n", isdigit(digit) ? 1 : 0,
         isdigit(upper) ? 1 : 0);
  printf("isgraph %d %d %d\n", isgraph(punctuation) ? 1 : 0,
         isgraph(blank) ? 1 : 0, isgraph(newline) ? 1 : 0);
  printf("islower %d %d\n", islower(lower) ? 1 : 0,
         islower(upper) ? 1 : 0);
  printf("isprint %d %d %d %d %d\n", isprint(blank) ? 1 : 0,
         isprint(upper) ? 1 : 0, isprint(newline) ? 1 : 0,
         isprint(del) ? 1 : 0, isprint(high) ? 1 : 0);
  printf("ispunct %d %d\n", ispunct(punctuation) ? 1 : 0,
         ispunct(upper) ? 1 : 0);
  printf("isspace %d %d %d\n", isspace(blank) ? 1 : 0,
         isspace(newline) ? 1 : 0, isspace(upper) ? 1 : 0);
  printf("isupper %d %d\n", isupper(upper) ? 1 : 0,
         isupper(lower) ? 1 : 0);
  printf("isxdigit %d %d %d\n", isxdigit(hex) ? 1 : 0,
         isxdigit(lower) ? 1 : 0, isxdigit(nonhex) ? 1 : 0);
  printf("tolower %d %d %d\n", tolower(upper), tolower(punctuation),
         tolower(eof));
  printf("toupper %d %d %d\n", toupper(lower), toupper(punctuation),
         toupper(eof));
  printf("eof %d %d\n", isalpha(eof) ? 1 : 0, isdigit(eof) ? 1 : 0);
  return 0;
}
