int B;

#ifdef AS_CAST
typedef int A;
int cast_main() {
  (A)(B);
  return 0;
}
#else
int A(int value);
int call_main() {
  (A)(B);
  return 0;
}
#endif
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES CAST AS_CAST

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: polyvariant:
// DEFAULT-NEXT: decl[0]: declaration type=int declarator=name=B
// DEFAULT-NEXT: decl[1]: conditional
// DEFAULT-NEXT:   branch[0]: when=defined(AS_CAST)
// DEFAULT-NEXT:     decl[0]: typedef name=A type=int
// DEFAULT-NEXT:     decl[1]: function name=cast_main return=int
// DEFAULT-NEXT:       stmt[0]: cast type=A expression=B
// DEFAULT-NEXT:       stmt[1]: return 0
// DEFAULT-NEXT:   branch[1]: when=not(defined(AS_CAST))
// DEFAULT-NEXT:     decl[0]: declaration type=int declarator=function(name=A,params=int value,variadic=false)
// DEFAULT-NEXT:     decl[1]: function name=call_main return=int
// DEFAULT-NEXT:       stmt[0]: call callee=A argument=B
// DEFAULT-NEXT:       stmt[1]: return 0
// DEFAULT-NEXT: concrete:
// DEFAULT-NEXT: decl[0]: declaration type=int declarator=name=B
// DEFAULT-NEXT: decl[1]: declaration type=int declarator=function(name=A,params=int value,variadic=false)
// DEFAULT-NEXT: decl[2]: function name=call_main return=int
// DEFAULT-NEXT:   stmt[0]: call callee=A argument=B
// DEFAULT-NEXT:   stmt[1]: return 0
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN CAST
// CAST: polyvariant:
// CAST-NEXT: decl[0]: declaration type=int declarator=name=B
// CAST-NEXT: decl[1]: conditional
// CAST-NEXT:   branch[0]: when=defined(AS_CAST)
// CAST-NEXT:     decl[0]: typedef name=A type=int
// CAST-NEXT:     decl[1]: function name=cast_main return=int
// CAST-NEXT:       stmt[0]: cast type=A expression=B
// CAST-NEXT:       stmt[1]: return 0
// CAST-NEXT:   branch[1]: when=not(defined(AS_CAST))
// CAST-NEXT:     decl[0]: declaration type=int declarator=function(name=A,params=int value,variadic=false)
// CAST-NEXT:     decl[1]: function name=call_main return=int
// CAST-NEXT:       stmt[0]: call callee=A argument=B
// CAST-NEXT:       stmt[1]: return 0
// CAST-NEXT: concrete:
// CAST-NEXT: decl[0]: declaration type=int declarator=name=B
// CAST-NEXT: decl[1]: typedef name=A type=int
// CAST-NEXT: decl[2]: function name=cast_main return=int
// CAST-NEXT:   stmt[0]: cast type=A expression=B
// CAST-NEXT:   stmt[1]: return 0
// SLATE-FILECHECK-END CAST
