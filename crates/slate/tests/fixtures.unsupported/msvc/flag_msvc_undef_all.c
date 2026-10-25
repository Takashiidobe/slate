/* { dg-options "/u" } */
void abort(void);

int main(void) {
#if defined(_MSC_VER) || defined(_WIN32) || defined(_MT)
  abort();
#endif
  return 0;
}
