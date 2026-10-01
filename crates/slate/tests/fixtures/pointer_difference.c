int main(void) {
  int values[4];
  int *p = &values[1];
  int *q = &values[3];
  return (int)(q - p);
}
