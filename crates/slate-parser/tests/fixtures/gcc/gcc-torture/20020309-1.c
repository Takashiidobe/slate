// SLATE-FILECHECK-DEFINES DEFAULT

int
sub1 (char *p, int i)
{
  char j = p[i];

  {
    void
    sub2 ()
      {
	i = 2;
	p = p + 2;
      }
  }
}

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × unsupported in numeric IR lowering: module statement
// SLATE-FILECHECK-END DEFAULT
