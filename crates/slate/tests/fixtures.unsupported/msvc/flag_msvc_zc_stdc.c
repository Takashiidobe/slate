/* { dg-options "/Zc:preprocessor /Zc:__STDC__" } */
void abort(void);

int main(void) {
#if !defined(__STDC__) || __STDC__ != 1
  abort();
#endif
  return 0;
}
