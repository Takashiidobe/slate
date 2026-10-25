/* { dg-options "/Zc:preprocessor /O2 /Ob2 /Oi /Ot /GS- /Gy /Gw /GF /Z7 /EHsc /GR- /W3 /wd4996 /WX- /Zc:wchar_t /Zc:forScope /Zc:inline /permissive- /MT /Qspectre /TC" } */
void abort(void);

static int square(int value) { return value * value; }

int main(void) {
  int total = 0;
  for (int i = 0; i < 5; i++)
    total += square(i);
  if (total != 30)
    abort();
  return 0;
}
