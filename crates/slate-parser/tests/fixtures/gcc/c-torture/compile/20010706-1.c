// SLATE-FILECHECK-DEFINES DEFAULT

int
foo(unsigned int x)
{
  return (x << 1) | (x >> 31);
}
