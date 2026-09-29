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
// DEFAULT: × linkage storage class
// DEFAULT: ╭─[tests/fixtures/suites/gcc-torture/clang/linux/x86_64/20010903-2.c:8:19]
// DEFAULT: 7 │ {
// DEFAULT: 8 │   auto inline int try (void *re);
// DEFAULT: ·                   ──────────────
// DEFAULT: 9 │
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × function definition is not allowed here
// DEFAULT: ╭─[tests/fixtures/suites/gcc-torture/clang/linux/x86_64/20010903-2.c:10:3]
// DEFAULT: 9 │
// DEFAULT: 10 │ ╭─▶   inline int try (void *re)
// DEFAULT: 11 │ │       {
// DEFAULT: 12 │ │         return __dummy (re, response);
// DEFAULT: 13 │ ╰─▶     }
// DEFAULT: 14 │       static void *yesre;
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
