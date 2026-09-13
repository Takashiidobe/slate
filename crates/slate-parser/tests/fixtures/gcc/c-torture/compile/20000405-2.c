// SLATE-FILECHECK-DEFINES DEFAULT

extern void foo (int);

void bar (unsigned long l)
{
  foo(l == 0);
}
