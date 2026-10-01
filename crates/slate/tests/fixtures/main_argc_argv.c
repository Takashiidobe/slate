#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
  printf("%d %d %d\n", argc, argv[0] != NULL, argv[argc] == NULL);
  size_t length = strlen(argv[0]);
  printf("%d\n", length > 0);
  for (int i = 1; i < argc; i++)
    printf("%s\n", argv[i]);
  return argc - 1;
}
