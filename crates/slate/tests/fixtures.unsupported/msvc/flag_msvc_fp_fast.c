/* { dg-options "/fp:fast" } */
void abort(void);

int main(void) {
#if !defined(_M_FP_FAST) || !defined(_M_FP_CONTRACT) || defined(_M_FP_PRECISE)
  abort();
#endif
  return 0;
}
