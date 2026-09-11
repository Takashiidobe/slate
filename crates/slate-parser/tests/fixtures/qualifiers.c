const volatile int value;
static const int cached;
extern int external;
_Atomic int atomic_value;
typedef const int ConstInt;
int *const pointer;
const int *restrict qualified_pointer;
static inline int helper() {
  return 1;
}

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: polyvariant:
// DEFAULT-NEXT: decl[0]: declaration type=int [qualifiers=const volatile] declarator=name=value
// DEFAULT-NEXT: decl[1]: declaration type=int [qualifiers=const,storage=static] declarator=name=cached
// DEFAULT-NEXT: decl[2]: declaration type=int [storage=extern] declarator=name=external
// DEFAULT-NEXT: decl[3]: declaration type=int [qualifiers=_Atomic] declarator=name=atomic_value
// DEFAULT-NEXT: decl[4]: typedef name=ConstInt type=qualified(int)
// DEFAULT-NEXT: decl[5]: declaration type=int declarator=pointer(qualifiers=const;name=pointer)
// DEFAULT-NEXT: decl[6]: declaration type=int [qualifiers=const] declarator=pointer(qualifiers=restrict;name=qualified_pointer)
// DEFAULT-NEXT: decl[7]: function name=helper return=int [storage=static,inline=true]
// DEFAULT-NEXT:   stmt[0]: return 1
// DEFAULT-NEXT: concrete:
// DEFAULT-NEXT: decl[0]: declaration type=int [qualifiers=const volatile] declarator=name=value
// DEFAULT-NEXT: decl[1]: declaration type=int [qualifiers=const,storage=static] declarator=name=cached
// DEFAULT-NEXT: decl[2]: declaration type=int [storage=extern] declarator=name=external
// DEFAULT-NEXT: decl[3]: declaration type=int [qualifiers=_Atomic] declarator=name=atomic_value
// DEFAULT-NEXT: decl[4]: typedef name=ConstInt type=qualified(int)
// DEFAULT-NEXT: decl[5]: declaration type=int declarator=pointer(qualifiers=const;name=pointer)
// DEFAULT-NEXT: decl[6]: declaration type=int [qualifiers=const] declarator=pointer(qualifiers=restrict;name=qualified_pointer)
// DEFAULT-NEXT: decl[7]: function name=helper return=int [storage=static,inline=true]
// DEFAULT-NEXT:   stmt[0]: return 1
// SLATE-FILECHECK-END DEFAULT
