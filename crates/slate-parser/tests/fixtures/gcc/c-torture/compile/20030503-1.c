// SLATE-FILECHECK-DEFINES DEFAULT

void bar (void);

void foo ()
{
  if (1)
    goto foo;
  else
    for (;;)
      {
      foo:
	bar ();
	return;
      }
}
