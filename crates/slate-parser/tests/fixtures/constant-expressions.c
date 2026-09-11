enum Values {
  FIRST = 1 + 2 * 3,
  SECOND = (4 << 1),
};
int values[1 + 2 * 3];
int flags[(4 < 5) && 1];
#if 1 + 2 * 3 > 6
int selected[2 + 3];
#else
int rejected[0];
#endif

int main() {
  return 0;
}

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: polyvariant:
// DEFAULT-NEXT: decl[0]: enum name=Values
// DEFAULT-NEXT:   enumerator: name=FIRST value=7
// DEFAULT-NEXT:   enumerator: name=SECOND value=8
// DEFAULT-NEXT: decl[1]: declaration type=int declarator=array(name=values,size=7)
// DEFAULT-NEXT: decl[2]: declaration type=int declarator=array(name=flags,size=1)
// DEFAULT-NEXT: decl[3]: conditional
// DEFAULT-NEXT:   branch[0]: when=constant(1)
// DEFAULT-NEXT:     decl[0]: declaration type=int declarator=array(name=selected,size=5)
// DEFAULT-NEXT:   branch[1]: when=not(constant(1))
// DEFAULT-NEXT:     decl[0]: declaration type=int declarator=array(name=rejected,size=0)
// DEFAULT-NEXT: decl[4]: function name=main return=int
// DEFAULT-NEXT:   stmt[0]: return 0
// DEFAULT-NEXT: concrete:
// DEFAULT-NEXT: decl[0]: enum name=Values
// DEFAULT-NEXT: decl[1]: declaration type=int declarator=array(name=values,size=7)
// DEFAULT-NEXT: decl[2]: declaration type=int declarator=array(name=flags,size=1)
// DEFAULT-NEXT: decl[3]: declaration type=int declarator=array(name=selected,size=5)
// DEFAULT-NEXT: decl[4]: function name=main return=int
// DEFAULT-NEXT:   stmt[0]: return 0
// SLATE-FILECHECK-END DEFAULT
