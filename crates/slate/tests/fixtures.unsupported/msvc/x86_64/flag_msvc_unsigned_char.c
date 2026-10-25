/* { dg-options "/J" } */
void abort(void);

int main(void) {
  char c = (char)200;
  if (c != 200 || !(c > 0) || (char)-1 != 255)
    abort();
  return 0;
}
