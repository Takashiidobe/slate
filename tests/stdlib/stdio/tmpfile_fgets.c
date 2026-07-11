#include <stdio.h>
int main(void) { FILE *f = tmpfile(); fputs("abc\n", f); rewind(f); char b[8]; fgets(b, sizeof b, f); fputs(b, stdout); fclose(f); return 0; }
