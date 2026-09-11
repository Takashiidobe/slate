#define VALUE 7
#define TRANSITIVE VALUE
#define ID(x) x
#define JOIN(a, b) a ## b

int ordinary = TRANSITIVE;

#define SELF SELF
int recursive __attribute__((slate_literal(SELF)));

#ifdef SELECT
#define CHOICE 1
int selected __attribute__((slate_literal(CHOICE)));
#else
#define CHOICE 2
int selected __attribute__((slate_literal(CHOICE)));
#endif

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES SELECT SELECT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: polyvariant:
// DEFAULT-NEXT: decl[0]: declaration type=int declarator=name=ordinary initializer=expr(7)
// DEFAULT-NEXT: decl[1]: declaration type=int declarator=name=recursive [attributes=slate_literal(SELF)]
// DEFAULT-NEXT: decl[2]: conditional
// DEFAULT-NEXT:   branch[0]: when=defined(SELECT)
// DEFAULT-NEXT:     decl[0]: declaration type=int declarator=name=selected [attributes=slate_literal(1)]
// DEFAULT-NEXT:   branch[1]: when=not(defined(SELECT))
// DEFAULT-NEXT:     decl[0]: declaration type=int declarator=name=selected [attributes=slate_literal(2)]
// DEFAULT-NEXT: concrete:
// DEFAULT-NEXT: decl[0]: declaration type=int declarator=name=ordinary initializer=expr(7)
// DEFAULT-NEXT: decl[1]: declaration type=int declarator=name=recursive [attributes=slate_literal(SELF)]
// DEFAULT-NEXT: decl[2]: declaration type=int declarator=name=selected [attributes=slate_literal(2)]
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN SELECT
// SELECT: polyvariant:
// SELECT-NEXT: decl[0]: declaration type=int declarator=name=ordinary initializer=expr(7)
// SELECT-NEXT: decl[1]: declaration type=int declarator=name=recursive [attributes=slate_literal(SELF)]
// SELECT-NEXT: decl[2]: conditional
// SELECT-NEXT:   branch[0]: when=defined(SELECT)
// SELECT-NEXT:     decl[0]: declaration type=int declarator=name=selected [attributes=slate_literal(1)]
// SELECT-NEXT:   branch[1]: when=not(defined(SELECT))
// SELECT-NEXT:     decl[0]: declaration type=int declarator=name=selected [attributes=slate_literal(2)]
// SELECT-NEXT: concrete:
// SELECT-NEXT: decl[0]: declaration type=int declarator=name=ordinary initializer=expr(7)
// SELECT-NEXT: decl[1]: declaration type=int declarator=name=recursive [attributes=slate_literal(SELF)]
// SELECT-NEXT: decl[2]: declaration type=int declarator=name=selected [attributes=slate_literal(1)]
// SLATE-FILECHECK-END SELECT
