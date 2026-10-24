/* { dg-options "-fdollars-in-identifiers" } */
int printf(const char *, ...);

static int $counter = 3;

static int add$one(int value$in) { return value$in + 1; }

int main(void) {
  int local$ = add$one($counter);
  printf("%d\n", local$);
  return 0;
}
