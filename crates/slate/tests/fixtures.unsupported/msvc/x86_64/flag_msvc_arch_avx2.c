/* { dg-options "/arch:AVX2" } */
void abort(void);

int main(void) {
#if !defined(__AVX__) || !defined(__AVX2__)
  abort();
#endif
#ifdef __AVX512F__
  abort();
#endif
  return 0;
}
