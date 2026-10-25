/* { dg-options "/fp:strict" } */
void abort(void);

int main(void) {
#if !defined(_M_FP_STRICT) || !defined(_M_FP_EXCEPT) || defined(_M_FP_FAST)
  abort();
#endif
  return 0;
}
