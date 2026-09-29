// SLATE-FILECHECK-DEFINES OBJECT OBJECT
// SLATE-FILECHECK-ERROR OBJECT
// SLATE-FILECHECK-DEFINES ARITY ARITY
// SLATE-FILECHECK-ERROR ARITY
// SLATE-FILECHECK-DEFINES INCOMPATIBLE INCOMPATIBLE
// SLATE-FILECHECK-ERROR INCOMPATIBLE

#if defined(OBJECT)
int release;
#elif defined(ARITY)
void release(void);
#elif defined(INCOMPATIBLE)
void release(long *p);
#endif

int f(void) {
  int x __attribute__((cleanup(release))) = 1;
  return x;
}

// SLATE-FILECHECK-BEGIN OBJECT
// OBJECT: Error:   × semantic analysis failed
// OBJECT: Error:
// OBJECT: × 'cleanup' argument is not a function
// OBJECT: ╭─[tests/fixtures/error/gcc/linux/x86_64/cleanup_invalid.c:11:32]
// OBJECT: 10 │ int f(void) {
// OBJECT: 11 │   int x __attribute__((cleanup(release))) = 1;
// OBJECT: ·                                ───────
// OBJECT: 12 │   return x;
// OBJECT: ╰────
// SLATE-FILECHECK-END OBJECT
// SLATE-FILECHECK-BEGIN ARITY
// ARITY: Error:   × semantic analysis failed
// ARITY: Error:
// ARITY: × 'cleanup' function must take 1 parameter
// ARITY: ╭─[tests/fixtures/error/gcc/linux/x86_64/cleanup_invalid.c:11:32]
// ARITY: 10 │ int f(void) {
// ARITY: 11 │   int x __attribute__((cleanup(release))) = 1;
// ARITY: ·                                ───────
// ARITY: 12 │   return x;
// ARITY: ╰────
// SLATE-FILECHECK-END ARITY
// SLATE-FILECHECK-BEGIN INCOMPATIBLE
// INCOMPATIBLE: Error:   × semantic analysis failed
// INCOMPATIBLE: Error: -Wincompatible-pointer-types
// INCOMPATIBLE: × incompatible pointer types
// INCOMPATIBLE: ╭─[tests/fixtures/error/gcc/linux/x86_64/cleanup_invalid.c:11:32]
// INCOMPATIBLE: 10 │ int f(void) {
// INCOMPATIBLE: 11 │   int x __attribute__((cleanup(release))) = 1;
// INCOMPATIBLE: ·                                ───────
// INCOMPATIBLE: 12 │   return x;
// INCOMPATIBLE: ╰────
// SLATE-FILECHECK-END INCOMPATIBLE
