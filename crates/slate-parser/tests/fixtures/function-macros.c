#define VALUE 7
#define ADD(left, right) left + right
#define STRINGIFY(value) #value
#define JOIN(left, right) left ## right
#define REST(first, ...) __VA_ARGS__
#define WRAP(value) value

int added __attribute__((slate_macro(ADD(1, 2))));
int stringified __attribute__((slate_macro(STRINGIFY(hello world))));
int joined __attribute__((slate_macro(JOIN(foo, bar))));
int variadic __attribute__((slate_macro(REST(first, 1, 2))));
int prescanned() {
  return WRAP(VALUE);
}

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: polyvariant:
// DEFAULT-NEXT: decl[0]: declaration type=int declarator=name=added [attributes=slate_macro(1 + 2)]
// DEFAULT-NEXT: decl[1]: declaration type=int declarator=name=stringified [attributes=slate_macro("hello world")]
// DEFAULT-NEXT: decl[2]: declaration type=int declarator=name=joined [attributes=slate_macro(foobar)]
// DEFAULT-NEXT: decl[3]: declaration type=int declarator=name=variadic [attributes=slate_macro(1 , 2)]
// DEFAULT-NEXT: decl[4]: function name=prescanned return=int
// DEFAULT-NEXT:   stmt[0]: return 7
// DEFAULT-NEXT: concrete:
// DEFAULT-NEXT: decl[0]: declaration type=int declarator=name=added [attributes=slate_macro(1 + 2)]
// DEFAULT-NEXT: decl[1]: declaration type=int declarator=name=stringified [attributes=slate_macro("hello world")]
// DEFAULT-NEXT: decl[2]: declaration type=int declarator=name=joined [attributes=slate_macro(foobar)]
// DEFAULT-NEXT: decl[3]: declaration type=int declarator=name=variadic [attributes=slate_macro(1 , 2)]
// DEFAULT-NEXT: decl[4]: function name=prescanned return=int
// DEFAULT-NEXT:   stmt[0]: return 7
// SLATE-FILECHECK-END DEFAULT
