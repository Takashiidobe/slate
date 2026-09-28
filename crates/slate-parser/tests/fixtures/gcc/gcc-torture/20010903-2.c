// SLATE-FILECHECK-DEFINES DEFAULT

extern int __dummy (void *__preg, const char *__string);
extern int rpmatch (const char *response);

int
rpmatch (const char *response)
{
  auto inline int try (void *re);

  inline int try (void *re)
    {
      return __dummy (re, response);
    }
  static void *yesre;
  return (try (&yesre));
}

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × unsupported in numeric IR lowering: linkage storage class
// DEFAULT: ╭─[tests/fixtures/gcc/gcc-torture/20010903-2.c:5:1]
// DEFAULT: 4 │
// DEFAULT: 5 │ ╭─▶ int
// DEFAULT: 6 │ │   rpmatch (const char *response)
// DEFAULT: 7 │ │   {
// DEFAULT: 8 │ │     auto inline int try (void *re);
// DEFAULT: 9 │ │
// DEFAULT: 10 │ │     inline int try (void *re)
// DEFAULT: 11 │ │       {
// DEFAULT: 12 │ │         return __dummy (re, response);
// DEFAULT: 13 │ │       }
// DEFAULT: 14 │ │     static void *yesre;
// DEFAULT: 15 │ │     return (try (&yesre));
// DEFAULT: 16 │ ╰─▶ }
// DEFAULT: 17 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
