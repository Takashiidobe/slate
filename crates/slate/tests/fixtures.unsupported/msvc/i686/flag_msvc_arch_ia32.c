/* { dg-options "/arch:IA32" } */
void abort(void);

int main(void) {
#if !defined(_M_IX86_FP) || _M_IX86_FP != 0
  abort();
#endif
  return 0;
}
