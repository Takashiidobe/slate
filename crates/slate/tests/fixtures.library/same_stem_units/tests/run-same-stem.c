extern int combined(int x);
extern int free_value(int x);
extern int ng_free_value(int x);

int main(void) {
  if (combined(3) != 5 * 100 + 17)
    return 1;
  if (free_value(1) != 4 || ng_free_value(1) != 14)
    return 2;
  return 0;
}
