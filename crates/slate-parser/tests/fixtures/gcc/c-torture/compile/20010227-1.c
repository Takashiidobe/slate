// SLATE-FILECHECK-DEFINES DEFAULT

void foo (void)
{
  double a = 0.0;
  double b = a;
  if (&b != &a);
}
