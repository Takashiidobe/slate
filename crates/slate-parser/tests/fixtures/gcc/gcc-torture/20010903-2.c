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
// DEFAULT: ╭─[tests/fixtures/gcc/gcc-torture/20010903-2.c:8:3]
// DEFAULT: 7 │ {
// DEFAULT: 8 │   auto inline int try (void *re);
// DEFAULT: ·   ───────────────────────────────
// DEFAULT: 9 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
