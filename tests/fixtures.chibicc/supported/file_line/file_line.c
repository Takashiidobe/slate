#include "test.h"
#include "file_line_include.h"

char *main_filename1 = __FILE__;
int main_line1 = __LINE__;
#define LINE() __LINE__
int main_line2 = LINE();

int main() {
  ASSERT(0, strcmp(main_filename1, "./test/file_line.c"));
  ASSERT(5, main_line1);
  ASSERT(7, main_line2);
  ASSERT(0, strcmp(include_filename, "./test/file_line_include.h"));
  ASSERT(2, include_line);
  printf("OK\n");
  return 0;
}
