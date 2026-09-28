// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT c17

/* { dg-skip-if "too many arguments in function call" { bpf-*-* } } */
/* { dg-additional-options "-std=gnu89" } */

struct utsname {
	char	sysname[32 ];	 
	char	version[32 ];	 
};
int
uname(name)
	struct utsname *name;
{
	int mib[2], rval;
	long len;
	char *p;
	int oerrno;
	if (sysctl(mib, 2, &name->sysname, &len, 0 , 0) == -1)
	  ;
	for (p = name->version; len--; ++p) {
				*p = ' ';
	}
}

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × unresolved ordinary name `sysctl`
// DEFAULT: ╭─[tests/fixtures/gcc/gcc-torture/20000403-1.c:17:6]
// DEFAULT: 16 │     int oerrno;
// DEFAULT: 17 │     if (sysctl(mib, 2, &name->sysname, &len, 0 , 0) == -1)
// DEFAULT: ·         ──────
// DEFAULT: 18 │       ;
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
