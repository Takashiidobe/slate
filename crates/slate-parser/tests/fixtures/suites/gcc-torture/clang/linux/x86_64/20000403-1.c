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
// DEFAULT: Error: -Wimplicit-function-declaration
// DEFAULT:   × call to undeclared function 'sysctl'; ISO C99 and later do not support
// DEFAULT:     ╭─[tests/fixtures/suites/gcc-torture/clang/linux/x86_64/20000403-1.c:17:6]
// DEFAULT:  16 │     int oerrno;
// DEFAULT:  17 │     if (sysctl(mib, 2, &name->sysname, &len, 0 , 0) == -1)
// DEFAULT:     ·         ──────
// DEFAULT:  18 │       ;
// DEFAULT:     ╰────
// DEFAULT:   ⚠ a function definition without a prototype is deprecated in all versions of
// DEFAULT:     ╭─[tests/fixtures/suites/gcc-torture/clang/linux/x86_64/20000403-1.c:9:1]
// DEFAULT:   8 │     };
// DEFAULT:   9 │ ╭─▶ int
// DEFAULT:  10 │ │   uname(name)
// DEFAULT:  11 │ │       struct utsname *name;
// DEFAULT:  12 │ │   {
// DEFAULT:  13 │ │       int mib[2], rval;
// DEFAULT:  14 │ │       long len;
// DEFAULT:  15 │ │       char *p;
// DEFAULT:  16 │ │       int oerrno;
// DEFAULT:  17 │ │       if (sysctl(mib, 2, &name->sysname, &len, 0 , 0) == -1)
// DEFAULT:  18 │ │         ;
// DEFAULT:  19 │ │       for (p = name->version; len--; ++p) {
// DEFAULT:  20 │ │                   *p = ' ';
// DEFAULT:  21 │ │       }
// DEFAULT:  22 │ ╰─▶ }
// DEFAULT:  23 │
// DEFAULT:     ╰────
// SLATE-FILECHECK-END DEFAULT
