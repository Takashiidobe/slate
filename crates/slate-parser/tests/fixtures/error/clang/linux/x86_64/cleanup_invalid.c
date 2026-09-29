// SLATE-FILECHECK-DEFINES UNDECLARED UNDECLARED
// SLATE-FILECHECK-ERROR UNDECLARED
// SLATE-FILECHECK-DEFINES OBJECT OBJECT
// SLATE-FILECHECK-ERROR OBJECT
// SLATE-FILECHECK-DEFINES ARITY ARITY
// SLATE-FILECHECK-ERROR ARITY
// SLATE-FILECHECK-DEFINES UNPROTOTYPED UNPROTOTYPED
// SLATE-FILECHECK-STD UNPROTOTYPED gnu17
// SLATE-FILECHECK-ERROR UNPROTOTYPED
// SLATE-FILECHECK-DEFINES INCOMPATIBLE INCOMPATIBLE
// SLATE-FILECHECK-ERROR INCOMPATIBLE
// SLATE-FILECHECK-DEFINES SIGN SIGN
// SLATE-FILECHECK-ERROR SIGN

#if defined(OBJECT)
void (*release)(int *);
#elif defined(ARITY)
void release(int *p, int q);
#elif defined(UNPROTOTYPED)
void release();
#elif defined(INCOMPATIBLE)
void release(long *p);
#elif defined(SIGN)
void release(unsigned *p);
#endif

int f(void) {
  int x __attribute__((cleanup(release))) = 1;
  return x;
}

// SLATE-FILECHECK-BEGIN UNDECLARED
// UNDECLARED: Error:   × semantic analysis failed
// UNDECLARED: Error:
// UNDECLARED: × 'cleanup' argument is not a function
// UNDECLARED: ╭─[tests/fixtures/error/clang/linux/x86_64/cleanup_invalid.c:15:32]
// UNDECLARED: 14 │ int f(void) {
// UNDECLARED: 15 │   int x __attribute__((cleanup(release))) = 1;
// UNDECLARED: ·                                ───────
// UNDECLARED: 16 │   return x;
// UNDECLARED: ╰────
// SLATE-FILECHECK-END UNDECLARED
// SLATE-FILECHECK-BEGIN OBJECT
// OBJECT: Error:   × semantic analysis failed
// OBJECT: Error:
// OBJECT: × 'cleanup' argument is not a function
// OBJECT: ╭─[tests/fixtures/error/clang/linux/x86_64/cleanup_invalid.c:15:32]
// OBJECT: 14 │ int f(void) {
// OBJECT: 15 │   int x __attribute__((cleanup(release))) = 1;
// OBJECT: ·                                ───────
// OBJECT: 16 │   return x;
// OBJECT: ╰────
// SLATE-FILECHECK-END OBJECT
// SLATE-FILECHECK-BEGIN ARITY
// ARITY: Error:   × semantic analysis failed
// ARITY: Error:
// ARITY: × 'cleanup' function must take 1 parameter
// ARITY: ╭─[tests/fixtures/error/clang/linux/x86_64/cleanup_invalid.c:15:32]
// ARITY: 14 │ int f(void) {
// ARITY: 15 │   int x __attribute__((cleanup(release))) = 1;
// ARITY: ·                                ───────
// ARITY: 16 │   return x;
// ARITY: ╰────
// SLATE-FILECHECK-END ARITY
// SLATE-FILECHECK-BEGIN UNPROTOTYPED
// UNPROTOTYPED: Error:   × semantic analysis failed
// UNPROTOTYPED: Error:
// UNPROTOTYPED: × 'cleanup' function must take 1 parameter
// UNPROTOTYPED: ╭─[tests/fixtures/error/clang/linux/x86_64/cleanup_invalid.c:15:32]
// UNPROTOTYPED: 14 │ int f(void) {
// UNPROTOTYPED: 15 │   int x __attribute__((cleanup(release))) = 1;
// UNPROTOTYPED: ·                                ───────
// UNPROTOTYPED: 16 │   return x;
// UNPROTOTYPED: ╰────
// SLATE-FILECHECK-END UNPROTOTYPED
// SLATE-FILECHECK-BEGIN INCOMPATIBLE
// INCOMPATIBLE: Error:   × semantic analysis failed
// INCOMPATIBLE: Error:
// INCOMPATIBLE: × 'cleanup' function parameter type is incompatible with the variable's
// INCOMPATIBLE: ╭─[tests/fixtures/error/clang/linux/x86_64/cleanup_invalid.c:15:32]
// INCOMPATIBLE: 14 │ int f(void) {
// INCOMPATIBLE: 15 │   int x __attribute__((cleanup(release))) = 1;
// INCOMPATIBLE: ·                                ───────
// INCOMPATIBLE: 16 │   return x;
// INCOMPATIBLE: ╰────
// SLATE-FILECHECK-END INCOMPATIBLE
// SLATE-FILECHECK-BEGIN SIGN
// SIGN: Error:   × semantic analysis failed
// SIGN: Error:
// SIGN: × 'cleanup' function parameter type is incompatible with the variable's
// SIGN: ╭─[tests/fixtures/error/clang/linux/x86_64/cleanup_invalid.c:15:32]
// SIGN: 14 │ int f(void) {
// SIGN: 15 │   int x __attribute__((cleanup(release))) = 1;
// SIGN: ·                                ───────
// SIGN: 16 │   return x;
// SIGN: ╰────
// SLATE-FILECHECK-END SIGN
