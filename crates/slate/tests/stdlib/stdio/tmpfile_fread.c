#include <stdio.h>
int main(void) {
  FILE *f      = tmpfile();
  char  in[4]  = "abc";
  char  out[4] = {0};
  fwrite(in, 1, 3, f);
  rewind(f);
  fread(out, 1, 3, f);
  puts(out);
  fclose(f);
  return 0;
}
