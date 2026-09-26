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
// DEFAULT: Error:   × invalid in this context: function definition is not allowed here
// SLATE-FILECHECK-END DEFAULT
