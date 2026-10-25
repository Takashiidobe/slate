/* { dg-options "/openmp" } */
void abort(void);

int main(void) {
#if !defined(_OPENMP) || _OPENMP != 200203
  abort();
#endif
  return 0;
}
