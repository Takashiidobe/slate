/* { dg-options "/Za" } */
void abort(void);

int main(void) {
#if !defined(__STDC__) || __STDC__ != 1
  abort();
#endif
#ifdef _MSC_EXTENSIONS
  abort();
#endif
  return 0;
}
