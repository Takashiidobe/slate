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
// DEFAULT: Error:   × unsupported in numeric IR lowering: linkage storage class
// SLATE-FILECHECK-END DEFAULT
