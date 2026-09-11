#include "limits.h"
#include "myconfig.h"

ULONG_MAX_TYPE value;

int main() {
  return 0;
}

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: polyvariant:
// DEFAULT-NEXT: decl[0]: typedef name=ULONG_MAX_TYPE type=int
// DEFAULT-NEXT: decl[1]: declaration type=ULONG_MAX_TYPE declarator=name=value
// DEFAULT-NEXT: decl[2]: function name=main return=int
// DEFAULT-NEXT:   stmt[0]: return 0
// DEFAULT-NEXT: concrete:
// DEFAULT-NEXT: decl[0]: typedef name=ULONG_MAX_TYPE type=int
// DEFAULT-NEXT: decl[1]: declaration type=ULONG_MAX_TYPE declarator=name=value
// DEFAULT-NEXT: decl[2]: function name=main return=int
// DEFAULT-NEXT:   stmt[0]: return 0
// SLATE-FILECHECK-END DEFAULT
