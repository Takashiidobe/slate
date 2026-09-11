int main() {
#if 0
  return 0;
#elif 1
#ifdef INNER
  return 1;
#else
  return 2;
#endif
#else
  return 3;
#endif
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES INNER INNER

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: polyvariant:
// DEFAULT-NEXT: decl[0]: function name=main return=int
// DEFAULT-NEXT:   stmt[0]: conditional
// DEFAULT-NEXT:     branch[0]: when=constant(0)
// DEFAULT-NEXT:       stmt[0]: return 0
// DEFAULT-NEXT:     branch[1]: when=and(not(constant(0)), constant(1))
// DEFAULT-NEXT:       stmt[0]: conditional
// DEFAULT-NEXT:         branch[0]: when=defined(INNER)
// DEFAULT-NEXT:           stmt[0]: return 1
// DEFAULT-NEXT:         branch[1]: when=not(defined(INNER))
// DEFAULT-NEXT:           stmt[0]: return 2
// DEFAULT-NEXT:     branch[2]: when=not(or(constant(0), and(not(constant(0)), constant(1))))
// DEFAULT-NEXT:       stmt[0]: return 3
// DEFAULT-NEXT: concrete:
// DEFAULT-NEXT: decl[0]: function name=main return=int
// DEFAULT-NEXT:   stmt[0]: return 2
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN INNER
// INNER: polyvariant:
// INNER-NEXT: decl[0]: function name=main return=int
// INNER-NEXT:   stmt[0]: conditional
// INNER-NEXT:     branch[0]: when=constant(0)
// INNER-NEXT:       stmt[0]: return 0
// INNER-NEXT:     branch[1]: when=and(not(constant(0)), constant(1))
// INNER-NEXT:       stmt[0]: conditional
// INNER-NEXT:         branch[0]: when=defined(INNER)
// INNER-NEXT:           stmt[0]: return 1
// INNER-NEXT:         branch[1]: when=not(defined(INNER))
// INNER-NEXT:           stmt[0]: return 2
// INNER-NEXT:     branch[2]: when=not(or(constant(0), and(not(constant(0)), constant(1))))
// INNER-NEXT:       stmt[0]: return 3
// INNER-NEXT: concrete:
// INNER-NEXT: decl[0]: function name=main return=int
// INNER-NEXT:   stmt[0]: return 1
// SLATE-FILECHECK-END INNER
